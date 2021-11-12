#ifndef _STRING_REDUCER_H_
#define _STRING_REDUCER_H_

#include <string>
#include <vector>


class StringReducer {
    public:
        virtual void reduce(const std::pair<size_t,std::string>& pair) = 0;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() = 0;
};

#endif // _STRING_REDUCER_H_
