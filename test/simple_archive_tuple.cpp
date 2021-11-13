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
}

