/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hardella <hardella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:34:09 by hardella          #+#    #+#             */
/*   Updated: 2022/11/24 21:59:01 by hardella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
#define VECTOR_HPP

# include <memory>
# include <iterator>
# include <limits>
# include <stdexcept>
# include <stddef.h>
# include <iostream>
# include <string>
# include <sstream>
# include <memory>
# include <cstddef>

namespace ft {
    template<class T, class Allocator = std::allocator<T> >
    class vector
    {
    public:
        typedef T                                   value_type;
        typedef Allocator                           allocator_type;
        typedef std::size_t                         size_type;
        typedef std::ptrdiff_t                      difference_type;
        typedef typename Allocator::reference       reference;
        typedef typename Allocator::const_reference const_reference;
        typedef typename Allocator::pointer         pointer;
        typedef typename Allocator::const_pointer   const_pointer;
        
        // ITERATORS 
        typedef ft::random_access_iterator<pointer>					    iterator;
        typedef ft::random_access_iterator<const_pointer>               const_iterator;
        typedef ft::reverse_iterator<iterator>                          reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;

    private:
        const static size_type  capacity_grow = 2;
        Allocator               m_alloc;
        size_type               m_capacity;
        size_type               m_size;
        T                       *m_items;

        void throw_range_exception(size_type n) const {
            std::stringstream s;
            s << "n (which is " << n << ") is bigger than" << m_size;
        }

        size_type get_alloc_size(size_type n){
            if ( (m_size + n) <= m_capacity ){
                return m_capacity;
            } else if ( (m_capacity + n) > (m_capacity * GROWTH_FACTOR) ){
                return m_capacity + n;
            } else {
                return m_capacity * GROWTH_FACTOR;
            }
        }

    public:
        // FUNCTIONS
        explicit vector(const allocator_type &a = allocator_type()): m_alloc(a), m_capacity(0), m_size(0), m_items(NULL) {}

        explicit vector(const vector &a): m_alloc(a.get_allocator()), m_capacity(a.m_capacity()), m_size(a.m_size()), m_items(NULL) {
            if (m_capacity != 0){
                m_items = m_alloc.allocate(m_capacity);
                for (size_type i = 0; i < m_size; i++)
                    m_alloc.construct(&m_items[i], a.m_items[i]);
            }
        };

        explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &a = allocator_type() ) {
				m_alloc = a;
				m_items = m_alloc.allocate( n );
				m_size = n;
				m_capacity = n;

				for ( size_type i = 0 ; i < n ; i++ ){
					m_alloc.construct( &m_items[i], val );
				}
			}

        vector& operator=( const vector& other ) {
            if (&other == this)
                return *this;
            this->clear();
            this->reserve(other.m_capacity());
            for (size_type i = 0; i < other.m_size(); i++)
                m_alloc.construct(&m_items[i], other[i]);
            m_size = other.m_size();
            return *this;
        }

        template< class InputIterator >
        vector(InputIterator first, InputIterator last, const allocator_type &m_alloc = allocator_type(),
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL ) :
				m_alloc( m_alloc ),
				m_items( NULL )
        {
            difference_type _size = ft::distance(first, last);

            if (m_size != 0) {
                m_items = m_alloc.allocate(_size);
                for (size_type i = 0; first != last; *first++, i++) {
                    m_alloc.construct(&m_items[i], *first);
                }
            }
            m_size = _size;
            m_capacity = _size;
        }

        ~vector() {
            if (m_items != NULL) {
                for (size_type i = 0; i < m_size; i++)
                    m_alloc.destroy(&m_items[i]);
            }
            m_alloc.deallocate(m_items, m_capacity);
        }

        size_type size() const { return m_size; }

        size_type max_size() const {
            return (std::min((size_type) std::numeric_limits<difference_type>::max(),
                             std::numeric_limits<size_type>::max() / sizeof(value_type)));
        }

        size_type capacity() const { return m_capacity; }


        void resize (size_type n, value_type val = value_type()) {
            if (n < m_size) {
                for (size_type i = n; i < m_size; i++)
                    m_alloc.destroy(&m_items[i]);
                m_size = n;
            } else {
                if (n > m_capacity)
                    reserve(n);
                for (size_type i = m_size; i < n; i++)
                    m_alloc.construct(&m_items[i], val);
                m_size = n;
            }
        }

        bool empty() const { return m_size == 0; }

        void reserve (size_type n) {
            if (n > this->max_size())
                throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported m_size");
            if (n > m_capacity) {
                T *tmp = m_alloc.allocate(n);

                if (m_items != NULL) {
                    for (size_type i = 0; i < n; i++)
                        m_alloc.construct(&tmp[i], m_items[i]);
                    m_alloc.deallocate(m_items, m_capacity);
                }
                m_items = tmp;
                m_capacity = n;
            }
        }

        reference   operator[](size_type n) { return m_items[n]; }

        const_reference operator[](size_type n) const { return m_items[n]; }

        reference at(size_type n) {
            if (n >= m_size)
                throw_range_exception(n);
            return m_items[n];
        }

        const_reference at (size_type n) const {
            if (n >= m_size)
                throw_range_exception(n);
            return m_items[n];
        }

        reference front() { return m_items[0]; }

        const_reference front() const { return m_items[0]; }

        reference back() {
            size_type i = (m_size == 0) ? 0 : m_size - 1;
            return m_items[i];
        }

        const_reference back() const {
            size_type i = (m_size == 0) ? 0 : m_size - 1;
            return m_items[i];
        }

        T* 			data() { return m_items; }

        const T*	data() const { return m_items; }

        void clear() {
            for (size_type i = 0; i < m_size; ++i)
                    m_alloc.destroy(m_items + i);
            m_size = 0;
        }

        iterator insert(iterator position, const value_type &val) {
            size_type index = position - begin();

            if (m_size == m_capacity){
                size_type capacity = m_capacity;

                if (capacity == 0)
                    capacity = 1;
                else
                    capacity *= capacity_grow;
                reserve(capacity);
            }

            for (size_type i = m_size; i != index; i--)
                m_items[index] = m_items[index - 1];
            m_alloc.construct( &m_items[index], val);
            m_size++;
            return iterator (&m_items[index]);
        }

        void insert(iterator position, size_type n, const value_type &val) {
            iterator it = position;
            difference_type distance = ft::distance(begin(), position);
            size_type alloc_size = get_alloc_size(n);

            reserve(alloc_size);

            it = begin() + distance;
            for (; n != 0; n--)
                it = this->insert(it, val);

        }

        template<class InputIterator>
        void insert (iterator position, InputIterator first, InputIterator last) {
            difference_type distance = ft::distance(begin(), position);
            difference_type n = ft::distance(first, last);
            iterator it;
            size_type alloc_size = get_alloc_size(n);

            reserve(alloc_size);
            it = begin() + distance;

            for (; first != last; first++, it++)
                it = this->insert(it, *first);
        }

        template <class InputIterator>
        void assign (InputIterator first, InputIterator last) {
            clear();
            for (InputIterator it = first; it != last; it++)
                push_back(*it);
        }

        void assign(size_type n, const value_type &val) {
            clear();
            resize(n, val);
        }

        void push_back(const value_type &val) {
            if (m_size == m_capacity) {
                size_type alloc = m_capacity;

                if (m_capacity == 0)
                    alloc = 1;
                else
                    alloc *= capacity_grow;
                value_type *tmp = m_alloc.allocate(alloc);

                for (size_type i = 0; i < m_size; i++)
                    m_alloc.construct(&tmp[i], m_items[i]);
                m_alloc.deallocate(m_items, m_capacity);
                m_items = tmp;
                m_capacity = alloc;
            }
            m_items[m_size++] = val;
        }

        void pop_back() {
            if (m_size != 0)
                m_alloc.destroy(&m_items[--m_size]);
        }

        iterator erase (iterator position) {
            difference_type distance = position - begin();

            if (m_size != 0) {
                for (size_type i = distance; i < m_size; ++index) {
                    m_alloc.destroy( &m_items[index] );
                    if (i != m_size - 1)
                        m_alloc.construct( &m_items[i], m_items[i + 1]);
                }
                m_size--;
            }

            return iterator (&m_items[distance]);
        }

        iterator erase (iterator first, iterator last) {
            if (first != last) {
                iterator ptr = std::copy(last, end(), first);

                while (end() != ptr)
                    pop_back();
            }

            return first;
        }

        iterator begin() {
            return iterator( m_items );
        }

        const_iterator begin() const {
            return const_iterator( m_items );
        }

        reverse_iterator rbegin() {
            return reverse_iterator( end() );
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator( end() );
        }

        iterator end() { return iterator ( m_items + m_size ); }

        const_iterator end() const { return iterator ( m_items + m_size ); }

        reverse_iterator rend() {
            return reverse_iterator( begin() );
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator( begin() );
        }

        allocator_type get_allocator() const { return m_alloc; }

        void swap(vector &x) {
            Allocator &tmp_alloc = x.m_alloc;
            size_type tmp_capacity = x.m_capacity;
            size_type tmp_size = x.m_size;
            value_type *tmp_items = x.m_items;

            x.m_alloc = this->m_alloc;
            x.m_size = this->m_size;
            x.m_capacity = this->m_capacity;
            x.m_items = this->m_items;

            this->m_size = tmp_size;
            this->m_alloc = tmp_alloc;
            this->m_capacity = tmp_capacity;
            this->m_items = tmp_items;
        }
    };

    template <class T, class Alloc>
    void swap(vector<T, Alloc> &x, vector <T, Alloc> &y) {
        x.swap(y);
    }

    template<class T, class Alloc>
    bool operator==(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return x.size() == y.size() && ft::equal(x.begin(), x.end(),y.begin(), y.end());
    }

    template<class T, class Alloc>
    bool operator!=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return !(x == y);
    }

    template<class T, class Alloc>
    bool operator<(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template<class T, class Alloc>
    bool operator<=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return !( y < x );
    }

    template<class T, class Alloc>
    bool operator>(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return y < x;
    }

    template<class T, class Alloc>
    bool operator>=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
        return !( x < y );
    }
}

#endif 