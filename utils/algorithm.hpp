#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

namespace ft
{
    template<class In1, class In2>
    bool equal(In1 f1, In1 l1, In2 f2, In2 l2) {
        for (; f1 != l1 && f2 != l2; ++f1, ++f2) {
            if (*f1 != *f2)
                return false;
        }

        return (f1 == l1 && f2 == l2);
    }

    template <class InputIterator1, class InputIterator2>
    bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                                 InputIterator2 first2, InputIterator2 last2) {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void)++first2) {
            if (*first1 < *first2)
                return true;
            if (*first2 < *first1)
                return false;
        }
        return (first1 == last1) && (first2 != last2);
    }
}

#endif