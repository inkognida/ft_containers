#ifndef MAP_HPP
#define MAP_HPP

# include "utils/pair.hpp"
# include "utils/iterator.hpp"
# include "utils/algorithm.hpp"
# include "utils/RBtree.hpp"
# include "utils/tree_iterator.hpp"

namespace ft {

    template <class value_type, class key_type, class Compare>
    class value_compare_map {
    public:
        bool operator()(const value_type &x, const value_type &y) const {
            return comp(x.first, y.first);
        }

        bool operator()(const key_type &x, const value_type &y) const { return comp(x, y.first); }

        bool operator()(const value_type &x, const key_type &y) const { return comp(x.first, y); }

        value_compare_map(Compare c = Compare()) : comp(c) {}
        Compare comp;
    };

    template <class Key, class T, class Compare = std::less<const Key>,
            class Alloc = std::allocator<ft::pair<const Key, T> > >
    class map {
    public:
        typedef Key                                                     key_type;
        typedef T                                                       mapped_type;
        typedef ft::pair<const key_type, mapped_type>                   value_type;
        typedef Compare                                                 key_compare;
        typedef value_compare_map<value_type, key_type, key_compare>    map_compare;
        typedef Alloc                                                   allocator_type;
        typedef ft::node<value_type>                                    node;
        typedef typename allocator_type::reference                      reference;
        typedef typename allocator_type::const_reference                const_reference;
        typedef typename allocator_type::pointer                        pointer;
        typedef typename allocator_type::const_pointer                  const_pointer;
        typedef typename ft::node<value_type>::node_pointer             node_pointer;
        typedef typename ft::node<value_type>::const_node_pointer       const_node_pointer;
        typedef ft::tree_iterator<node_pointer, value_type>             iterator;
        typedef ft::const_tree_iterator<const_node_pointer, value_type> const_iterator;
        typedef ft::reverse_iterator<iterator>                          reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;
        typedef typename allocator_type::difference_type                difference_type;
        typedef typename allocator_type::size_type                      size_type;

    private:
        ft::red_black_tree<value_type, key_type, mapped_type, map_compare, allocator_type> _tree;

    public:
        explicit map(const key_compare    &comp = key_compare(), const allocator_type &alloc = allocator_type())
                : _tree(map_compare(comp), alloc) {}

        template <class InputIterator>
        map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(),
            const allocator_type &alloc = allocator_type()) : _tree(map_compare(comp), alloc) {
            _tree.insert(first, last);
        }

        map(const map &other) : _tree(other._tree) {}

        ~map() {}

        map &operator=(const map &other) {
            _tree = other._tree;
            return *this;
        }

        iterator begin() { return _tree.begin(); }

        const_iterator begin() const { return _tree.begin(); }

        iterator end() { return _tree.end(); }

        const_iterator end() const { return _tree.end(); }

        reverse_iterator rbegin() { return _tree.rbegin(); }

        const_reverse_iterator rbegin() const { return _tree.rbegin(); }

        reverse_iterator rend() { return _tree.rend(); }

        const_reverse_iterator rend() const { return _tree.rend(); }

        bool empty() const { return _tree.empty(); }

        size_type size() const { return _tree.size(); }

        size_type max_size() const { return _tree.max_size(); }

        mapped_type &operator[](const key_type &key) { return _tree[key]; }

        mapped_type &at(const key_type &key) { return _tree.at(key); }

        const mapped_type &at(const key_type &key) const { return _tree.at(key); }

        ft::pair<iterator, bool> insert(const value_type &value) {
            return _tree.insert(value);
        }

        iterator insert(iterator pos, const value_type &value) {
            return _tree.insert(pos, value);
        }

        template<class In>
        void insert(In f, In l) {
            _tree.insert(f, l);
        }

        void erase(iterator pos) {
            _tree.erase(pos.base());
        }

        void erase(iterator first, iterator last) {
            for (; first != last; )
                erase(iterator(first++));

        }

        size_type erase(const Key &key) {
            iterator x = find(key);

            if (x == end())
                return 0;
            erase(x);
            return 1;
        }

        void clear() {
            _tree.clear();
        }

        void swap(map &other) { _tree.swap(other._tree); }

        iterator find(const key_type &key) { return _tree.find(key); }

        const_iterator find(const key_type &key) const { return _tree.find(key); }

        key_compare key_comp() const { return key_compare(); }

        size_type count(const key_type &key) const { return find(key) != end() ? 1 : 0; }

        iterator lower_bound(const key_type &key) { return _tree.lower_bound(key); }

        const_iterator lower_bound(const key_type &key) const { return _tree.lower_bound(key); }

        iterator upper_bound(const key_type &key) { return _tree.upper_bound(key); }

        const_iterator upper_bound(const key_type &key) const { return _tree.upper_bound(key); }

        ft::pair<iterator, iterator> equal_range(const key_type &key) { return _tree.equal_range(key); }

        ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const { return _tree.equal_range(key); }

        allocator_type get_allocator() const { return _tree.get_allocator(); }

        class value_compare {
            friend class map;

        public:
            typedef bool       result_type;
            typedef value_type first_argument_type;
            typedef value_type second_argument_type;

            bool operator()(const value_type &x, const value_type &y) const {
                return comp(x.first, y.first);
            }

        protected:
            value_compare(Compare c) : comp(c) {}
            Compare comp;
        };
        value_compare value_comp() const { return value_compare(key_compare()); }
    };

    template <class Key, class T, class Compare, class Alloc>
    void swap(ft::map<Key, T, Compare, Alloc> &x, ft::map<Key, T, Compare, Alloc> &y) {
        x.swap(y);
    }

    template <class Key, class T, class Compare, class Alloc>
    inline bool operator==(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
    }

    template <class Key, class T, class Compare, class Alloc>
    inline bool operator!=(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return !(x == y);
    }

    template <class Key, class T, class Compare, class Alloc>
    inline bool operator<(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template <class Key, class T, class Compare, class Alloc>
    inline bool operator>(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return y < x;
    }
    template <class Key, class T, class Compare, class Alloc>
    inline bool operator<=(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return !(y < x);
    }
    template <class Key, class T, class Compare, class Alloc>
    inline bool operator>=(const ft::map<Key, T, Compare, Alloc>& x, const ft::map<Key, T, Compare, Alloc>& y) {
        return !(x < y);
    }
}

#endif