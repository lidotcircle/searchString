#include "sgetter/getter_factory.hpp"


std::unique_ptr<StringGetterBase>
create(
        std::unique_ptr<StringGetterBase> getter,
        std::unique_ptr<StringReducer> reducer)
{
    return make_string_getter(std::move(getter), std::move(reducer));
}
