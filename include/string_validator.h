#ifndef _STRING_VALIDATOR_H_
#define _STRING_VALIDATOR_H_

#include <string>


class StringValidator {
    public:
        virtual bool validate(const std::string&) = 0;
        virtual ~StringValidator() = 0;
};

class MiniumLength: public StringValidator {
    private:
        size_t min;

    public:
        MiniumLength(size_t);
        bool validate(const std::string&) override;
        ~MiniumLength();
};


#endif // _STRING_VALIDATOR_H_

