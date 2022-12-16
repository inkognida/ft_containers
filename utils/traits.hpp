#ifndef TRAITS_HPP
#define TRAITS_HPP

namespace ft {
    template<bool B, class T = void>
    struct enable_if {};

    template<class T>
    struct enable_if<true,T> { typedef T type; };

    template<class T, T v>
    struct integral_constant {
        static const T value = v;
        typedef T value_type;
        typedef integral_constant<T, v> type;
    };

    struct true_type: ft::integral_constant<bool, true> {};

    struct false_type: ft::integral_constant<bool, false> {};

    template<class Ty>
    struct is_integral: public ft::false_type {};

    template <> struct is_integral<bool> 				: public ft::true_type {};
    template <> struct is_integral<char> 				: public ft::true_type {};
    template <> struct is_integral<signed char> 		: public ft::true_type {};
    template <> struct is_integral<unsigned char> 		: public ft::true_type {};
    template <> struct is_integral<wchar_t> 			: public ft::true_type {};
    template <> struct is_integral<short> 				: public ft::true_type {};
    template <> struct is_integral<unsigned short> 		: public ft::true_type {};
    template <> struct is_integral<int> 				: public ft::true_type {};
    template <> struct is_integral<unsigned int> 		: public ft::true_type {};
    template <> struct is_integral<long>				: public ft::true_type {};
    template <> struct is_integral<unsigned long> 		: public ft::true_type {};
    template <> struct is_integral<long long> 			: public ft::true_type {};
    template <> struct is_integral<unsigned long long> 	: public ft::true_type {};
}

#endif