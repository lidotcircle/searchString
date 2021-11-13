#include <gtest/gtest.h>
#include <map>
#include "svm/word_counter_ngram.hpp"
#include <string>
using namespace std;

static const string test_words_a[] = {
    "hello world",
    "good morning",
    "good afternoon",
    "good evening",
    "good night",
    "good bye",
    "good luck",
    "today is a good day",
    "today is a bad day",
};

TEST(WordCounterNGram, 1gram) {
    auto wc = WordCounterNGram<1,char>();
    for (auto& w : test_words_a)
        wc.train(w.begin(), w.end());

    string good = "good";
    auto a = wc.feature(good.begin(), good.end());
    EXPECT_EQ(a[0], 4);
    EXPECT_GT(a[1], 7);

    size_t n = 0, m = 0;
    EXPECT_FALSE(wc.save(nullptr, 0, n));
    EXPECT_GT(n, 0);
    char buf[4096];
    EXPECT_TRUE(wc.save(buf, sizeof(buf), m));
    EXPECT_EQ(m, n);

    auto wd = WordCounterNGram<1,char>();
    auto b = wd.feature(good.begin(), good.end());
    EXPECT_EQ(b[0], 4);
    EXPECT_EQ(b[1], 0);
    EXPECT_TRUE(wd.load(buf, sizeof(buf), n));
    EXPECT_EQ(n, m);

    b = wd.feature(good.begin(), good.end());
    EXPECT_EQ(b[0], 4);
    EXPECT_EQ(b[1], a[1]);
}

