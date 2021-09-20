#ifndef _STRING_VALIDATOR_H_
#define _STRING_VALIDATOR_H_

#include <string>


class StringFilter {
    public:
        virtual bool filter(const std::string&) = 0;
        virtual ~StringFilter() = 0;
};

class MiniumLength: public StringFilter {
    private:
        size_t min;

    public:
        MiniumLength(size_t);
        bool filter(const std::string&) override;
        ~MiniumLength();
};


#endif // _STRING_VALIDATOR_H_

