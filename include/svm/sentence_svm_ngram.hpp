#ifndef _SENTENCE_SVM_NGRAM_HPP_
#define _SENTENCE_SVM_NGRAM_HPP_

#include <map>
#include <vector>
#include <memory>
#include <dlib/svm.h>
#include <assert.h>
#include "word_counter_ngram.hpp"


template<size_t N, typename TWord>
class NGramSentenceSVM {
public:
    static_assert(N > 0, "N must be greater than 0");
    typedef dlib::matrix<double,2*N+1,1> sample_type;


private:
    typedef dlib::radial_basis_kernel<sample_type>    kernel_type;
    typedef dlib::decision_function<kernel_type>      dec_funct_type;
    typedef dlib::normalized_function<dec_funct_type> funct_type;
    WordCounterNGram<N,TWord> counter;
    funct_type learned_function;


public:
    void train(std::vector<sample_type> samples, std::vector<double> labels) 
    {
        assert(samples.size() == labels.size());

        dlib::vector_normalizer<sample_type> normalizer;
        normalizer.train(samples);
        for(size_t i=0;i<samples.size();i++)
            samples[i] = normalizer(samples[i]);
        dlib::randomize_samples(samples, labels);

        dlib::svm_c_trainer<kernel_type> trainer;
        double m = 0;
        double ga = 0;
        double ca = 0;
        for(double gamma = 0.00001;gamma<=1;gamma *= 5) {
            for(double C = 1; C<100000; C *= 5) {
                trainer.set_kernel(kernel_type(gamma));
                trainer.set_c(C);
                auto result = dlib::cross_validate_trainer(trainer, samples, labels, 3);
                double mx = 2 * dlib::prod(result) / dlib::sum(result);
                if(mx > m) {
                    ga = gamma;
                    ca = C;
                }
                m = mx > m ? mx : m;
            }
        }

        trainer.set_kernel(kernel_type(ga));
        trainer.set_c(ca);

        this->learned_function.normalizer = normalizer;
        this->learned_function.function = trainer.train(samples, labels);
    }

    template<typename Iter, typename = is_input_iterator_t<Iter>>
    void word_counter_feed(Iter begin, Iter end)
    {
        this->counter.train(begin, end);
    }

    template<typename Iter, typename = is_input_iterator_t<Iter>>
    auto feature(Iter begin, Iter end) const {
        auto ft = this->counter.feature(begin, end);
        sample_type ret;
        auto len = ft[0];
        ret(0) = len;
        for (size_t i=1;i <= N;i++) {
            ret(i) = ft[i];
            ret(i+N) = (double)ft[i] / len;
        }

        return ret;
    }

    template<typename Iter, typename = is_input_iterator_t<Iter>>
    bool predict(Iter begin, Iter end) const {
        auto ft = this->feature(begin, end);
        return this->learned_function(ft) > 0;
    }

    bool save(char* buf, size_t bufsize, size_t& writed) const {
        bool ret = true;

        ret = this->counter.save(buf, bufsize, writed);
        size_t k1 = writed;

        std::ostringstream membuf;
        dlib::serialize(membuf) << this->learned_function;
        size_t n = membuf.str().size();
        writed += n;
        writed += sizeof(size_t);

        if(bufsize < writed) {
            ret = false;
        } else {
            *(size_t*)(buf + k1) = n;
            memcpy(buf + k1 + sizeof(size_t), membuf.str().c_str(), membuf.str().size());
        }

        return ret;
    }

    bool load(char* buf, size_t bufsize, size_t& readed) {
        if (!this->counter.load(buf, bufsize, readed))
            return false;

        if (bufsize < readed + sizeof(size_t))
            return false;
        size_t n = *(size_t*)(buf + readed);
        readed += sizeof(size_t);

        if (bufsize < readed + n)
            return false;

        std::vector<char> data(buf + readed, buf + readed + n);
        dlib::deserialize(data) >> this->learned_function;
        readed += n;
        return true;
    }
};

template<size_t N,typename TWord>
std::ostream& operator<<(std::ostream& o, const NGramSentenceSVM<N,TWord>& s) {
    size_t n;
    if (s.save(nullptr, 0, n))
        throw std::runtime_error("expcet false");

    std::shared_ptr<char> buf(new char[n], std::default_delete<char[]>());
    if (!s.save(buf.get(), n, n))
        throw std::runtime_error("serialization failed");

    o.write(buf.get(), n);
    return o;
}

template<size_t N,typename TWord>
std::istream& operator>>(std::istream& input, NGramSentenceSVM<N,TWord>& s) {
    std::istream_iterator<char> eos;
    size_t cur = input.tellg();
    input.seekg(0, input.end);
    size_t end = input.tellg();
    input.seekg(cur, input.beg);
    size_t n = end - cur; 

    std::shared_ptr<char> buf(new char[n], std::default_delete<char[]>());
    input.read(buf.get(), n);

    if (!s.load(buf.get(), n, n))
        throw std::runtime_error("deserialization failed");

    return input;
}

#endif // _SENTENCE_SVM_NGRAM_HPP_
