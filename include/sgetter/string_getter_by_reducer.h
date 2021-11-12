#ifndef _STRING_SGETTER_GETTER_BY_REDUCER_H_
#define _STRING_SGETTER_GETTER_BY_REDUCER_H_

#include "string_getter_base.h"
#include "../sreduce/string_reducer.h"
#include <vector>
#include <string>

class StringGetterByReducer: public StringGetterBase {
private:
    std::vector<std::pair<size_t,std::string>> outputs;
    std::vector<std::shared_ptr<StringFilter>> filters;
    std::vector<std::shared_ptr<StringMapper>> mappers;
    std::unique_ptr<StringGetterBase> getter;
    std::unique_ptr<StringReducer>    reducer;
    bool dirty;

protected:
    virtual bool empty() const override;
    virtual std::pair<size_t,std::string>& top() override;
    virtual void pop() override;
    virtual void get_at_least_one_until_end() override;

public:
    StringGetterByReducer() = delete;
    StringGetterByReducer(
            std::unique_ptr<StringGetterBase> getter,
            std::unique_ptr<StringReducer> reducer);

    virtual void add_filter(std::shared_ptr<StringFilter> filter) override;
    virtual void add_mapper(std::shared_ptr<StringMapper> mapper) override;

    virtual StringGetterIter begin() override;
    virtual StringGetterIter end() override;

    virtual ~StringGetterByReducer() override = default;
};

std::unique_ptr<StringGetterBase>
make_string_getter( 
        std::unique_ptr<StringGetterBase> getter,
        std::unique_ptr<StringReducer> reducer);

#endif // _STRING_SGETTER_GETTER_BY_REDUCER_H_
