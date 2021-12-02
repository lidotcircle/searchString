#ifndef _REDUCER_FACTORY_H_
#define _REDUCER_FACTORY_H_

#include "string_reducer.h"
#include <memory>
#include <map>
#include <set>
#include <string>


namespace ReducerFactory {

typedef std::unique_ptr<StringReducer> (*create_reducer_func_t)(const std::string& params);

class ReducerGenerator {
public:
    virtual std::unique_ptr<StringReducer> operator()(const std::string& params) = 0;
};

std::unique_ptr<StringReducer>
create(const std::string& encoding, const std::string &reducer_expr);

const std::map<std::string,std::map<std::string,std::string>>&
get_supported_reducers();

int
register_reducer(const std::string& encoding,
                 const std::string& reducer_type,
                 const std::string& description,
                 std::shared_ptr<ReducerGenerator> factory);

int
register_reducer(const std::string& encoding,
                 const std::string& reducer_type,
                 const std::string& description,
                 create_reducer_func_t factory);

}

#endif // _REDUCER_FACTORY_H_
