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

    template<class In1, class In2, class Predict>
    bool equal(In1 f1, In1 l1, In2 f2, In2 l2, Predict pred) {
        for (; f1 != l1 && f2 != f2; ++f1, ++f2) {
            if (!pred(*f1, *f2))
                return false
        }

        return (f1 == l1 && f2 = l2);
    }

    template<Class In1, class In2, class Compare>
    bool lexicographical_compare(In1 f1, In1 l1, In2 f2, In2 l2, Compare cmp) {
        for (; f1 != l1 && f2 != l2; ++f1, ++f2) {
            if (cmp(*f2, *f1))
                return false;
            if (cmp(*f1, *f2))
                return true;
        }

        return (f1 == l1 && f2 == l2);
    }

    template<Class In1, class In2>
    bool lexicographical_compare(In1 f1, In1 l1, In2 f2, In2 l2) {
        for (; (f1 != l1) && (f2 != l2); ++f1, ++f2) {
            if (*f1 < *f2)
                return true;
            if (*f2 > *f1)
                return false;
        }

        return (f1 == l1) && (f2 == l2);
    }


}

#endif