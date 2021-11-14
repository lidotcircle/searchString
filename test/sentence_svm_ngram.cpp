#include <gtest/gtest.h>
#include "svm/sentence_svm_ngram.hpp"
#include <vector>
#include <iostream>
#include <memory>
using namespace std;

class NGramSentenceSVMTest: public ::testing::Test {
    protected:
        NGramSentenceSVM<2,uint16_t,dlib::svm_c_trainer,dlib::radial_basis_kernel> svm, svm2;
        vector<decltype(svm)::sample_type> samples;
        vector<double> labels;
        vector<std::vector<uint16_t>> uy;

        void SetUp() override {
            for(uint16_t i=0;i<100;i++) {
                vector<uint16_t> mn;

                for(uint16_t j=0;j<100;j++)
                    mn.push_back(i << 7 & j);

                svm.word_counter_feed(mn.begin(), mn.end());
            }

            for(uint16_t i=0;i<8;i++) {
                vector<uint16_t> mn;

                for(uint16_t j=0;j<20;j++)
                    mn.push_back(i << 5 & j);

                uy.push_back(mn);
            }

            for(uint16_t i=0;i<30;i++) {
                vector<uint16_t> mn;

                for(uint16_t j=0;j<30;j++)
                    mn.push_back((j << (i << 7) & 0x0F) + i + j);

                samples.push_back(svm.feature(mn.begin(), mn.end()));
                labels.push_back((i % 10 < 5) ? -1 : 1);
            }
            svm.train(samples, labels);
        }
};


TEST_F(NGramSentenceSVMTest, TrainPredict) {
    for(auto& un: uy) {
        auto v = svm.predict(un.begin(), un.end());
    }
}

TEST_F(NGramSentenceSVMTest, SaveLoadWithBuffer) {
    size_t bufsize, b2;
    ASSERT_FALSE(svm.save(nullptr, 0, bufsize));
    std::shared_ptr<char> buf(new char[bufsize], std::default_delete<char[]>());
    ASSERT_TRUE(svm.save(buf.get(), bufsize, b2));
    EXPECT_EQ(b2, bufsize);

    ASSERT_TRUE(svm2.load(buf.get(), bufsize, b2));
    EXPECT_EQ(b2, bufsize);

    for(auto& un: uy) {
        auto v  = svm.predict(un.begin(), un.end());
        auto v2 = svm2.predict(un.begin(), un.end());
        EXPECT_EQ(v2, v);
    }
}

TEST_F(NGramSentenceSVMTest, SaveLoadWithIOS) {
    std::ostringstream oss;
    oss << svm;
    std::istringstream iss(oss.str());
    iss >> svm2;

    for(auto& un: uy) {
        auto v  = svm.predict(un.begin(), un.end());
        auto v2 = svm2.predict(un.begin(), un.end());
        EXPECT_EQ(v2, v);
    }
}

