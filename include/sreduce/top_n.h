#ifndef _STRING_REDUCER_TOP_N_H_
#define _STRING_REDUCER_TOP_N_H_

#include "string_reducer.h"

class TopNReducer : public StringReducer {
    private:
        size_t n;
        std::vector<std::pair<size_t,std::string>> result;

    public:
        TopNReducer(size_t n);
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_REDUCER_TOP_N_H_
