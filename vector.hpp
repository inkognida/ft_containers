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
        // typedef ft::normal_iterator<pointer>					iterator;
        // typedef ft::normal_iterator<const_pointer>              const_iterator;
        // typedef ft::reverse_iterator<iterator>                  reverse_iterator;
        // typedef ft::reverse_iterator<const_iterator>            const_reverse_iterator;
    private:
        const static size_type  capacity_grow = 2;
        Allocator               alloc;
        size_type               capacity;
        size_type               size;
        T                       *items;

        void clear_all();
    public:
        // FUNCTIONS
        explicit vector(const allocator_type& a = allocator_type()): alloc(a), capacity(0), size(0), items(NULL) {};
        explicit vector(const Allocator& a): alloc(a.get_allocator()), capacity(a.capacity()), size(a.size()), items(NULL) {
            if (capacity != 0){
                items = alloc.allocate(capacity);
                for (size_type i = 0; i < size; i++) 
                    alloc.construct(&items[i], a.items[i])
            }
        };
        explicit vector(size_type n, const value_type &val = value_type(), const allocator_type &a = allocator_type() ) {
				alloc = a;
				items = alloc.allocate( n );
				size = n;
				capacity = n;

				for ( size_type i = 0 ; i < n ; i++ ){
					alloc.construct( &items[i], val );
				}
			}
        vector& operator=( const vector& other ) {
            if (&other == this) 
                return *this;
            this->clear();
            this->reserve(x.capacity());
            for (size_type i = 0; i < other.size(); i++)
                alloc.construct(&items[i], other[i]);
            size = other.size();
            return *this;
        }

        template< class InputIterator >
        vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(), 
                typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL ) :
				m_alloc( alloc ),
				m_items( NULL )
        {
            difference_type _size = ft::distance(first, last);
            
            if (size != 0) {
                items = alloc.allocate(_size);
                for (size_type i = 0; first != last; *first++; i++)
                    alloc.construct(&items[i], *first)
            }
            size = _size;
            capacity = _size;
        }
        
        ~vector() {
            if (items != NULL) {
                for (size_type i = 0; i < size; i++)
                    alloc.destroy(&items[i]);
            }
            alloc.deallocate(items, capacity);
        }
        size_type size() const { return size; }
        size_type max_size() const { 
            return return (std::min((size_type) std::numeric_limits<difference_type>::max(), std::numeric_limits<size_type>::max() / sizeof(value_type))); 
        }
        void resize (size_type n, value_type val = value_type()) {
            if (n < size) {
                for (size_type i = n; i < size; i++)
                    alloc.destroy(&items[i]);
                size = n;
            } else {
                if (n > capacity)
                    reserve(n);
                for (size_type i = size; i < n; i++)
                    alloc.construct(&items[i], val);
                size = n;
            }
        }
        size_type capacity() const { return capacity; }
        bool empty() const { return size == 0; }
        void reserve (size_type n) {
            if (n > this->max_size())
                throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size");
            if (n > capacity) {
                T *tmp = alloc.allocate(n);

                if (items != NULL) {
                    for (size_type i = 0; i < n; i++) 
                        alloc.construct(&tmp[i], items[i]);
                    alloc.deallocate(items, capacity);
                }
                items = tmp;
                capacity = n;
            }
        }
        
        
        
        
        

        
        
        
        
        
    
    };
}

#endif 