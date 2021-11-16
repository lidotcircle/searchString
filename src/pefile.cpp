#include "main.h"
#include "win_process/win_process_pefile.h"
#include <iomanip>
using namespace std;


int search_in_pefile(const std::string& pefile, 
                     const std::string& encoding, 
                     const vector<string>& transforms,
                     bool print_prefix)
{
    try {
        WinProcessPEFile pefile_obj(pefile);
        auto count  = pefile_obj.map_count();

        for (size_t i=0;i<count;i++) {
            auto mmap = pefile_obj.get_map(i);

            auto getter = GetterFactory::create_by_exprs(
                    encoding, transforms, mmap->begin(), mmap->end());
            size_t base = reinterpret_cast<size_t>(mmap->baseaddr());

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

