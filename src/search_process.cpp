#if defined(_WIN32) || defined(_WIN64)
#include "main.h"
#include "utils.h"
#include "process/win_process_native.h"
#include "process/memory_map_module.h"
#include "process/memory_map_section.h"
#include <memory>
using namespace std;

int search_in_win_process(int pid,
                          const std::string& encoding,
                          const std::vector<std::string>& transforms,
                          bool print_prefix,
                          const std::string& needfilter)
{
    auto printx = [&](std::shared_ptr<MemoryMap> mmap) {
        auto getter = GetterFactory::create_by_exprs(
            encoding, transforms, mmap->begin(), mmap->end());
        auto base = mmap->baseaddr();

        try
        {
            for (auto &n : *getter.get())
            {
                if (print_prefix)
                    std::cout << "0x" << std::hex << n.first + base << ": ";
                std::cout << n.second << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "warning: " << e.what() << std::endl;
        }
    };
    auto nf = parse_needfilter_regex(needfilter);
    auto mf = nf.first;
    auto sf = nf.second;

    try {
        WinProcessNative winprocess(pid);
        auto count  = winprocess.map_count();

        for (size_t i=0;i<count;i++) {
            auto mmap = winprocess.get_map(i);
            auto modmap = dynamic_pointer_cast<MemoryMapModule>(mmap);

            if (modmap) {
                auto modname = modmap->module_name();
                if (!regex_match(modname, mf))
                    continue;
                std::cout << "module: " << modname << std::endl;

                for (auto& sec: modmap->get_sections()) {
                    if (!regex_match(sec.first, sf))
                        continue;
                    std::cout << "module@section: " << modname << "@" << sec.first << std::endl;
                    printx(sec.second);
                }
            } else if (needfilter.empty()) {
                cout << "page: 0x" << std::hex << mmap->baseaddr() << endl;
                printx(mmap);
            }
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

}

#endif // defined(_WIN32) || defined(_WIN64)
