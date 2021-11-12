#ifndef _STRING_REDUCER_REVERSE_H_
#define _STRING_REDUCER_REVERSE_H_

#include "string_reducer.h"

class ReverseReducer : public StringReducer {
    private:
        std::vector<std::pair<size_t,std::string>> result;

    public:
        ReverseReducer() = default;
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;
};

#endif // _STRING_REDUCER_REVERSE_H_
