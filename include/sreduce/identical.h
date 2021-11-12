#ifndef _SREDUCE_IDENTICAL_H_
#define _SREDUCE_IDENTICAL_H_

#include "string_reducer.h"


class IdenticalReducer : public StringReducer {
    private:
        std::vector<std::pair<size_t,std::string>> saver;

    public:
        IdenticalReducer() = default;

        virtual void reduce(const std::pair<size_t,std::string>& pair)  override;
        virtual std::vector<std::pair<size_t,std::string>> reduce_end() override;
};


#endif // _SREDUCE_IDENTICAL_H_
