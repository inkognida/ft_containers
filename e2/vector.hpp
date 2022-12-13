//#ifndef VECTOR_HPP
//#define VECTOR_HPP
//
//# include <memory>
//# include <iterator>
//# include <limits>
//# include <stdexcept>
//# include <stddef.h>
//# include <iostream>
//# include <string>
//# include <sstream>
//# include "algorithm/algorithm.hpp"
//# include "traits/traits.hpp"
//# include "traits/iterator.hpp"
//
//namespace ft {
//	/**
//	 * The documentation of this vector implementation comes frome
//	 * https://www.cplusplus.com/reference/vector/vector/?kw=vector
//	 *
//	 */
//	template<class T, class Allocator = std::allocator<T> >
//	class vector {
//		public:
//			typedef std::size_t										size_type;
//			typedef std::ptrdiff_t									difference_type;
//			typedef T												value_type;
//			typedef Allocator										allocator_type;
//			typedef typename Allocator::reference					reference;
//			typedef typename Allocator::const_reference				const_reference;
//			typedef typename Allocator::pointer						pointer;
//			typedef typename Allocator::const_pointer				const_pointer;
//			typedef ft::normal_iterator<pointer>					iterator;
//            typedef ft::normal_iterator<const_pointer>              const_iterator;
//            typedef ft::reverse_iterator<iterator>                  reverse_iterator;
//            typedef ft::reverse_iterator<const_iterator>            const_reverse_iterator;
//		private:
//			const static size_type	GROWTH_FACTOR = 2;
//			Allocator				m_alloc;
//			size_type				m_capacity;
//			size_type				m_size;
//			T						*m_items;
//		public:
//			/**
//			 * (1) empty container constructor (default constructor)
//			 *
//			 * Constructs an empty container, with no elements.
//			 *
//			 */
//			explicit vector( const allocator_type &alloc = allocator_type() ) :
//				m_alloc( alloc ),
//				m_capacity( 0 ),
//				m_size( 0 ),
//				m_items( NULL ) {}
//
//			/**
//			 * (2) fill constructor
//			 *
//			 * Constructs a container with n elements.
//			 * Each element is a copy of val.
//			 *
//			 */
//			explicit vector( size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type() ) {
//				m_alloc = alloc;
//				m_items = m_alloc.allocate( n );
//				m_size = n;
//				m_capacity = n;
//
//				for ( size_type index = 0 ; index < n ; index++ ){
//					m_alloc.construct( &m_items[index], val );
//				}
//			}
//
//			/**
//			 *
//			 * Assign content
//			 *
//			 * Assigns new contents to the container, replacing its current contents,
//			 * and modifying its size accordingly.
//			 *
//			 */
//			vector &operator=(const vector &x){
//				if ( &x == this ){
//					return *this;
//				}
//				this->clear();
//				this->reserve(x.capacity());
//				for ( size_type index = 0 ; index < x.size() ; index++ ){
//					m_alloc.construct( &m_items[index], x[index] );
//				}
//				m_size = x.size();
//				return *this;
//			}
//
//			/**
//			 * (3) range constructor
//			 *
//			 * Constructs a container with as many elements as the range [first,last),
//			 * with each element constructed from its corresponding element in that range, in the same order.
//			 *
//			 */
//			template<class InputIterator>
//			vector( InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
//                    typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL ) :
//				m_alloc( alloc ),
//				m_items( NULL )
//			{
//				difference_type size = ft::distance(first, last);
//
//				if ( size != 0 ){
//					m_items = m_alloc.allocate( size );
//
//					for ( size_type index = 0 ; first != last ; first++, index++ ){
//						m_alloc.construct( &m_items[index], *first );
//					}
//				}
//				m_size = size;
//				m_capacity = size;
//			}
//
//			/**
//			 * (4) copy constructor
//			 *
//			 * Constructs a container with a copy of each of the elements in x, in the same order.
//			 *
//			 */
//			explicit vector( const vector &other ) :
//				m_alloc( other.get_allocator() ),
//				m_capacity( other.capacity() ),
//				m_size( other.size() ),
//				m_items( NULL )
//			{
//				if ( m_capacity != 0 ){
//					m_items = m_alloc.allocate( m_capacity );
//
//					for ( size_type index = 0 ; index < m_size ; index++ ){
//						m_alloc.construct( &m_items[index], other.m_items[index] );
//					}
//				}
//			}
//
//			/**
//			 * Vector destructor
//			 *
//			 * Destroys the container object.
//			 *
//			 * This destroys all container elements, and deallocates all
//			 * the storage capacity allocated by the vector using its allocator.
//			 *
//			 */
//			~vector() {
//				if ( m_items != NULL ){
//					for ( size_type index = 0 ; index < m_size ; index++ ){
//						m_alloc.destroy( &m_items[index] );
//					}
//					m_alloc.deallocate( m_items, m_capacity );
//				}
//			}
//
//			/**
//			 * Return size
//			 *
//			 * Returns the number of elements in the vector.
//			 * This is the number of actual objects held in the vector,
//			 * which is not necessarily equal to its storage capacity.
//			 *
//			 */
//			size_type size() const {
//				return m_size;
//			}
//
//			/**
//			 * Return capacity
//			 *
//			 * Returns the size of the storage space currently allocated
//			 * for the vector, expressed in terms of elements.
//			 *
//			 * This capacity is not necessarily equal to the vector size.
//			 * It can be equal or greater, with the extra space allowing
//			 * to accommodate for growth without the need to reallocate on each insertion.
//			 *
//			 * Notice that this capacity does not suppose a limit on the size of the vector.
//			 * When this capacity is exhausted and more is needed,
//			 * it is automatically expanded by the container (reallocating it storage space).
//			 * The theoretical limit on the size of a vector is given by member max_size.
//			 *
//			 * The capacity of a vector can be explicitly altered by calling member vector::reserve.
//			 */
//			size_type capacity() const {
//				return m_capacity;
//			}
//
//			/**
//			 * Return maximum size
//			 *
//			 * Returns the maximum number of elements that the vector can hold.
//			 *
//			 * This is the maximum potential size the container can reach
//			 * due to known system or library implementation limitations,
//			 * but the container is by no means guaranteed to be able to reach
//			 * that size: it can still fail to allocate storage at any point before that size is reached.
//			 *
//			 */
//			size_type max_size() const {
//				return (std::min((size_type) std::numeric_limits<difference_type>::max(),
//						std::numeric_limits<size_type>::max() / sizeof(value_type)) );
//			}
//
//			/**
//			 * Test whether vector is empty
//			 *
//			 * Returns whether the vector is empty (i.e. whether its size is 0).
//			 * This function does not modify the container in any way.
//			 * To clear the content of a vector, see vector::clear.
//			 *
//			 */
//			bool empty() const {
//				return m_size == 0;
//			}
//
//			/**
//			 * Access element
//			 *
//			 * Returns a reference to the element at position n in the vector container.
//			 *
//			 * A similar member function, vector::at, has the same behavior as this
//			 * operator function, except that vector::at is bound-checked and signals
//			 * if the requested position is out of range by throwing an out_of_range exception.
//			 *
//			 * Portable programs should never call this function with an argument n
//			 * that is out of range, since this causes undefined behavior.
//			 *
//			 */
//			reference operator[](size_type n) {
//				return ( m_items[n] );
//			}
//
//			const_reference operator[](size_type n) const {
//				return ( m_items[n] );
//			}
//
//			/**
//			 * Access first element
//			 *
//			 * Returns a reference to the first element in the vector.
//			 *
//			 * Unlike member vector::begin, which returns an iterator to this same element,
//			 * this function returns a direct reference.
//			 *
//			 * Calling this function on an empty container causes undefined behavior.
//			 *
//			 */
//			reference front(){
//				return m_items[0];
//			}
//
//			const_reference front() const {
//				return m_items[0];
//			}
//
//			/**
//			 * Access last element
//			 *
//			 * Returns a reference to the last element in the vector.
//			 *
//			 * Unlike member vector::end, which returns an iterator just past this element,
//			 * this function returns a direct reference.
//			 *
//			 * Calling this function on an empty container causes undefined behavior.
//			 *
//			 */
//			reference back(){
//				size_type index = (m_size == 0) ? 0 : m_size - 1;
//
//				return m_items[index];
//			}
//
//			const_reference back() const {
//				size_type index = (m_size == 0) ? 0 : m_size - 1;
//
//				return m_items[index];
//			}
//
//			/**
//			 * Access element
//			 *
//			 * Returns a reference to the element at position n in the vector.
//			 *
//			 * The function automatically checks whether n is within the bounds
//			 * of valid elements in the vector, throwing an out_of_range exception
//			 * if it is not (i.e., if n is greater than, or equal to, its size).
//			 * This is in contrast with member operator[], that does not check against bounds.
//			 *
//			 */
//			reference at(size_type n){
//				if ( n >= m_size ){
//					throw_range_exception(n);
//				}
//				return ( m_items[ n ] );
//			}
//
//			const_reference at(size_type n) const {
//				if ( n >= m_size ){
//					throw_range_exception(n);
//				}
//				return ( m_items[ n ] );
//			}
//
//
//			/**
//			 * Request a change in capacity
//			 *
//			 * Requests that the vector capacity be at least enough to contain n elements.
//			 *
//			 * If n is greater than the current vector capacity,
//			 * the function causes the container to reallocate its storage increasing its capacity to n (or greater).
//			 *
//			 * In all other cases, the function call does not cause a reallocation and the vector capacity is not affected.
//			 *
//			 * This function has no effect on the vector size and cannot alter its elements.
//			 *
//			 */
//			void reserve( size_type n ) {
//				if ( n > this->max_size() ){
//					throw std::length_error("allocator<T>::allocate(size_t n) 'n' exceeds maximum supported size");
//				}
//				if ( n > m_capacity ){
//					T *tmp = m_alloc.allocate( n );
//
//					if ( m_items != NULL ){
//						for ( size_type index = 0 ; index < m_size ; index++ ){
//							m_alloc.construct( &tmp[index], m_items[index] );
//						}
//						m_alloc.deallocate( m_items, m_capacity );
//					}
//					m_items = tmp;
//					m_capacity = n;
//				}
//			}
//
//			/**
//			 * Change size
//			 *
//			 * Resizes the container so that it contains n elements.
//			 *
//			 * If n is smaller than the current container size, the content is reduced to its first n elements,
//			 * removing those beyond (and destroying them).
//			 *
//			 * If n is greater than the current container size,
//			 * the content is expanded by inserting at the end as many elements as needed to reach a size of n.
//			 * If val is specified, the new elements are initialized as copies of val, otherwise, they are value-initialized.
//			 *
//			 * If n is also greater than the current container capacity, an automatic reallocation of the allocated storage space takes place.
//			 *
//			 * Notice that this function changes the actual content of the container by inserting or erasing elements from it.
//			 *
//			 */
//			void resize( size_type n, value_type val = value_type() ){
//				if ( n < m_size ){
//					for ( size_type index = n ; index < m_size ; index++ ){
//						m_alloc.destroy( &m_items[index] );
//					}
//					m_size = n;
//				} else {
//					if ( n > m_capacity ){
//						reserve(n);
//					}
//					for ( size_type index = m_size ; index < n ; index++ ){
//						m_alloc.construct( &m_items[index], val );
//					}
//					m_size = n;
//				}
//			}
//
//			/**
//			 * Add element at the end
//			 *
//			 * Adds a new element at the end of the vector, after its current last element.
//			 * The content of val is copied (or moved) to the new element.
//			 *
//			 * This effectively increases the container size by one, which causes an automatic
//			 * reallocation of the allocated storage space if -and only if- the new vector size
//			 * surpasses the current vector capacity.
//			 *
//			 */
//			void push_back( const value_type &val ){
//				if ( m_size == m_capacity ){
//					size_type alloc_size;
//
//					if ( m_size == 0 )
//						alloc_size = 1;
//					else
//						alloc_size = m_capacity * GROWTH_FACTOR;
//					value_type *tmp = m_alloc.allocate( alloc_size );
//
//					for ( size_type index = 0 ; index < m_size ; index++ ){
//						m_alloc.construct( &tmp[index], m_items[index] );
//					}
//					m_alloc.deallocate( m_items, m_capacity );
//					m_items = tmp;
//					m_capacity = alloc_size;
//				}
//				m_items[m_size++] = val;
//			}
//
//			/**
//			 * Delete last element
//			 *
//			 * Removes the last element in the vector, effectively reducing the container size by one.
//			 * This destroys the removed element.
//			 *
//			 */
//			void pop_back() {
//				if ( m_size != 0 ){
//					m_alloc.destroy( &m_items[--m_size] );
//				}
//			}
//
//			/**
//			 * Assign vector content ( fill )
//			 *
//			 * Assigns new contents to the vector, replacing its current contents,
//			 * and modifying its size accordingly.
//			 *
//			 */
//			void assign( size_type n, value_type const &val ){
//				clear();
//				resize(n, val);
//			}
//
//			/**
//			 * Assign vector content ( range )
//			 *
//			 * Assigns new contents to the vector, replacing its current contents,
//			 * and modifying its size accordingly.
//			 *
//			 */
//			template<class InputIterator>
//			void assign( InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL){
//				clear();
//				for ( InputIterator it = first ; it != last ; it++ ){
//					push_back( *it );
//				}
//
//
//			/**
//			 * Erase elements (position)
//			 *
//			 * Removes from the vector either a single element (position) or a range of elements ([first,last)).
//			 *
//			 * This effectively reduces the container size by the number of elements removed, which are destroyed.
//			 *
//			 * Because vectors use an array as their underlying storage, erasing elements in positions other
//			 * than the vector end causes the container to relocate all the elements after the segment erased to
//			 * their new positions. This is generally an inefficient operation compared to the one performed for
//			 * the same operation by other kinds of sequence containers (such as list or forward_list).
//			 *
//			 */
//			iterator erase(iterator position){
//				difference_type distance = ft::distance(begin(), position);
//
//				if ( m_size != 0 ){
//					size_type index = distance;
//					for ( ; index < m_size - 1 ; index++ ){
//						m_items[index] = m_items[index + 1];
//					}
//					m_alloc.destroy( &m_items[index] );
//					m_size--;
//				}
//				return ( iterator( &m_items[distance] ) );
//			}
//
//			/**
//			 * Erase elements (range)
//			 *
//			 */
//			iterator erase(iterator first, iterator last){
//				/**
//				 * REFACTOR: Refactor this function, because it is unreadable.
//				 *
//				 */
//				size_type begin_index = ft::distance(begin(), first);
//				size_type end_index = ft::distance(begin(), last);
//				size_type offset = (m_size < end_index) ? 0 : m_size - end_index;
//				size_type index;
//
//				m_size = m_size - (end_index - begin_index);
//				for ( index = 0 ; index < offset ; index++ ){
//					m_alloc.destroy( &m_items[begin_index + index] );
//					m_items[begin_index + index] = m_items[end_index + index];
//				}
//				for ( ; index < m_size ; index++ ){
//					m_alloc.destroy( &m_items[begin_index + index] );
//				}
//				return ( iterator( &m_items[begin_index] ) );
//			}
//
//			/**
//			 * Insert elements ( single element )
//			 *
//			 * The vector is extended by inserting new elements before the element
//			 * at the specified position, effectively increasing the container size
//			 * by the number of elements inserted.
//			 *
//			 * This causes an automatic reallocation of the allocated storage space
//			 * if -and only if- the new vector size surpasses the current vector capacity.
//			 *
//			 *
//			 * Because vectors use an array as their underlying storage, inserting elements
//			 * in positions other than the vector end causes the container to relocate all
//			 * the elements that were after position to their new positions. This is generally
//			 * an inefficient operation compared to the one performed for the same operation
//			 * by other kinds of sequence containers (such as list or forward_list).
//			 *
//			 */
//			iterator insert(iterator position, value_type const &val){
//				size_type pos_index = position - begin();
//
//				if ( m_size == m_capacity ){
//					size_type capacity = m_capacity;
//
//					if ( capacity == 0 )
//						capacity = 1;
//					else
//						capacity *= GROWTH_FACTOR;
//					reserve( capacity );
//				}
//
//				for ( size_type index = m_size ; index != pos_index ; index-- ){
//					m_items[index] = m_items[index - 1];
//				}
//				m_alloc.construct( &m_items[pos_index], val );
//				m_size++;
//				return iterator( &m_items[pos_index] );
//			}
//
//			/**
//			 * Insert elements ( fill )
//			 *
//			 */
//			void insert(iterator position, size_type n, value_type const &val ){
//				/**
//				 * FIXME: If vector has no previous allocation, position will point to a
//				 * non-existent pointer, that will cause a segmentation fault.
//				 *
//				 * The position pointer will be invalidated.
//				 *
//				 * FIXED: If vector has no prevous allocation, it means that pointer
//				 * pointed by begin() and end() will point to a pointer that will
//				 * be invalidated by call to reserve().
//				 *
//				 * We must calculated the distance, instead of using iterator directly.
//				 * In all cases, position pointer will be invalidated by further allocation.
//				 *
//				 */
//				iterator it = position;
//				difference_type distance = ft::distance(begin(), position);
//				size_type alloc_size = get_alloc_size(n);
//
//				reserve( alloc_size );
//
//				it = begin() + distance;
//
//				for ( ; n != 0 ; n-- ){
//					it = this->insert( it, val );
//				}
//			}
//
//			template<class InputIterator>
//			void insert(iterator position, InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL ){
//				difference_type distance = ft::distance(begin(), position);
//				difference_type n = ft::distance(first, last);
//				iterator it;
//				size_type alloc_size = get_alloc_size(n);
//
//				reserve( alloc_size );
//
//				it = begin() + distance;
//				for ( ; first != last ; first++, it++ ){
//					it = this->insert( it, *first );
//				}
//			}
//
//			/**
//			 *
//			 * Swap content
//			 *
//			 * Exchanges the content of the container by the content of x,
//			 * which is another vector object of the same type. Sizes may differ.
//			 *
//			 * After the call to this member function, the elements in this container
//			 * are those which were in x before the call, and the elements of x are those
//			 * which were in this. All iterators, references and pointers remain valid for the swapped objects.
//			 *
//			 * Notice that a non-member function exists with the same name,
//			 * swap, overloading that algorithm with an optimization that behaves like this member function.
//			 *
//			 */
//			void swap(vector &x){
//				Allocator	&tmp_alloc = x.m_alloc;
//				size_type	tmp_size = x.m_size;
//				size_type	tmp_capacity = x.m_capacity;
//				value_type	*tmp_items = x.m_items;
//
//
//				x.m_alloc = this->m_alloc;
//				x.m_size = this->m_size;
//				x.m_capacity = this->m_capacity;
//				x.m_items = this->m_items;
//
//
//				this->m_alloc = tmp_alloc;
//				this->m_size = tmp_size;
//				this->m_capacity = tmp_capacity;
//				this->m_items = tmp_items;
//			}
//
//
//			/**
//			 * Clear content
//			 *
//			 * Removes all elements from the vector (which are destroyed),
//			 * leaving the container with a size of 0.
//			 *
//			 * A reallocation is not guaranteed to happen, and the vector
//			 * capacity is not guaranteed to change due to calling this function.
//			 *
//			 */
//			void clear() {
//				// Refactor with pop_back
//                for (size_type i = 0; i < m_size; i++)
//                    m_alloc.destroy(m_items + i);
//                m_size = 0;
//			}
//
//			/**
//			 * Return iterator to beginning
//			 *
//			 * Returns an iterator pointing to the first element in the vector.
//			 * Notice that, unlike member vector::front, which returns a reference
//			 * to the first element, this function returns a random access iterator pointing to it.
//			 * If the container is empty, the returned iterator value shall not be dereferenced.
//			 *
//			 */
//
//			iterator begin() {
//				return iterator( m_items );
//			}
//
//			const_iterator begin() const {
//				return const_iterator( m_items );
//			}
//
//			reverse_iterator rbegin() {
//				return reverse_iterator( end() );
//			}
//
//			const_reverse_iterator rbegin() const {
//				return const_reverse_iterator( end() );
//			}
//
//			iterator end() { return iterator ( m_items + m_size ); }
//
//			const_iterator end() const { return iterator ( m_items + m_size ); }
//
//			reverse_iterator rend() {
//				return reverse_iterator( begin() );
//			}
//
//			const_reverse_iterator rend() const {
//				return const_reverse_iterator( begin() );
//			}
//
//			/**
//			 * Get allocator
//			 *
//			 * Returns a copy of the allocator object associated with the vector.
//			 *
//			 */
//			allocator_type get_allocator() const {
//				return m_alloc;
//			}
//
//		private:
//			void throw_range_exception(size_type n) const {
//				std::stringstream s;
//
//				s << "n (which is " << n << ") >= this->size() (which is " << m_size << ")";
//				throw std::out_of_range(s.str());
//			}
//
//			/**
//			 * Get allocation size
//			 *
//			 * Get the allocation size, if `n` plus the current size is
//			 * less than the current capacity, get_alloc_size returns
//			 * capacity.
//			 *
//			 * If the current capacity + `n` is greater than the capacity
//			 * multiplied by the growth factor, then current capacity + `n`
//			 * is returned.
//			 *
//			 * Otherwise, get_alloc_size returns the current capacity multiplied
//			 * by the growth factor.
//			 *
//			 */
//			size_type get_alloc_size(size_type n){
//				if ( (m_size + n) <= m_capacity ){
//					return m_capacity;
//				} else if ( (m_capacity + n) > (m_capacity * GROWTH_FACTOR) ){
//					return m_capacity + n;
//				} else {
//					return m_capacity * GROWTH_FACTOR;
//				}
//			}
//	};
//
//	template<class T, class Alloc>
//	void swap(vector<T, Alloc> &x, vector<T, Alloc> &y){
//		x.swap(y);
//	}
//
//	template<class T, class Alloc>
//	bool operator==(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return x.size() == y.size() && ft::equal(x.begin(), x.end(),y.begin(), y.end());
//	}
//
//	template<class T, class Alloc>
//	bool operator!=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return !(x == y);
//	}
//
//	template<class T, class Alloc>
//	bool operator<(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
//	}
//
//	template<class T, class Alloc>
//	bool operator<=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return !( y < x );
//	}
//
//	template<class T, class Alloc>
//	bool operator>(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return y < x;
//	}
//
//	template<class T, class Alloc>
//	bool operator>=(const vector<T, Alloc> &x, const vector<T, Alloc> &y){
//		return !( x < y );
//	}
//}
//
//
//#endif
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

# include "algorithm/algorithm.hpp"
# include "traits/traits.hpp"
# include "traits/iterator.hpp"

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
        typedef ft::normal_iterator<pointer>					    iterator;
        typedef ft::normal_iterator<const_pointer>               const_iterator;
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
            } else if ( (m_capacity + n) > (m_capacity * capacity_grow) ){
                return m_capacity + n;
            } else {
                return m_capacity * capacity_grow;
            }
        }

    public:
        // FUNCTIONS
        explicit vector(const allocator_type &a = allocator_type()): m_alloc(a), m_capacity(0), m_size(0), m_items(NULL) {}

        explicit vector(const vector &a): m_alloc(a.get_allocator()), m_capacity(a.capacity()), m_size(a.size()), m_items(NULL) {
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
            this->reserve(other.capacity());
            for (size_type i = 0; i < other.size(); i++)
                m_alloc.construct(&m_items[i], other[i]);
            m_size = other.size();
            return *this;
        }

        template< class InputIterator >
        vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type(),
               typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL ) :
                m_alloc( alloc ),
                m_items( NULL )
        {
            difference_type _size = ft::distance(first, last);

            if (m_size != 0) {
                m_items = m_alloc.allocate(_size);
                for (size_type i = 0; first != last; first++, i++) {
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
        void assign( InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* = NULL){
            clear();
            for ( InputIterator it = first ; it != last ; it++ )
                push_back( *it );
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
                for (size_type index = distance; index < m_size; ++index) {
                    m_alloc.destroy( &m_items[index] );
                    if (index != m_size - 1)
                        m_alloc.construct( &m_items[index], m_items[index + 1]);
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