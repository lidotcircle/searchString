#ifndef _OPTIONAL_HPP_
#define _OPTIONAL_HPP_

#include <exception>
#include <stdexcept>


template<typename T>
union Optional
{
    using value_type = T;

private:
    struct
    {
        bool is_value;
        value_type i;
    } m;

    struct
    {
        bool is_value;
        char a[sizeof(value_type)];
    } n;


public:
    Optional() : n{false} {}
    Optional(value_type value) : m{true, std::move(value)} {}

    Optional(Optional &&o)
    {
        if (o.m.is_value)
        {
            this->m.is_value = true;
            this->m.i = std::move(o.m.i);
        }
        else
        {
            this->m.is_value = false;
        }
    }
    Optional(const Optional &o)
    {
        if (o.m.is_value)
        {
            this->m.is_value = true;
            this->m.i = o.m.i;
        }
        else
        {
            this->m.is_value = false;
        }
    }

    Optional &operator=(Optional &&o)
    {
        ~Optional();
        new (this) Optional(std::move(o));
        return *this;
    }

    Optional &operator=(const Optional &o)
    {
        ~Optional();
        new (this) Optional(o);
        return *this;
    }

    ~Optional()
    {
        if (this->m.is_value)
        {
            this->m.i.~value_type();
        }
    }

    value_type &get()
    {
        if (!this->m.is_value)
        {
            throw std::runtime_error("access inactive member in union");
        }

        return this->m.i;
    }

    operator bool() const
    {
        return this->m.is_value;
    }
};

#endif // _OPTIONAL_HPP_
