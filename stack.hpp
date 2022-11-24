/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hardella <hardella@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/24 16:31:28 by hardella          #+#    #+#             */
/*   Updated: 2022/11/24 18:20:34 by hardella         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
#define STACK_HPP

# include <memory>
# include <iterator>
# include <limits>
# include <stdexcept>
# include <stddef.h>
# include <iostream>
# include <string>
# include <sstream>
# include "vector.hpp"

namespace ft
{
    template<class T, class Container = ft::vector<T> >
    class stack
    {
    
    public:
        typedef T           value_type;
        typedef Container   container_type;
        typedef std::size_t size_type;

    private:
        container_type  elems; 
    
    public:
        explicit stack(const container_type &c = container_type()): m_items(c) {}
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
        
        template< class T, class Container >
        friend bool operator==( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs );   

        template< class T, class Container >
        friend bool operator!=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ); 
        
        template< class T, class Container >
        friend bool operator<( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs );      
    
        template< class T, class Container >
        friend bool operator>( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs );   
    
        template< class T, class Container >
        friend bool operator<=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs );   
    
        template< class T, class Container >
        friend bool operator>=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs );   
    };
    
    template< class T, class Container >
    bool operator==( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems == rhs.elems; }

    template< class T, class Container >
    bool operator!=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems != rhs.elems; }

    template< class T, class Container >
    bool operator<( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems < rhs.elems; }

    template< class T, class Container >
    bool operator>( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems > rhs.elems; }

    template< class T, class Container >
    bool operator<=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems <= rhs.elems; }

    template< class T, class Container >
    bool operator>=( const ft::stack<T,Container>& lhs, const ft::stack<T,Container>& rhs ) { return lhs.elems >= rhs.elems; }
    
} // namespace ft

#endif