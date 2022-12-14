#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include <stddef.h>
#include <iterator>

namespace ft {
    template<class Category, class T, class Distance = ptrdiff_t,
            class Pointer = T *, class Reference = T &>
    struct iterator {
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        typedef Category iterator_category;
    };

    template<class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category iterator_category;
        typedef typename Iterator::value_type value_type;
        typedef typename Iterator::difference_type difference_type;
        typedef typename Iterator::pointer pointer;
        typedef typename Iterator::reference reference;
    };

    template<class T>
    struct iterator_traits<T *> {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template<class T>
    struct iterator_traits<const T *> {
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        typedef std::random_access_iterator_tag iterator_category;
    };

    template<class InputIterator>
    typename iterator_traits<InputIterator>::difference_type
    distance(InputIterator first, InputIterator last, std::input_iterator_tag){
        typename iterator_traits<InputIterator>::difference_type n = 0;

        while ( first != last ){
            ++first;
            ++n;
        }
        return n;
    }

    template<class RandomAccessIterator>
    typename iterator_traits<RandomAccessIterator>::difference_type
    distance(RandomAccessIterator first, RandomAccessIterator last, std::random_access_iterator_tag){
        return last - first;
    }

    template<class Iterator>
    typename iterator_traits<Iterator>::difference_type
    distance(Iterator first, Iterator last){
        return ft::distance(first, last, typename iterator_traits<Iterator>::iterator_category());
    }

    template<class It>
    class reverse_iterator {
    public:
        typedef It iterator_type;
        typedef typename ft::iterator_traits<It>::value_type value_type;
        typedef typename ft::iterator_traits<It>::iterator_category iterator_category;
        typedef typename ft::iterator_traits<It>::difference_type difference_type;
        typedef typename ft::iterator_traits<It>::pointer pointer;
        typedef typename ft::iterator_traits<It>::reference reference;

        reverse_iterator() : _current() {}

        explicit reverse_iterator(iterator_type it) : _current(it) {}

        template<class Iter>
        reverse_iterator(const reverse_iterator<Iter> &rev_it): _current(rev_it.base()) {}

        ~reverse_iterator() {}

        iterator_type base() const { return _current; }

        reference operator*() const {
            iterator_type tmp = _current;
            return *(--tmp);
        }

        pointer operator->() const { return pointer(&(operator*())); }

        template<class Iter>
        reverse_iterator &operator=(const reverse_iterator<Iter> &x) {
            _current = x.base();
            return *this;
        }

        template<class Iter>
        reverse_iterator operator[](difference_type n) const { return *(operator+(n)); }

        reverse_iterator &operator++() {
            _current--;
            return *this;
        }

        reverse_iterator &operator--() {
            _current++;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp = *this;
            --_current;
            return tmp;
        }

        reverse_iterator operator--(int) {
            reverse_iterator tmp = *this;
            ++_current;
            return tmp;
        }

        reverse_iterator operator+(difference_type n) const { return reverse_iterator(_current - n); }

        reverse_iterator operator-(difference_type n) const { return reverse_iterator(_current + n); }

        reverse_iterator &operator+=(difference_type n) const {
            _current -= n;
            return *this;
        }

        reverse_iterator &operator-=(difference_type n) const {
            _current += n;
            return *this;
        }

    private:
        iterator_type _current;
    };

    template<class Iterator1, class Iterator2>
    bool operator==(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return x.base() == y.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator!=(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return !(x == y);
    }

    template<class Iterator1, class Iterator2>
    bool operator<(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return y.base() < x.base();
    }

    template<class Iterator1, class Iterator2>
    bool operator<=(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return !(x > y);
    }

    template<class Iterator1, class Iterator2>
    bool operator>(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return y < x;
    }

    template<class Iterator1, class Iterator2>
    bool operator>=(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return !(x < y);
    }

    template<class Iterator>
    reverse_iterator<Iterator>
    operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> &rev_it) {
        return reverse_iterator<Iterator>(rev_it + n);
    }

    template<class Iterator1, class Iterator2>
    typename reverse_iterator<Iterator1>::difference_type
    operator-(const reverse_iterator<Iterator1> &x, const reverse_iterator<Iterator2> &y) {
        return typename reverse_iterator<Iterator1>::difference_type(y.base() - x.base());
    }

    template<class It>
    class random_access_iterator: public ft::iterator<std::bidirectional_iterator_tag, It> {
        public:
            typedef It                                                          iterator_type;
            typedef typename ft::iterator_traits<It>::value_type          value_type;
            typedef typename ft::iterator_traits<It>::pointer             pointer;
            typedef typename ft::iterator_traits<It>::reference           reference;
            typedef typename ft::iterator_traits<It>::difference_type     difference_type;
            typedef typename ft::iterator_traits<It>::iterator_category   iterator_category;


            random_access_iterator() : _current( iterator_type() ) {}
            explicit random_access_iterator(iterator_type const &x) : _current(x) {}

            template<class Iter>
            random_access_iterator(random_access_iterator<Iter> const &it) : _current(it.base()) {}


            ~random_access_iterator() {}

            template<class Iter>
            random_access_iterator &operator=(const random_access_iterator<Iter> &x) {
                _current = x.base();
                return *this;
            }

            random_access_iterator &operator++() { _current++; return *this; };
            random_access_iterator operator++(int) { random_access_iterator tmp = *this; ++(*this); return tmp; }
            random_access_iterator &operator--() { _current--; return *this; }
            random_access_iterator operator--(int) { random_access_iterator tmp = *this; --(*this); return tmp; }

            random_access_iterator &operator+=(difference_type n) {
                _current += n;
                return *this;
            }

            random_access_iterator &operator-=(difference_type n) {
                _current -= n;
                return *this;
            }

            random_access_iterator operator+(difference_type n) const { return random_access_iterator(_current + n); }

            random_access_iterator operator-(difference_type n) const { return random_access_iterator(_current - n); }

            const iterator_type &base() const { return _current; }

            pointer operator->() const { return _current; }

            reference operator[](difference_type n) const { return _current[n]; }

            reference operator*() const { return *_current; }
        private:
            iterator_type _current;
    };

    template<class U, class V>
    bool operator==(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return x.base() == y.base();
    }

    template<class U, class V>
    bool operator!=(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return !(x == y);
    }

    template<class U, class V>
    bool operator<(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return (x.base() < y.base());
    }

    template<class U, class V>
    bool operator<=(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return !(x > y);
    }

    template<class U, class V>
    bool operator>(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return y < x;
    }

    template<class U, class V>
    bool operator>=(const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return !(x < y);
    }

    template<class U>
    random_access_iterator<U>
    operator+(const typename random_access_iterator<U>::difference_type &x, const random_access_iterator<U> &y) {
        return random_access_iterator<U>(y.base() + x);
    }

    template<class U>
    typename random_access_iterator<U>::difference_type operator-(
            const random_access_iterator<U> &x, const random_access_iterator<U> &y) {
        return typename random_access_iterator<U>::difference_type(x.base() - y.base());
    }

    template<class U, class V>
    typename random_access_iterator<U>::difference_type operator-(
            const random_access_iterator<U> &x, const random_access_iterator<V> &y) {
        return typename random_access_iterator<U>::difference_type(x.base() - y.base());
    }
}

#endif