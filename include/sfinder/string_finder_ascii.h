#include "string_finder.h"


class StringFinderASCII: public StringFinder {
private:
    std::vector<std::pair<size_t,std::string>> outputs;
    std::string candidate;
    size_t candidate_begin;
    size_t pos;
    bool is_end;

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() override;

public:
    StringFinderASCII();

    virtual void feed_char(unsigned char c) override;
    virtual void feed_end() override;
};
