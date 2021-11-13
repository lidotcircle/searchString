#include <gtest/gtest.h>
#include <map>
#include "simple_archive.hpp"
using namespace std;

TEST(TupleArchive, IntegerElem) {
    tuple<int> t(1);
    char buf[1024];

    size_t n = 0;
    writeToBuf(t, buf, sizeof(buf), n);
    EXPECT_EQ(n, sizeof(int));

    tuple<int> t11;
    readFromBuf(t11, buf, sizeof(buf), n);
    EXPECT_EQ(n, sizeof(int));
    EXPECT_EQ(get<0>(t11), 1);

    tuple<> t2;
    writeToBuf(t2, buf, sizeof(buf), n);
    EXPECT_EQ(n, 0);

    tuple<map<int, int>> t3;
    get<0>(t3).insert(make_pair(1, 1));
    get<0>(t3).insert(make_pair(2, 1));
    get<0>(t3).insert(make_pair(3, 1));
    EXPECT_FALSE(writeToBuf(t3, buf, 0, n));
    EXPECT_GT(n, 0);
    size_t m;
    EXPECT_TRUE(writeToBuf(t3, buf, n, m));
    EXPECT_EQ(m, n);
}

TEST(ArrayArchive, IntegerElem) {
    array<int, 3> t{1, 2, 3};
    char buf[1024];

    size_t n = 0, m = 0;
    EXPECT_FALSE(writeToBuf(t, nullptr, 0, n));
    EXPECT_GT(n, 0);
    EXPECT_TRUE(writeToBuf(t, buf, sizeof(buf), m));
    EXPECT_EQ(m, n);

    array<int, 3> t11;
    EXPECT_TRUE(readFromBuf(t11, buf, sizeof(buf), m));
    EXPECT_EQ(n, m);
    EXPECT_EQ(n, sizeof(int) * 3);
    EXPECT_EQ(t11[0], 1);
    EXPECT_EQ(t11[1], 2);
    EXPECT_EQ(t11[2], 3);
}
