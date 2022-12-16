#ifndef MAP_HPP
#define MAP_HPP

# include "utils/pair.hpp"

namespace ft {
    template<class Key, class T, class Compare = std::less<const Key>,
            class Allocator = std::allocator<ft::pair<const Key, T>>>
    class map {
    public:
        typedef Key                                                     key_type;
        typedef T                                                       mapped_type;
        typedef ft::pair<const key_type, mapped_type>                   value_type;
        typedef std::size_t                                             size_type;
        typedef std::ptrdiff_t                                          difference_type;
        typedef Compare                                                 key_compare;
        typedef value_compare_map<value_type, key_type, key_compare>    map_compare;
        typedef Allocator                                               allocator_type;
        typedef value_type&                                             reference;
        typedef const value_type&                                       const_reference;

        typedef typename allocator_type::pointer                        pointer;
        typedef typename allocator_type::const_pointer                  const_pointer;

        typedef typename ft::node<value_type>::node_pointer             node_pointer;
        typedef typename ft::node<value_type>::const_node_pointer       const_node_pointer;

        typedef ft::tree_iterator<node_pointer, value_type>             iterator;
        typedef ft::const_tree_iterator<const_node_pointer, value_type> const_iterator;
        typedef ft::reverse_iterator<iterator>                          reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;

        map(): _tree();

        explicit map(const Compare &comp, const Allocator &alloc = Allocator()):
            _tree(map_compare(comp), alloc) {}

        template<class In>
        map(In f, In l, const Compare &comp = Compare(), const Allocator &alloc = Allocator()):
            _tree(map_compare(comp), alloc) {
            _tree.insert(f, l);
        }

        map(const map &other): _tree(other._tree) {}

        ~map() {}

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

        //NEED TO CHECK
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


    private:
        ft::red_black_tree<value_type, key_type, mapped_type, map_compare, allocator_type> _tree;
    };

}


#endif