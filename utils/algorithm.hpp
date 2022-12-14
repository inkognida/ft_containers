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
}

#endif