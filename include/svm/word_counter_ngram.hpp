#ifndef _WORD_COUNTER_N_GRAM_HPP_
#define _WORD_COUNTER_N_GRAM_HPP_

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <type_traits>
#include <assert.h>
#include <iterator>
#include <array>
#include "simple_archive.hpp"
#include "utils.hpp"

template<size_t N, typename T>
using ntuple_t = std::array<T, N>;

template<size_t N, typename TWord>
class WordCounterNGram {
private:
    static_assert(N >= 1, "N must be greater than 1");
    using key_type = ntuple_t<N, TWord>;

    WordCounterNGram<N-1, TWord> pv;
    std::map<key_type,int> ngram_map;
    std::array<TWord, N> array;
    size_t validn = 0;
    int val = 0;

    void push_new_word(TWord word) {
        if (validn < N) {
            array[validn] = word;
            ++validn;
        } else {
            for (size_t i = 0; i < N-1; ++i)
                array[i] = array[i+1];
            array[N-1] = word;
        }
    }

public:
    void train_eat(TWord word) {
        this->pv.train_eat(word);

        push_new_word(word);
        if (validn == N)
            this->ngram_map[this->array] += 1;
    }

    void train_eat_end() {
        this->pv.train_eat_end();
        this->validn = 0;
    }

    template<
        typename TWordIter,
        typename = is_input_iterator_t<TWordIter>,
        typename = is_iterator_value_is_same_with_t<TWordIter, TWord>>
    auto train(TWordIter begin, TWordIter end) {
        this->validn = 0;

        for(;begin != end;begin++)
            this->train_eat(*begin);

        return this->train_eat_end();
    }

    bool save(char* buf, size_t bufsize, size_t& writed) const {
        bool ret = true;
        ret = this->pv.save(buf, bufsize, writed);

        size_t n = 0;
        size_t k = bufsize > writed ? bufsize - writed : 0;
        ret = writeToBuf(this->ngram_map, buf + writed, k, n);
        writed += n;

        return ret;
    }
    bool load(char* buf, size_t bufsize, size_t& read) {
        if (!this->pv.load(buf, bufsize, read))
            return false;

        if (bufsize < read)
            return false;

        size_t n = 0;
        if (!readFromBuf(this->ngram_map, buf + read, bufsize - read, n))
            return false;

        read += n;
        return true;
    }

    template<
        typename TWordIter,
        typename = is_input_iterator_t<TWordIter>,
        typename = is_iterator_value_is_same_with_t<TWordIter, TWord>>
    auto feature(TWordIter begin, TWordIter end) {
        assert(this->validn == 0 && "call eat_end() or feature_end() before another sentence");

        for(;begin != end;begin++)
            this->feature_eat(*begin);

        return this->feature_end();
    }

    void feature_eat(TWord word) {
        this->pv.feature_eat(word);

        push_new_word(word);
        if (this->validn != N)
            return;

        if (this->ngram_map.find(this->array) != this->ngram_map.end()) 
            this->val += this->ngram_map[this->array];
    }

    auto feature_end() {
        std::array<int,N+1> ret;
        auto nx = this->pv.feature_end();
        for (size_t i = 0; i < N; ++i)
            ret[i] = nx[i];
        ret[N] = this->val;

        this->validn = 0;
        this->val = 0;

        return ret;
    }
};


template<typename TWord>
class WordCounterNGram<0,TWord> {
private:
    int val = 0;

public:
    void train_eat(TWord word) {}
    void train_eat_end() {}

    void feature_eat(TWord word) {
        this->val++;
    }
    auto feature_end() {
        auto v = this->val;
        this->val = 0;
        return std::array<int,1>{v};
    }

    bool save(char* buf, size_t bufsize, size_t& writed) const {
        writed = 0;
        return true;
    }
    bool load(char* buf, size_t bufsize, size_t& readed) {
        readed = 0;
        return true; 
    }

};

#endif // _WORD_COUNTER_N_GRAMHPP_

