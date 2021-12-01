#include "main.h"
#include "utils.h"
#include "lua/lua_wrapper.h"
#include <filesystem>
#include <fstream>
using namespace std;


int main(int argc, char** argv) {
    if (argc > 1 && string(argv[1]) == "trainsvm")
        return trainsvm_cmd(argc - 1, argv + 1);
    
    string encoding;
    string list;
    string pefile;
    string processname;
    string need_filter;
    string lua_script_path;
    bool disable_lua = false;
    int pid;

    string train_output;
    vector<string> train_dirs;
    bool print_prefix = false;

    string model;
    vector<string> input_files;
    vector<string> filters;
    vector<string> mappers;
    vector<string> reducers;
    vector<string> transforms;

    cxxopts::Options options("StringFinder",
            "extract string from a binary file, "
            "support various encoding schema eg. ascii, gb2312");
    options.set_width(100);

    options.add_options("extract", {
        { "f,filter",    "string filters, applied in specified order", cxxopts::value<vector<string>>(filters),  "<filters>" },
        { "m,mapper",    "string mappers, applied in specified order", cxxopts::value<vector<string>>(mappers),  "<mappers>" },
        { "r,reducer",   "string mappers, applied in specified order", cxxopts::value<vector<string>>(reducers), "<reducers>" },
        { "t,transform", "transformations include filter, mapper, reducer. "
                         "prefixed by 'f','m','r' respectively. "
                         "this option is exclusive with -fmr", cxxopts::value<vector<string>>(transforms), "<transform>" },
        { "n,num",       "print offset",                               cxxopts::value<bool>(print_prefix) },
        { "i,input",     "input file", cxxopts::value<vector<string>>(input_files) },
    });
    options.custom_help("[ trainsvm ]");
    options.parse_positional("input");
    options.positional_help("<files>");

    if (lua_wrapper) {
        options.add_options()
            ("luas", "lua script path with following priority:\n"
                     "{ this option > ./strfd.lua > $HOME/strfd.lua }",
              cxxopts::value<string>(lua_script_path), "<path>");
        options.add_options()
            ("stop-lua", "disable lua", cxxopts::value<bool>(disable_lua));
    }

    options.add_options()
        ("e,encoding", "support ascii and gb2312", cxxopts::value<string>(encoding)->default_value("gb2312"), "<encoding>")
        ("list",       "list filters, mappers and reducers", cxxopts::value<string>(list), "[encoding | all]")
        ("pe",         "search in pe file, which will do searching regarding pe section", cxxopts::value(pefile), "<pefile>")
        ("need",       "filter pe modules and sections, only meaningful in searching PE."
                       "spec: module, module@section, @section or empty for all pages, support regex", cxxopts::value(need_filter), "<need-filter>")
#if defined(_WIN32) || defined(_WIN64)
        ("pid",      "search process", cxxopts::value(pid), "<pid>")
        ("process",  "process name", cxxopts::value(processname), "<process-name>")
#endif // defined(_WIN32) || defined(_WIN64)
        ("h,help",     "print help");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (std::exception e) {
        cout << "bad arguments" << endl;
        cout << options.help() << endl;
        return 1;
    }

    if (result.count("help")) {
        cout << options.help() << endl;
        return 0;
    }

    if (result.count("luas")) {
        if (!std::filesystem::is_regular_file(lua_script_path)) {
            cout << "lua script path not found: " << lua_script_path << endl;
            return 1;
        }
    }

    if (lua_script_path.empty()) {
        static const string strfd_lua_cwd  = "./strfd.lua";
#if defined(_WIN32) || defined(_WIN64)
        static const string strfd_lua_home = string(std::getenv("UserProfile")) + "\\strfd.lua";
#else
        static const string strfd_lua_home = string(std::getenv("HOME")) + "/strfd.lua";
#endif

        if (std::filesystem::is_regular_file(strfd_lua_cwd)) {
            lua_script_path = strfd_lua_cwd;
        } else if (std::filesystem::is_regular_file(strfd_lua_home)) {
            lua_script_path = strfd_lua_home;
        }
    }

    if (!lua_script_path.empty() && !disable_lua) {
        try {
            std::ifstream luasf(lua_script_path, std::ios::in | std::ios::binary);
            if (!luasf)
                throw std::runtime_error("open lua script file '" + lua_script_path + "' failed");

            string luascript = string((std::istreambuf_iterator<char>(luasf)),
                                      std::istreambuf_iterator<char>());
            if (!setup_lua_strfd(luascript))
                throw std::runtime_error("can't load luascript");
        } catch (std::exception& e) {
            cout << "lua script error: " << endl << e.what() << endl;
            return 1;
        }
    }

    if (result.count("filter") || result.count("mapper") || result.count("reducer")) {
        if (result.count("transform")) {
            cerr << "transform and filters, mappers and reducers are exclusive" << endl;
            cerr << options.help() << endl;
            return 1;
        }
    }

    if (transforms.empty()) {
        for (auto& f: filters)
            transforms.push_back("f" + f);
        for (auto& m: mappers)
            transforms.push_back("m" + m);
        for (auto& r: reducers)
            transforms.push_back("r" + r);
    }

    if (result.count("list")) {
        print_transforms(list);
        return 0;
    }

    int nmode = 0;
    if (result.count("pid"))
        nmode++;
    if (result.count("process"))
        nmode++;
    if (result.count("pe"))
        nmode++;
    if (!input_files.empty())
        nmode++;
    if (nmode != 1) {
        cerr << "bad command line: specify pefile or pid or files only one" << endl;
        cerr << options.help() << endl;
        return 1;
    }

    if (result.count("pe")) {
        return search_in_pefile(pefile, encoding, transforms, print_prefix, need_filter);
    }

#if defined(_WIN32) || defined(_WIN64)
    if (result.count("pid")) {
        return search_in_win_process(pid, encoding, transforms, print_prefix, need_filter);
    }
    if (result.count("process")) {
        pid = GetPIDByProcessName(processname);
        if (pid == 0) {
            cerr << "process not found" << endl;
            return 1;
        }
        return search_in_win_process(pid, encoding, transforms, print_prefix, need_filter);
    }
#endif // defined(_WIN32) || defined(_WIN64)

    if (input_files.empty()) {
        cout << options.help() << endl;
        return 1;
    }

    int status = 0;
    for (auto& filename: input_files) {
        std::ifstream inputFile(filename, std::ios::binary);

        if(!inputFile.is_open()) {
            cout << "fail to open file" << endl;
            status = 1;
            continue;
        }
        cerr << "Search " << filename << ":" << endl;
        std::istreambuf_iterator<char> inputBegin(inputFile), inputEnd;

        try
        {
            auto getter = GetterFactory::create_by_exprs(encoding, transforms, inputBegin, inputEnd);
            for (auto &ss : *getter.get())
            {
                if (print_prefix)
                    cout << "0x" << std::hex << ss.first << ": ";

                cout << ss.second << endl;
            }
            cerr << "Search Finish" << endl;
        }
        catch (std::runtime_error &e)
        {
            cerr << e.what() << endl;
            status = 1;
            break;
        }
    }

    return status;
}

