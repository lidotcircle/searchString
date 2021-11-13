#ifndef _STRING_REDUCER_MOST_REPEATED_N_H_
#define _STRING_REDUCER_MOST_REPEATED_N_H_

#include "string_reducer.h"
#include <map>

class MostRepeatedN : public StringReducer {
    private:
        size_t n;
        std::map<std::string,std::pair<size_t,size_t>> saver;

    public:
        MostRepeatedN(size_t n);
        virtual void reduce(const std::pair<size_t,std::string>& pair) override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_REDUCER_MOST_REPEATED_N_H_
