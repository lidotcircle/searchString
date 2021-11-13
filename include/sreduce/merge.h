#ifndef _STRING_REDUCER_MERGE_H_
#define _STRING_REDUCER_MERGE_H_

#include "string_reducer.h"

class MergeReducer : public StringReducer {
    private:
        std::pair<size_t,std::string> result;

    public:
        MergeReducer() = default;
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_REDUCER_MERGE_H_
