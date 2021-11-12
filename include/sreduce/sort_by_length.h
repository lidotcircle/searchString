#ifndef _STRING_REDUCER_SORT_BY_LENGTH_H_
#define _STRING_REDUCER_SORT_BY_LENGTH_H_

#include "string_reducer.h"

class SortByLength : public StringReducer {
    private:
        bool ascend;
        std::vector<std::pair<size_t,std::string>> result;

    public:
        SortByLength(bool ascend);
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;
};

#endif // _STRING_REDUCER_SORT_BY_LENGTH_H_
