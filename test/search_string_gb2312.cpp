#include "search_string.h"
#include "traindata.h"
#include "utils.h"
#include <gtest/gtest.h>
#include <algorithm>
#include <tuple>


TEST(SearchStringGB2312, WholeSentence) {
    for (auto &str : traindata_valid)
    {
        auto buf = str.c_str();
        auto cc = make_string_getter<StringFinderGB2312>(buf, buf + str.size());
        auto gbegin = cc->begin();
        auto gend = cc->end();

        ASSERT_NE(gbegin, gend);

        std::vector<std::string> strs;
        for(;gbegin != gend;gbegin++) strs.push_back(gbegin->second);
        EXPECT_GT(strs.size(), 0);
        std::sort(strs.begin(), strs.end(), [](std::string f1, std::string f2) {return f1.size() > f2.size();});

        EXPECT_EQ(strs[0], str);
    }
}
