#if defined(_WIN32) || defined(_WIN64)
#include "main.h"
#include "win_process/win_process_native.h"
using namespace std;

int search_in_win_process(int pid,
                          const std::string& encoding,
                          const std::vector<std::string>& transforms,
                          bool print_prefix)
{
    try {
        WinProcessNative winprocess(pid);
        auto count  = winprocess.map_count();

        for (size_t i=0;i<count;i++) {
            auto mmap = winprocess.get_map(i);

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

#endif // defined(_WIN32) || defined(_WIN64)
