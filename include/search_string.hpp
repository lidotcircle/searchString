#ifndef _SEARCH_STRING_
#define _SEARCH_STRING_

#include <vector>
#include <string>
#include <tuple>
#include <type_traits>
#include <iterator>
#include <exception>
#include <memory>

#include "optional.hpp"
#include "string_mapper.h"
#include "string_filter.h"

template <
    typename InIter,
    typename = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<InIter>::iterator_category,
                            std::input_iterator_tag>::value,
        void>::type,
    typename = typename std::enable_if<
        std::is_integral<
            typename std::iterator_traits<InIter>::value_type>::value,
        void>::type,
    typename = typename std::enable_if<
        sizeof(typename std::iterator_traits<InIter>::value_type) == 1, void>::type>
class SearchStringInputIter
{
public:
    typedef size_t difference_type;
    typedef std::tuple<size_t, size_t, std::string> value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef std::input_iterator_tag iterator_catogory;

protected:
    typedef std::vector<value_type> output_array_t;
    size_t currentPosition() { return this->cur_pos; }
    bool generate_string() { return this->_generate_string; }

private:
    using input_iter = typename std::remove_reference<InIter>::type;
    Optional<input_iter> iter_begin, iter_end;
    output_array_t out;

    size_t cur_pos;
    bool is_proxy;
    value_type proxy_value;
    bool _generate_string;
    std::vector<std::shared_ptr<StringMapper>> mappers;
    std::vector<std::shared_ptr<StringFilter>> filters;

    SearchStringInputIter(const value_type &o) : is_proxy(true), proxy_value(o) {}

    void feed_output()
    {
        while (this->out.empty() && this->iter_begin.get() != this->iter_end.get())
        {
            unsigned char c = *this->iter_begin.get();
            for (auto &cn : this->feed_char(c))
            {
                const std::string &m = std::get<2>(cn);

                if (!this->filter(m))
                    continue;
                std::get<2>(cn) = this->map(m);

                this->out.push_back(std::move(cn));
            }
            ++this->iter_begin.get();
            this->cur_pos++;
        }

        if (this->iter_begin.get() == this->iter_end.get())
        {
            for (auto &cn : this->feed_char('\0'))
            {
                const std::string &m = std::get<2>(cn);

                if (!this->filter(m))
                    continue;
                std::get<2>(cn) = this->map(m);

                this->out.push_back(std::move(cn));
            }
        }
    }

    bool is_end()
    {
        if (!this->iter_begin)
            return true;

        if (this->out.empty())
        {
            if (this->iter_begin.get() == this->iter_end.get())
            {
                return true;
            }
            else
            {
                this->feed_output();
                return this->out.empty();
            }
        }
        else
        {
            return false;
        }
    }

    bool filter(const std::string &str)
    {
        for (auto &filter : this->filters)
        {
            if (!filter->filter(str))
                return false;
        }

        return true;
    }

    std::string map(const std::string &str)
    {
        std::string ans = str;
        for (auto &mapper : this->mappers)
        {
            ans = mapper->map(ans);
        }

        return ans;
    }

protected:
    virtual output_array_t feed_char(unsigned char c)
    {
        throw std::runtime_error("call unimplemented virtual function");
    }

public:
    SearchStringInputIter(
        InIter begin, InIter end,
        bool generate_string = true) : iter_begin(begin), iter_end(end),
                                       is_proxy(false), proxy_value(),
                                       _generate_string(generate_string),
                                       cur_pos(0) {}
    SearchStringInputIter() : is_proxy(false) {}

    SearchStringInputIter end() { return SearchStringInputIter(); }

    SearchStringInputIter &operator++()
    {
        if (this->is_proxy)
            throw std::runtime_error("can't forward a proxy iterator");

        if (this->is_end())
            throw std::runtime_error("iterator in the end can't forward");

        this->out.erase(this->out.begin());
        if (this->out.empty())
            this->feed_output();

        return *this;
    }
    SearchStringInputIter operator++(int)
    {
        SearchStringInputIter ans = SearchStringInputIter(this->operator*());
        this->operator++();
        return ans;
    }

    reference operator*()
    {
        if (this->is_proxy)
            return this->proxy_value;

        this->feed_output();
        if (this->out.empty())
            throw std::runtime_error("access end of iterator");

        return this->out.front();
    }
    pointer operator->()
    {
        return &this->operator*();
    }

    bool operator==(const SearchStringInputIter &o) const
    {
        if (&o == this)
            return true;

        SearchStringInputIter *_this = const_cast<SearchStringInputIter *>(this);
        SearchStringInputIter &_o = const_cast<SearchStringInputIter &>(o);
        return _this->is_end() && _o.is_end();
    }
    bool operator!=(const SearchStringInputIter &o) const
    {
        return !this->operator==(o);
    }

    void add_mapper(std::shared_ptr<StringMapper> mapper)
    {
        this->mappers.push_back(mapper);
    }
    void add_filter(std::shared_ptr<StringFilter> filter)
    {
        this->filters.push_back(filter);
    }
};

#endif // _SEARCH_STRING_

