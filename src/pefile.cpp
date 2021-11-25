#include "main.h"
#include "utils.h"
#include "process/win_process_pefile.h"
#include "process/memory_map_pefile.h"
#include <iomanip>
#include <regex>
using namespace std;


int search_in_pefile(const std::string& pefile, 
                     const std::string& encoding, 
                     const vector<string>& transforms,
                     bool print_prefix,
                     const string& needfilter)
{
    auto sf = parse_needfilter_regex(needfilter).second;
    try {
        WinProcessPEFile pefile_obj(pefile);
        auto count  = pefile_obj.map_count();

        for (size_t i=0;i<count;i++) {
            auto mmap = pefile_obj.get_map(i);
            auto pemmap = dynamic_pointer_cast<MemoryMapPEFile>(mmap);
            if (!needfilter.empty() && pemmap && !regex_match(pemmap->section_name(), sf))
                continue;

            if (pemmap) {
                cout << "section: " << pemmap->section_name() << endl;
            }

            auto getter = GetterFactory::create_by_exprs(
                    encoding, transforms, mmap->begin(), mmap->end());
            auto base = mmap->baseaddr();

            for(auto& n: *getter.get()) {
                if (print_prefix)
                    std::cout << "0x" << std::hex << n.first + base << ": ";
                std::cout << n.second << std::endl;
            }
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}

