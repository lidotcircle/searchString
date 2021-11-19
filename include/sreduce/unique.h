#ifndef _STRING_REDUCER_UNIQUE_H_
#define _STRING_REDUCER_UNIQUE_H_

#include "string_reducer.h"
#include <set>

class UniqueReducer : public StringReducer {
    private:
        std::vector<std::pair<size_t,std::string>> result;
        std::set<std::string> getted;

    public:
        UniqueReducer();
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_REDUCER_UNIQUE_H_
