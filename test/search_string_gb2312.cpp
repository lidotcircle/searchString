#include "search_string_gb2312.hpp"
#include "traindata.h"
#include "utils.h"
#include <gtest/gtest.h>


TEST(SearchStringGB2312, WholeSentence) {
    for (auto &str : traindata_valid)
    {
        auto buf = str.c_str();
        auto gbegin = gb2312Begin(buf, buf + str.size(), true, true);
        auto gend = gbegin.end();

        ASSERT_NE(gbegin, gend);
        auto s1 = *gbegin;
        auto str1 = std::get<2>(s1);
        EXPECT_TRUE(false) << str1;
        EXPECT_EQ(str1, str);
        EXPECT_EQ(str.size(), std::get<1>(s1) - std::get<0>(s1));
        gbegin++;
        ASSERT_TRUE(gbegin == gend) << str2hexstr(str);
    }
}
