#ifndef STACK_HPP
#define STACK_HPP

# include "vector.hpp"

namespace ft {
    template <class T, class Container = ft::vector<T> >
    class stack {
    public:
        typedef T           value_type;
        typedef Container   container_type;
        typedef std::size_t size_type;

    public:
        explicit stack(const container_type &c = container_type()): elems(c) {}

        stack (const stack &other): elems(other.elems) {}

        stack &operator=(const stack &other) {
            if (&other == this)
                return *this;

            this->elems = other.elems;
            return *this;
        }

        ~stack() {}

        bool empty() const { return elems.empty(); }

        size_type size() const { return elems.size(); }

        value_type& top() { return elems.back(); }

        const value_type& top() const { return elems.back(); }

        void push (const value_type &val) { elems.push_back(val); }

        void pop() { elems.pop_back(); }

        template <class U, class Cntr>
        friend bool operator==(const stack<U, Cntr>& x, const stack<U, Cntr>& y);
        template <class U, class Cntr>
        friend bool operator<(const stack<U, Cntr>& x, const stack<U, Cntr>& y);

    private:
        container_type elems;
    };

    template <class U, class Cntr>
    bool operator==(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return x.elems == y.elems;
    }

    template <class U, class Cntr>
    bool operator!=(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return !(x == y);
    }

    template <class U, class Cntr>
    bool operator<(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return x.elems < y.elems;
    }
    template <class U, class Cntr>
    bool operator<=(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return !(x > y);
    }
    template <class U, class Cntr>
    bool operator>(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return y < x;
    }
    template <class U, class Cntr>
    bool operator>=(const ft::stack<U, Cntr>& x, const ft::stack<U, Cntr>& y) {
        return !(x < y);
    }
}  // namespace ft


#endif