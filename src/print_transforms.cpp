#include "main.h"
using namespace std;


void print_transforms(const std::string& encoding)
{
    auto filter_info = FilterFactory::get_supported_filters();
    if (encoding != "all" && filter_info.find(encoding) == filter_info.end()) {
        std::cout << "No filters for " << encoding << std::endl;
    } else {
        if (encoding == "all") {
            for (auto& efilter : filter_info) {
                std::cout << "Filters for " << efilter.first << ":" << std::endl;

                size_t max = 0;
                for (auto& filter : efilter.second)
                    max = std::max(max, filter.first.size());

                for (auto& filter : efilter.second) {
                    std::cout << "  "  << filter.first 
                              << string(max - filter.first.size() + 2, ' ')
                              << filter.second << std::endl;
                }
                std::cout << std::endl;
            }
        } else {
            auto& efilter = filter_info.at(encoding);

            size_t max = 0;
            for (auto& filter : efilter)
                max = std::max(max, filter.first.size());

            for (auto& filter : efilter) {
                std::cout << "  "  << filter.first 
                    << string(max - filter.first.size() + 2, ' ')
                    << filter.second << std::endl;
            }
            std::cout << std::endl;

        }
    }

    auto mapper_info = MapperFactory::get_supported_mappers();
    if (encoding != "all" && mapper_info.find(encoding) == mapper_info.end()) {
        std::cout << "No mappers for " << encoding << std::endl;
    } else {
        if (encoding == "all") {
            for (auto& emapper : mapper_info) {
                std::cout << "mappers for " << emapper.first << ":" << std::endl;

                size_t max = 0;
                for (auto& mapper : emapper.second)
                    max = std::max(max, mapper.first.size());

                for (auto& mapper : emapper.second) {
                    std::cout << "  "  << mapper.first 
                              << string(max - mapper.first.size() + 2, ' ')
                              << mapper.second << std::endl;
                }
                std::cout << std::endl;
            }
        } else {
            auto& emapper = mapper_info.at(encoding);

            size_t max = 0;
            for (auto& mapper : emapper)
                max = std::max(max, mapper.first.size());

            for (auto& mapper : emapper) {
                std::cout << "  "  << mapper.first 
                    << string(max - mapper.first.size() + 2, ' ')
                    << mapper.second << std::endl;
            }
            std::cout << std::endl;
        }
    }

    auto reducer_info = ReducerFactory::get_supported_reducers();
    if (encoding != "all" && reducer_info.find(encoding) == reducer_info.end()) {
        std::cout << "No reducers for " << encoding << std::endl;
    } else {
        if (encoding == "all") {
            for (auto& ereducer : reducer_info) {
                std::cout << "reducers for " << ereducer.first << ":" << std::endl;

                size_t max = 0;
                for (auto& reducer : ereducer.second)
                    max = std::max(max, reducer.first.size());

                for (auto& reducer : ereducer.second) {
                    std::cout << "  "  << reducer.first 
                              << string(max - reducer.first.size() + 2, ' ')
                              << reducer.second << std::endl;
                }
                std::cout << std::endl;
            }
        } else {
            auto& ereducer = reducer_info.at(encoding);

            size_t max = 0;
            for (auto& reducer : ereducer)
                max = std::max(max, reducer.first.size());

            for (auto& reducer : ereducer) {
                std::cout << "  "  << reducer.first 
                    << string(max - reducer.first.size() + 2, ' ')
                    << reducer.second << std::endl;
            }
            std::cout << std::endl;
        }
    }
}
