#ifndef _WIN_PROCESS_PEFILE_H_
#define _WIN_PROCESS_PEFILE_H_

#include "win_process.h"
#include <pe-parse/parse.h>
#include <memory>


class WinProcessPEFile: public WinProcess {
public:
    using PEFileHandle_t = std::unique_ptr<peparse::parsed_pe,void (*)(peparse::parsed_pe*)>;

private:
    PEFileHandle_t pehandle;
    std::vector<std::shared_ptr<ProcessMap>> mmaps;

public:
    WinProcessPEFile(const std::string& path);

    virtual size_t map_count() const override;
    virtual std::shared_ptr<ProcessMap> get_map(size_t index) override;
};

#endif // _WIN_PROCESS_PEFILE_H_
