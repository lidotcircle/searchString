#include "search_string_ascii.hpp"
#include "utils.h"
#include <gtest/gtest.h>
#include <tuple>
#include <string>
#include <vector>
using namespace std;


TEST(SearchStringAscii, WholeSentence) {
    const vector<string> sentences = {
        "hello world",
        "if you found life is hard you should give youself some hopes",

        "the protest on monday was against a requirement for staff to prove they had "
        "recieved a vaccine dose to access their workplace.",

        "it comes following an annoucement that from thursday 23 september "
        "construction workers will be required to show proof that they have had at least "
        "one vaccine dose in order to continue to work, local media report.",

        " "
    };

    for (auto &str : sentences)
    {
        auto buf = str.c_str();
        auto gbegin = asciiBegin(buf, buf + str.size(), true, true);
        auto gend = gbegin.end();

        ASSERT_NE(gbegin, gend);
        auto s1 = *gbegin;
        auto str1 = std::get<2>(s1);
        EXPECT_EQ(str1, str);
        EXPECT_EQ(str.size(), std::get<1>(s1) - std::get<0>(s1));
        gbegin++;
        ASSERT_TRUE(gbegin == gend) << str;
    }
}
