#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include <string>
#include "search_string.h"
#include "traindata.h"


class GB2312FilterTest: public ::testing::Test {
    protected:
        GB2312SVMFilter filter;
        GB2312SVMFilter filter2;

        void SetUp() override {
            filter.svm_train(traindata_valid, traindata_invalid);
        }
};

TEST_F(GB2312FilterTest, SaveLoadWithBUffer) {
    size_t m,n;
    ASSERT_TRUE(filter.save(nullptr, 0, m));
    char* buf = new char[m];
    ASSERT_TRUE(filter.save(buf, m, n));
    ASSERT_EQ(m, n);

    ASSERT_TRUE(filter2.load(buf, m, n));
    ASSERT_EQ(m, n);

    for(auto& s: testdata_valid) 
        EXPECT_EQ(filter.filter(s), filter2.filter(s));
    for(auto& s: testdata_invalid) 
        EXPECT_EQ(filter.filter(s), filter2.filter(s));
}

TEST_F(GB2312FilterTest, SaveLoadWithIOS) {
    std::ostringstream oss;
    oss << filter;
    std::istringstream iss(oss.str());
    iss >> filter2;

    for(auto& s: testdata_valid) {
        auto o1 = filter.filter(s);
        EXPECT_EQ(o1, filter2.filter(s));
        EXPECT_TRUE(o1);
    }

    for(auto& s: testdata_invalid) {
        auto o1 = filter.filter(s);
        EXPECT_EQ(o1, filter2.filter(s));
        EXPECT_FALSE(o1);
    }
}

