#ifndef _SEARCH_STRING_
#define _SEARCH_STRING_

#include <vector>
#include <string>
#include <tuple>
#include <type_traits>
#include <iterator>
#include <exception>


template<
    typename InIter,
    typename = typename std::enable_if<
        std::is_convertible<typename std::iterator_traits<InIter>::iterator_category,
            std::input_iterator_tag>::value, void>::type,
    typename = typename std::enable_if<
        std::is_integral<
            typename std::iterator_traits<InIter>::value_type>::value, void>::type,
    typename = typename std::enable_if<
        sizeof(typename std::iterator_traits<InIter>::value_type) == 1, void>::type
    >
class SearchStringInputIter {
    public:
        typedef size_t                                difference_type;
        typedef std::tuple<size_t,size_t,std::string> value_type;
        typedef value_type&                           reference;
        typedef value_type*                           pointer;
        typedef std::input_iterator_tag               iterator_catogory;

    protected:
        typedef int candidate_state;
        typedef std::vector<value_type> output_array_t;
        typedef std::vector<std::tuple<candidate_state,size_t,size_t,std::string>> candidate_array_t;

    private:
        using input_iter = typename std::remove_reference<InIter>::type;

        union _IUnion {
            struct {
                bool is_iter;
                input_iter i;
            } m;

            struct {
                bool is_iter;
                char a[sizeof(input_iter)];
            } n;

            _IUnion(): n{false} {}
            _IUnion(input_iter iter): m{true, std::move(iter)} {}

            _IUnion(_IUnion&& o) {
                if(o.m.is_iter) {
                    this->m.is_iter = true;
                    this->m.i = std::move(o.m.i);
                } else {
                    this->m.is_iter = false;
                }
            }
            _IUnion(const _IUnion& o) {
                if(o.m.is_iter) {
                    this->m.is_iter = true;
                    this->m.i = o.m.i;
                } else {
                    this->m.is_iter = false;
                }
            }

            _IUnion& operator=(_IUnion&& o) {
                ~_IUnion();
                new (this)_IUnion(std::move(o));
                return *this;
            }

            _IUnion& operator=(const _IUnion& o) {
                ~_IUnion();
                new (this)_IUnion(o);
                return *this;
            }

            ~_IUnion() {
                if (this->m.is_iter) {
                    this->m.i.~input_iter();
                }
            }

            input_iter& get() {
                if(!this->m.is_iter) {
                    throw std::runtime_error("access inactive member in union");
                }

                return this->m.i;
            }

            operator bool() const {
                return this->m.is_iter;
            }
        } iter_begin, iter_end;
        output_array_t    out;
        candidate_array_t candidates;

        size_t     cur_pos;
        bool       is_proxy;
        value_type proxy_value;
        bool       _generate_string;
        size_t     _min_string_length;

        SearchStringInputIter(const value_type& o):
            is_proxy(true), proxy_value(o) {}

        void feed_output() {
            while(this->out.empty()
                    && this->iter_begin.get() != this->iter_end.get()) 
            {
                unsigned char c = *this->iter_begin.get();
                this->out = this->feed_char(this->cur_pos, c, this->candidates);
                ++this->iter_begin.get();
                this->cur_pos++;
            }

            if(this->iter_begin.get() == this->iter_end.get() 
                    && !this->candidates.empty()) 
            {
                for(auto& cn: this->feed_char(this->cur_pos, '\0', this->candidates)) {
                    this->out.push_back(std::move(cn));
                }
            }
        }

        bool is_end() {
            if(!this->iter_begin) {
                return true;
            }

            if (this->out.empty()) {
                if(this->iter_begin.get() == this->iter_end.get()) {
                    return true;
                } else {
                    this->feed_output();
                    return this->out.empty();
                }
            } else {
                return false;
            }
        }


    protected:
        virtual output_array_t feed_char(size_t pos, unsigned char c, candidate_array_t& _candidates) {
            throw std::runtime_error("call unimplemented virtual function");
        }
        bool generate_string() {return this->_generate_string;}
        size_t min_string_length() {return this->_min_string_length;}


    public:
        SearchStringInputIter(
                InIter begin, InIter end, 
                bool generate_string = true, size_t min_len = 0
                ): 
            iter_begin(begin), iter_end(end),
            is_proxy(false), proxy_value(),
            _generate_string(generate_string), _min_string_length(min_len),
            cur_pos(0) {}
        SearchStringInputIter(): is_proxy(false) {}

        SearchStringInputIter end() {return SearchStringInputIter();}

        SearchStringInputIter& operator++() {
            if(this->is_proxy) {
                throw std::runtime_error("can't forward a proxy iterator");
            }

            if(this->is_end()) {
                throw std::runtime_error("iterator in the end can't forward");
            }

            this->out.erase(this->out.begin());
            if(this->out.empty()) {
                this->feed_output();
            }

            return *this;
        }
        SearchStringInputIter  operator++(int) {
            SearchStringInputIter ans = SearchStringInputIter(this->operator*());
            this->operator++();
            return ans;
        }

        reference operator*() {
            if(this->is_proxy) {
                return this->proxy_value;
            }

            this->feed_output();
            if(this->out.empty()) {
                throw std::runtime_error("access end of iterator");
            }

            return this->out.front();
        }
        pointer   operator->() {
            return &this->operator*();
        }

        bool operator==(const SearchStringInputIter& o) const {
            if(&o == this) return true;

            SearchStringInputIter* _this = const_cast<SearchStringInputIter*>(this);
            SearchStringInputIter& _o    = const_cast<SearchStringInputIter&>(o);
            return _this->is_end() && _o.is_end();
        }
        bool operator!=(const SearchStringInputIter& o) const {
            return !this->operator==(o);
        }
};

#endif // _SEARCH_STRING_

