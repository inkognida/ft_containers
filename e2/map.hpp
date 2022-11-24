#ifndef MAP_HPP
#define MAP_HPP

# include <functional>
# include <memory>
# include "utility/pair.hpp"
# include "traits/iterator.hpp"
# include "algorithm/algorithm.hpp"

namespace ft
{
	/**
	 * Forward declarations
	 * 
	 */
	template<class T>
	struct rb_iterator;

	template<class T>
	struct rb_const_iterator;

	template<class T>
	struct rb_reverse_iterator;

	template<class T>
	struct rb_const_reverse_iterator;


	enum rb_color { RB_COLOR_BLACK = 1, RB_COLOR_RED, RB_COLOR_SENTINEL, RB_COLOR_NULL };
	/**
	 * Internal struct representing a binary tree node.
	 * 
	 */
	template<class T, class Alloc = std::allocator<T> >
	struct rb_node {
		typedef T												value_type;
		typedef typename ft::rb_node<T>							node_type;
		typedef Alloc											allocator_type;
		typedef typename std::allocator<node_type>				node_allocator_type;
		typedef typename node_allocator_type::reference			reference;
		typedef typename node_allocator_type::const_reference	const_reference;
		typedef typename node_allocator_type::pointer			pointer;
		typedef typename node_allocator_type::const_pointer		const_pointer;
		typedef typename std::size_t							size_type;

		value_type	data;
		pointer		parent;
		pointer		left;
		pointer		right;
		int			color;

		/**
		 * Default constructor
		 * 
		 * Constructs a new empty node with default value_type() and all
		 * pointers to NULL.
		 * 
		 * Color is always set to RED as it will always be inserted as a RED
		 * node.
		 * 
		 */
		rb_node() 
			: data( value_type() ), parent( NULL ), left( NULL ), right( NULL ), color( RB_COLOR_RED ) { }

		/**
		 * Data constructor
		 * 
		 * Constructs a new node with data and all pointers to NULL.
		 * 
		 * Color is always set to RED as it will always be inserted as a RED
		 * node.
		 * 
		 */
		rb_node( const value_type &__data )
			: data( __data ), parent( NULL ), left( NULL ), right( NULL ), color( RB_COLOR_RED ) { }

		/**
		 * Data-Parent constructor
		 * 
		 * Constructs a new node with data and a parent, right and left 
		 * pointers are set to NULL.
		 * 
		 * Color is always set to RED as it will always be inserted as a RED
		 * node.
		 * 
		 */
		rb_node( const value_type &__data, pointer __parent )
			: data( __data ), parent( __parent ), left( NULL ), right( NULL ), color( RB_COLOR_RED ) { }

		/**
		 * Get the grand parent
		 * 
		 * Returns the grand parent of the current node.
		 * 
		 */
		pointer grand_parent() {
			if ( this->parent == NULL )
				return NULL;
			return this->parent->parent;
		}

		/**
		 * Get uncle node
		 * 
		 * Returns the uncle node of the current node.
		 * 
		 */
		pointer uncle() {
			pointer gp = this->grand_parent();

			if ( gp == NULL )
				return NULL;
			return this->parent->sibling();
		}

		/**
		 * Get the sibling node
		 * rb_node
		 * Returns the sibling node.
		 * 
		 */
		pointer sibling() {
			if ( this->parent == NULL )
				return NULL;
			if ( this == this->parent->left )
				return this->parent->right;
			return this->parent->left;
		}

		bool is_sentinel() const {
			return color == RB_COLOR_SENTINEL;
		}

		bool is_left() const {
			if ( this->parent == NULL )
				return false;
			return this->parent->left == this;
		}

		bool is_right() const {
			if ( this->parent == NULL )
				return false;
			return this->parent->right == this;
		}

		bool is_null_node() const {
			return this->color == RB_COLOR_NULL;
		}

		void set_left( pointer node ){
			this->left = node;
			node->parent = this;
		}

		void set_right( pointer node ) {
			this->right = node;
			node->parent = this;
		}

        void detach() {
            if ( this->parent ){
                if ( this->is_left() ){
                    this->parent->left = NULL;
                } else {
                    this->parent->right = NULL;
                }
                this->parent = NULL;
            }
        }

		void assign( pointer node ){
			if ( node ){
				this->parent = node->parent;
				this->color = node->color;
				this->set_left(node->left);
				this->set_right(node->right);
			}
		}
		
		size_type max_size() const {
			return node_allocator_type().max_size();
		}

		class Compare {
			private:
				pointer m_node;
			public:
			Compare(pointer node) : m_node(node) { }
			Compare(const Compare &comp) : m_node( comp.m_node ) { }
			Compare &operator=(const Compare &comp) {
				m_node = comp.m_node;
				return *this;
			}
			bool operator()(pointer node) const {
				return m_node == node;
			}
		};

		static Compare create_compare( pointer node ){
			return Compare(node);
		}

		static pointer create_node( node_allocator_type alloc = node_allocator_type() ){
			pointer node = alloc.allocate( 1 );

			alloc.construct( node, rb_node() );
			return node;
		}

		static pointer create_node( const value_type &data, node_allocator_type alloc = node_allocator_type() ){
			pointer node = alloc.allocate( 1 );

			alloc.construct( node, data );
			return node;
		}

		static pointer create_node( const value_type &data, pointer parent, node_allocator_type alloc = node_allocator_type() ){
			pointer node = alloc.allocate( 1 );

			alloc.construct( node, rb_node( data, parent ) );
			return node;
		}

		static pointer create_sentinel_node( node_allocator_type alloc = node_allocator_type() ){
			pointer node = rb_node::create_node( alloc );

			node->color = RB_COLOR_SENTINEL;
			return node;
		}

		static pointer create_null_node(pointer parent, node_allocator_type alloc = node_allocator_type()){
			pointer node = rb_node::create_node( alloc );

			node->color = RB_COLOR_BLACK;
			node->parent = parent;
			node->left = NULL;
			node->right = NULL;
			return node;
		}

		static pointer create_null_node( node_allocator_type alloc = node_allocator_type() ){
			pointer node = rb_node::create_node( alloc );

			node->color = RB_COLOR_BLACK;
			return node;
		}

		/**
		 * Destroy node
		 * 
		 * Call the delete operator of the node and deallocate memory.
		 */
		static void destroy_node( pointer node, node_allocator_type alloc = node_allocator_type() ){
			if ( node != NULL ){
				alloc.destroy( node );
				alloc.deallocate( node, 1 );
				node = NULL;
			}
		}
	};


	/**
	 * 
	 * Map
	 * 
	 * Maps are associative containers that store elements formed by a combination of a
	 * key value and a mapped value, following a specific order.
	 * 
	 * In a map, the key values are generally used to sort and uniquely identify the elements,
	 * while the mapped values store the content associated to this key. The types of key and 
	 * mapped value may differ, and are grouped together in member type value_type, which is 
	 * a pair type combining both:
	 * 
	 * typedef pair<const Key, T> value_type;
	 * 
	 * Internally, the elements in a map are always sorted by its key following a specific strict 
	 * weak ordering criterion indicated by its internal comparison object (of type Compare).
	 * 
	 * map containers are generally slower than unordered_map containers to access individual 
	 * elements by their key, but they allow the direct iteration on subsets based on their order.
	 * 
	 * The mapped values in a map can be accessed directly by their corresponding 
	 * key using the bracket operator ((operator[]).
	 * 
	 * Maps are typically implemented as binary search trees.
	 * 
	 */
	template<
		class Key,												// map::key_type
		class T,												// map::mapped_type
		class Compare = std::less<Key>,							// map::key_compare
		class Alloc = std::allocator<ft::pair<const Key, T> >	// map::allocator_type
	>
	class map {
		public:
			typedef Key																		key_type;
			typedef T																		mapped_type;
			typedef ft::pair<const key_type, mapped_type>									value_type;
			typedef Compare																	key_compare;
			typedef Alloc																	allocator_type;
			typedef typename allocator_type::reference										reference;
			typedef typename allocator_type::const_reference								const_reference;
			typedef typename allocator_type::pointer										pointer;
			typedef typename allocator_type::const_pointer									const_pointer;
			typedef std::ptrdiff_t															difference_type;
			typedef std::size_t																size_type;
			typedef rb_iterator<value_type>													iterator;
			typedef rb_const_iterator<value_type>											const_iterator;
			typedef ft::reverse_iterator<iterator>											reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>									const_reverse_iterator;

			class value_compare : std::binary_function<value_type, value_type, bool> {
				friend class map;
				protected:
					Compare m_comp;
					value_compare(Compare comp) : m_comp(comp) {}
				public:
					typedef bool result_type;
					typedef value_type first_argument_type;
					typedef value_type second_argument_type;
					bool operator()(const value_type &x, const value_type &y) const {
						return m_comp(x.first, y.first);
					}
			};
		
		private:
			typedef rb_node<value_type>			node_type;
			typedef typename node_type::pointer	node_pointer;

		/**
		 * Member variables
		 */
		private:
			node_pointer						m_root;
			size_type							m_size;
			key_compare							m_comp;
			allocator_type						m_alloc;
			node_pointer						m_right_sentinel;
			node_pointer						m_left_sentinel;
			node_pointer						m_null;
			typename node_type::Compare         m_is_null_node;

		/**
		 * Public member functions.
		 * 
		 */
		public:
			/**
			 * Construct map
			 * 
			 * (1) empty container constructor (default constructor)
			 * 
			 * Constructs an empty container, with no elements.
			 */
			explicit map( const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type() )
				:   m_root( NULL ),
					m_size( 0 ),
					m_comp( comp ),
					m_alloc( alloc ),
					m_right_sentinel( node_type::create_sentinel_node() ),
					m_left_sentinel( node_type::create_sentinel_node() ),
					m_null( node_type::create_null_node() ),
					m_is_null_node( node_type::create_compare( m_null ) )
			{
				
			}

			/**
			 * Construct map
			 * 
			 * (2) range constructor
			 * 
			 * Constructs a container with as many elements as the range [first,last), with each element constructed from its
			 * corresponding element in that range.
			 */
			template<class InputIterator>
			map(InputIterator first, InputIterator last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
				:   m_root( NULL ),
					m_size( 0 ),
					m_comp( comp ),
					m_alloc( alloc ),
					m_right_sentinel( node_type::create_sentinel_node() ),
					m_left_sentinel( node_type::create_sentinel_node() ),
					m_null( node_type::create_null_node() ),
					m_is_null_node( node_type::create_compare( m_null ) )
			{
				for ( ; first != last ; ++first ){
					insert(*first);
				}
			}

			/**
			 * Constructor map
			 * 
			 * (3) copy constructor
			 * 
			 * Constructs a container with a copy of each of the elements in x.
			 */
			map(const map &x) :
				m_root(NULL),
				m_size(0),
				m_comp(x.m_comp),
				m_alloc(x.m_alloc),
				m_right_sentinel(node_type::create_sentinel_node()),
				m_left_sentinel(node_type::create_sentinel_node()),
				m_null(node_type::create_null_node()),
				m_is_null_node(node_type::create_compare(m_null))
			{
				// Copy with breath first search, to get rid of rebalancing ?
				// When x is copied, the traversal will be in order, that will cause
				// rebalancing.
				this->insert(x.begin(), x.end());
			}

			/**
			 * Copy container content
			 * 
			 * Assigns new contents to the container, replacing its current content.
			 * 
			 * Copies all the elements from x into the container, changing its size accordingly.
			 * 
			 * The container preserves its current allocator, which is used to allocate additional storage if needed.
			 */
			map &operator=(const map &x){
				if ( this == &x ){
					return *this;
				}
				// Delete previous
				this->clear();

				// Copy
				m_comp = x.m_comp;
				this->insert(x.begin(), x.end());
				return *this;
			}

			~map() {
				clear();
				node_type::destroy_node(m_right_sentinel);
				node_type::destroy_node(m_left_sentinel);
				node_type::destroy_node(m_null);
			}

			/**
			 * Insert elements
			 * 
			 * Extends the container by inserting new elements, effectively increasing the container 
			 * size by the number of elements inserted.
			 * 
			 * Because element keys in a map are unique, the insertion operation checks whether each 
			 * inserted element has a key equivalent to the one of an element already in the container, 
			 * and if so, the element is not inserted, returning an iterator to this existing element 
			 * (if the function returns a value).
			 * 
			 * For a similar container allowing for duplicate elements, see multimap.
			 * 
			 * An alternative way to insert elements in a map is by using member function map::operator[].
			 * 
			 * Internally, map containers keep all their elements sorted by their key following the criterion 
			 * specified by its comparison object. The elements are always inserted in its respective position 
			 * following this ordering.
			 * 
			 */
			ft::pair<iterator, bool> insert(const value_type &val){
				ft::pair<iterator, bool> ret;

				if ( m_root == NULL ){
					m_size++;
					m_root = node_type::create_node( val );
					m_root->set_right(m_right_sentinel);
					m_root->set_left(m_left_sentinel);
					return ft::pair<iterator, bool>( iterator( m_root ), true );
				}
				ret = insert_recursive_(m_root, val);
				if ( ret.second ){
					m_size++;
					this->rb_insert_fix_tree_(ret.first.m_ptr);
				}
				return ret;
			}

			template<class InputIterator>
			void insert(InputIterator first, InputIterator last){
				for ( ; first != last ; ++first ){
					this->insert(*first);
				}
			}

			iterator insert(iterator position, const value_type &val){
				iterator					curr = position;
				iterator					next = position;
				ft::pair<iterator, bool>	result;

				// If the position point to the end, insert normally
				if ( size() == 0 || position == this->end() || position == (--this->begin()) ){
					return this->insert(val).first;
				}
				// Otherwise, try another strategy, check that the val is within the range (position, position + 1)
				// until a valid range is found, where node will be inserted.
				// Check if greater or lower to determine the direction
				if ( is_equal_val_(*position, val) ){
					return position;
				}
				if ( this->value_comp()(*position, val) ){
					// Go to right
					while ( next != this->end() ) {
						curr = next++;
						
						// Not found, insert at the end
						if ( next == this->end() )
							break;
						if ( is_equal_val_(*curr, val) )
							return curr;
						if ( this->value_comp()(*curr, val) && this->value_comp()(val, *next) )
							break;
					}
				} else {
					// Go to left
					while ( next != this->begin() ){
						curr = next--;
						
						// Not found, insert at the end
						if ( next == this->begin() )
							break;
						if ( is_equal_val_(*curr, val) )
							return curr;
						if ( this->value_comp()(val, *curr) && this->value_comp()(*next, val) )
							break;
					}
				}
				result = insert_recursive_(curr.m_ptr, val);
				if ( result.second )
					m_size++;
				return result.first;
			}

			/**
			 * Access element
			 * 
			 * If k matches the key of an element in the container, the function returns a reference to its mapped value.
			 * If k does not match the key of any element in the container, the function inserts a new element with that
			 * key and returns a reference to its mapped value. Notice that this always increases the container size by
			 * one, even if no mapped value is assigned to the element (the element is constructed using its default constructor).
			 * A similar member function, map::at, has the same behavior when an element with the key exists, 
			 * but throws an exception when it does not.
			 * 
			 */
			mapped_type& operator[](const key_type& k) {
				return (*((this->insert(ft::make_pair(k,mapped_type()))).first)).second;
			}

			/**
			 * Clear content
			 * 
			 * Removes all elements from the map container (which are destroyed), leaving the container with a size of 0.
			 */
			void clear() {
				if ( m_root ){
					clear_recursive_(m_root);
					m_root = NULL;
				}
				m_size = 0;
			}

			/**
			 * Get iterator to element
			 * 
			 * Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
			 * otherwise it returns an iterator to map::end.
			 * 
			 * Two keys are considered equivalent if the container's comparison object returns false reflexively 
			 * (i.e., no matter the order in which the elements are passed as arguments).
			 * Another member function, map::count, can be used to just check whether a particular key exists.
			 */
			iterator find(const key_type &k){
				iterator found = this->lower_bound(k);

				return ( found == this->end() || (m_comp(k, (*found).first))) ? this->end() : found;
			}

			/**
			 * Get iterator to element
			 * 
			 * Searches the container for an element with a key equivalent to k and returns an iterator to it if found,
			 * otherwise it returns an iterator to map::end.
			 * 
			 * Two keys are considered equivalent if the container's comparison object returns false reflexively 
			 * (i.e., no matter the order in which the elements are passed as arguments).
			 * Another member function, map::count, can be used to just check whether a particular key exists.
			 */
			const_iterator find(const key_type &k) const {
				const_iterator found = this->lower_bound(k);

				return ( found == this->end() || (m_comp(k, (*found).first))) ? this->end() : found;
			}

			/**
			 * Erase elements
			 * 
			 * Removes from the map container either a single element or a range of elements ([first,last)).
			 * 
			 * This effectively reduces the container size by the number of elements removed, which are destroyed.
			 */
			void erase(iterator position){
				if ( position != this->end() ){
					node_pointer target = position.m_ptr;
					node_pointer successor;
					int         prev_color = target->color;
					bool        is_root = target == m_root;

					if ( target->left != NULL && target->right != NULL ){
						// Case 3, node has children
						// Go to the right subtree, then find the min (go to leftmost)
						if ( target->left->is_sentinel() && target->right->is_sentinel() ){
							m_root = NULL;
							successor = NULL;
						} else {
							ft::pair<node_pointer, int> ret = detach_node_(target);

							prev_color = ret.second;
							successor = (!is_root) ? ret.first : m_root;
						}
					}
					else if ( target->left != NULL || target->right != NULL){
						// Case 2, target has one child
						successor = (target->right != NULL) ? target->right : target->left;

						if ( target->is_left() ){
							target->parent->set_left(successor);
						} else {
							target->parent->set_right(successor);
						}
						if ( successor->is_sentinel() )
							successor = target->parent;
					}
					else {
						// Case 1, target has no children
						m_null->parent = target->parent;
						successor = m_null;
						if ( target->is_left() ){
							target->parent->left = m_null;
						} else {
							target->parent->right = m_null;
						}
					}
					node_type::destroy_node( target );
					m_size--;
					if ( m_root != NULL && !m_root->left->is_sentinel() && !m_root->right->is_sentinel() &&
						prev_color == RB_COLOR_BLACK ){
							this->rb_erase_fix_(successor);
					}
                    m_null->detach();
				}
			}

			/**
			 * Erase elements
			 * 
			 * Removes from the map container either a single element or a range of elements ([first,last)).
			 * 
			 * This effectively reduces the container size by the number of elements removed, which are destroyed.
			 */
			size_type erase(const key_type &k){
				iterator it = this->find( k );

				if ( it == this->end() )
					return 0;
				this->erase( it );
				return 1;
			}

			/**
			 * Erase elements
			 * 
			 * Removes from the map container either a single element or a range of elements ([first,last)).
			 * 
			 * This effectively reduces the container size by the number of elements removed, which are destroyed.
			 */
			void erase(iterator first, iterator last){
				while ( first != last ){
					this->erase((*first++).first);
				}
			}

			/**
			 * Swap content
			 * 
			 * Exchanges the content of the container by the content of x, which is another map of the same type. 
			 * Sizes may differ.
			 * 
			 * After the call to this member function, the elements in this container are those which were in x 
			 * before the call, and the elements of x are those which were in this.
			 * All iterators, references and pointers remain valid for the swapped objects.
			 * 
			 * Notice that a non-member function exists with the same name, swap, overloading that algorithm 
			 * with an optimization that behaves like this member function.
			 * 
			 */
			void swap(map &x){
				node_pointer				tmp_root = x.m_root;
				size_type					tmp_size = x.m_size;
				key_compare					tmp_comp = x.m_comp;
				allocator_type				tmp_alloc = x.m_alloc;
				node_pointer				tmp_right_sentinel = x.m_right_sentinel;
				node_pointer				tmp_left_sentinel = x.m_left_sentinel;
				typename node_type::Compare tmp_is_null_node = x.m_is_null_node;

				x.m_root = this->m_root;
				x.m_size = this->m_size;
				x.m_comp = this->m_comp;
				x.m_alloc = this->m_alloc;
				x.m_right_sentinel = this->m_right_sentinel;
				x.m_left_sentinel = this->m_left_sentinel;
				x.m_is_null_node = this->m_is_null_node;
				
				this->m_root = tmp_root;
				this->m_size = tmp_size;
				this->m_comp = tmp_comp;
				this->m_alloc = tmp_alloc;
				this->m_right_sentinel = tmp_right_sentinel;
				this->m_left_sentinel = tmp_left_sentinel;
				this->m_is_null_node = tmp_is_null_node;
			}

			/**
			 * Return container size
			 * 
			 * Returns the number of elements in the map container.
			 */
			size_type size() const {
				return m_size;
			}

			/**
			 * Test whether container is empty
			 * 
			 * Returns whether the map container is empty (i.e. whether its size is 0).
			 * 
			 * This function does not modify the container in any way. To clear the content of a map container, 
			 * see map::clear.
			 */
			bool empty() const {
				return m_size == 0;
			}

			/**
			 * Return maximum size
			 * 
			 * Returns the maximum number of elements that the map container can hold.
			 * 
			 * This is the maximum potential size the container can reach due to known
			 * system or library implementation limitations, but the container is by 
			 * no means guaranteed to be able to reach that size: it can still fail to 
			 * allocate storage at any point before that size is reached.
			 */
			size_type max_size() const {
				return ( node_type().max_size() );
			}

			/**
			 * Return iterator to begining
			 * 
			 * Returns an iterator referring to the first element in the map container
			 * Because map containers keep their elements ordered at all times, begin points to the element that goes 
			 * first following the container's sorting criterion
			 * If the container is empty, the returned iterator value shall not be dereferenced.
			 * 
			 */
			iterator begin(){
				if ( m_root == NULL ){
					return end();
				}
				return iterator( m_left_sentinel->parent );
			}

			/**
			 * Return iterator to begining
			 * 
			 * Returns an iterator referring to the first element in the map container
			 * Because map containers keep their elements ordered at all times, begin points to the element that goes 
			 * first following the container's sorting criterion
			 * If the container is empty, the returned iterator value shall not be dereferenced.
			 * 
			 */
			const_iterator begin() const {
				if ( m_root == NULL ){
					return end();
				}
				return const_iterator( m_left_sentinel->parent );
			}

			/**
			 * Return reverse iterator to reverse beginning
			 * 
			 * Returns a reverse iterator pointing to the last element in the container (i.e., its reverse beginning).
			 * 
			 * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
			 * 
			 * rbegin points to the element preceding the one that would be pointed to by member end.
			 */ 
			reverse_iterator rbegin() {
				if ( m_root == NULL ){
					return rend();
				}
				return reverse_iterator( end() );
			}

			/**
			 * Return reverse iterator to reverse beginning
			 * 
			 * Returns a reverse iterator pointing to the last element in the container (i.e., its reverse beginning).
			 * 
			 * Reverse iterators iterate backwards: increasing them moves them towards the beginning of the container.
			 * 
			 * rbegin points to the element preceding the one that would be pointed to by member end.
			 */
			const_reverse_iterator rbegin() const {
				if ( m_root == NULL ){
					return rend();
				}
				return const_reverse_iterator( end() );
			}

			/**
			 * Return iterator to end
			 * 
			 * Returns an iterator referring to the past-the-end element in the map container.
			 * 
			 * The past-the-end element is the theoretical element that would follow the last 
			 * element in the map container. It does not point to any element, and thus shall not be dereferenced.
			 * 
			 * Because the ranges used by functions of the standard library do not include the element pointed 
			 * by their closing iterator, this function is often used in combination with map::begin to specify 
			 * a range including all the elements in the container.
			 * 
			 * If the container is empty, this function returns the same as map::begin.
			 */
			iterator end() {
				return iterator( m_right_sentinel );
			}

			/**
			 * Return iterator to end
			 * 
			 * Returns an iterator referring to the past-the-end element in the map container.
			 * 
			 * The past-the-end element is the theoretical element that would follow the last 
			 * element in the map container. It does not point to any element, and thus shall not be dereferenced.
			 * 
			 * Because the ranges used by functions of the standard library do not include the element pointed 
			 * by their closing iterator, this function is often used in combination with map::begin to specify 
			 * a range including all the elements in the container.
			 * 
			 * If the container is empty, this function returns the same as map::begin.
			 */
			const_iterator end() const {
				return const_iterator( m_right_sentinel );
			}

			/**
			 * Return reverse iterator to reverse end
			 * 
			 * Returns a reverse iterator pointing to the theoretical element right before the first element in the map container
			 * (which is considered its reverse end).
			 * The range between map::rbegin and map::rend contains all the elements of the container (in reverse order).
			 */
			reverse_iterator rend() {
				return ( reverse_iterator( begin() ) );
			}

			/**
			 * Return reverse iterator to reverse end
			 * 
			 * Returns a reverse iterator pointing to the theoretical element right before the first element in the map container
			 * (which is considered its reverse end).
			 * The range between map::rbegin and map::rend contains all the elements of the container (in reverse order).
			 */
			const_reverse_iterator rend() const {
				return ( const_reverse_iterator( begin() ) );
			}


			/**
			 * Return iterator to upper bound
			 * 
			 * Returns an iterator pointing to the first element in the container whose key is considered to go after k.
			 * 
			 * The function uses its internal comparison object (key_comp) to determine this, returning an iterator to 
			 * the first element for which key_comp(k,element_key) would return true.
			 * 
			 * If the map class is instantiated with the default comparison type (less), the function returns an iterator 
			 * to the first element whose key is greater than k.
			 * 
			 * A similar member function, lower_bound, has the same behavior as upper_bound, except in the case that the 
			 * map contains an element with a key equivalent to k: In this case lower_bound returns an iterator pointing 
			 * to that element, whereas upper_bound returns an iterator pointing to the next element.
			 */
			iterator upper_bound(const key_type &k){
				node_pointer y = NULL;
				node_pointer x = m_root;

				while ( x != NULL && !x->is_sentinel() ){
					if ( m_comp(k, x->data.first) ){
						y = x;
						x = x->left;
					} else {
						x = x->right;
					}
				}
				return (y == NULL) ? this->end() : iterator(y);
			}

			const_iterator upper_bound(const key_type &k) const {
				node_pointer y = NULL;
				node_pointer x = m_root;

				while ( x != NULL && !x->is_sentinel() ){
					if ( m_comp(k, x->data.first) ){
						y = x;
						x = x->left;
					} else {
						x = x->right;
					}
				}
				return (y == NULL) ? this->end() : const_iterator(y);
			}

			/**
			 * Return iterator to lower bound
			 * 
			 * Returns an iterator pointing to the first element in the container whose key is not considered to go before 
			 * k (i.e., either it is equivalent or goes after).
			 * 
			 * The function uses its internal comparison object (key_comp) to determine this, returning an iterator to the
			 *  first element for which key_comp(element_key,k) would return false.
			 * If the map class is instantiated with the default comparison type (less), the function returns an iterator to the first element whose key is not less than k.
			 * A similar member function, upper_bound, has the same behavior as lower_bound, except in the case that the map contains an element with a key equivalent to k: In this case, lower_bound returns an iterator pointing to that element, whereas upper_bound returns an iterator pointing to the next element.
			 */
			iterator lower_bound(const key_type &k){
				node_pointer y = NULL;
				node_pointer x = m_root;

				while ( x != NULL && !x->is_sentinel() ){
					if ( !m_comp(x->data.first, k) ){
						y = x;
						x = x->left;
					} else {
						x = x->right;
					}
				}
				return (y == NULL) ? this->end() : iterator(y);
			}

			const_iterator lower_bound(const key_type &k) const {
				node_pointer y = NULL;
				node_pointer x = m_root;

				while ( x != NULL && !x->is_sentinel() ){
					if ( !m_comp(x->data.first, k) ){
						y = x;
						x = x->left;
					} else {
						x = x->right;
					}
				}
				return (y == NULL) ? this->end() : const_iterator(y);
			}

			/**
			 * Get range of equal elements
			 * 
			 * Returns the bounds of a range that includes all the elements in the container which have a key equivalent to k.
			 * 
			 * Because the elements in a map container have unique keys, the range returned will contain a single element at most.
			 * 
			 * If no matches are found, the range returned has a length of zero, with both iterators pointing to the first element 
			 * that has a key considered to go after k according to the container's internal comparison object (key_comp).
			 * 
			 * Two keys are considered equivalent if the container's comparison object returns false reflexively 
			 * (i.e., no matter the order in which the keys are passed as arguments).
			 */
			ft::pair<iterator, iterator> equal_range(const key_type& k) {
				return ft::pair<iterator, iterator>( this->lower_bound(k), this->upper_bound(k) );
			}

			ft::pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
				return ft::pair<const_iterator, const_iterator>( this->lower_bound(k), this->upper_bound(k) );
			}

			/**
			 * Count elements with a specific key
			 * 
			 * Searches the container for elements with a key equivalent to k and returns the number of matches.
			 * 
			 * Because all elements in a map container are unique, the function can only return 1 (if the element is found) or zero (otherwise).
			 * Two keys are considered equivalent if the container's comparison object returns false reflexively 
			 * (i.e., no matter the order in which the keys are passed as arguments).
			 */
			size_type count(const key_type &k) const {
				return this->find(k) != this->end();
			}

			/**
			 * Get allocator
			 * 
			 * Returns a copy of the allocator object associated with the map.
			 */
			allocator_type get_allocator() const {
				return m_alloc;
			}

			/**
			 * Return value comparison object
			 * 
			 * Returns a comparison object that can be used to compare two elements to get whether the key of the first one goes before the second.
			 * 
			 * The arguments taken by this function object are of member type value_type (defined in map as an alias of pair<const key_type,mapped_type>),
			 * but the mapped_type part of the value is not taken into consideration in this comparison.
			 * 
			 * The comparison object returned is an object of the member type map::value_compare, which is a nested class that uses the internal comparison
			 * object to generate the appropriate comparison functional class.
			 * 
			 * The public member of this comparison class returns true if the key of the first argument is considered to go before that of the second 
			 * (according to the strict weak ordering specified by the container's comparison object, key_comp), and false otherwise.
			 * Notice that value_compare has no public constructor, therefore no objects can be directly created from this nested class outside map members.
			 */
			value_compare value_comp() const {
				return value_compare(m_comp);
			}

			/**
			 * Return key comparison object
			 * 
			 * Returns a copy of the comparison object used by the container to compare keys.
			 * 
			 * The comparison object of a map object is set on construction. Its type (member key_compare) is the third template parameter of the map template.
			 * By default, this is a less object, which returns the same as operator<.
			 * 
			 * This object determines the order of the elements in the container: it is a function pointer or a function object that takes two arguments of the
			 * same type as the element keys, and returns true if the first argument is considered to go before the second in the strict weak ordering it defines,
			 * and false otherwise.
			 * 
			 * Two keys are considered equivalent if key_comp returns false reflexively (i.e., no matter the order in which the keys are passed as arguments).
			 */
			key_compare key_comp() const {
				return m_comp;
			}

		/**
		 * Private implementations.
		 * 
		 */
		private:
			/**
			 * 
			 * Insert recursive
			 * 
			 * Insert a node recursively. It returns a pair with an iterator
			 * pointing to the node newly inserted.
			 * 
			 */
			ft::pair<iterator, bool> insert_recursive_(node_pointer current, const value_type &val){
				if ( this->is_equal_key_(val.first, current->data.first) ){
					return ft::pair<iterator, bool>( iterator( current ), false );
				}
				if ( m_comp( val.first, current->data.first ) ){
					if ( current->left == NULL ){
						return ft::pair<iterator, bool>( iterator( current->left = node_type::create_node( val, current ) ), true );
					} 
					else if ( current->left->is_sentinel() ){
						node_pointer node = node_type::create_node( val, current );

						current->left = node;
						node->set_left(m_left_sentinel);
						return ft::pair<iterator, bool>( iterator( node ), true );
					} else {
						return insert_recursive_(current->left, val);
					}
				} else {
					if ( current->right == NULL ){
						return ft::pair<iterator, bool>( iterator( current->right = node_type::create_node( val, current ) ), true );
					} else if ( current->right->is_sentinel() ){
						node_pointer node = node_type::create_node( val, current );

						current->right = node;
						node->set_right(m_right_sentinel);
						return ft::pair<iterator, bool>( iterator( node ), true );
					} else {
						return insert_recursive_(current->right, val);
					}
				}
			}

			void clear_recursive_(node_pointer current){
				if ( current != NULL && !current->is_sentinel() && !m_is_null_node(current) ){
					clear_recursive_(current->left);
					clear_recursive_(current->right);
					node_type::destroy_node(current);
				}
			}

			ft::pair<node_pointer, int> detach_node_(node_pointer target){
				node_pointer rightmost;
				node_pointer successor = target->right;
				ft::pair<node_pointer, int> ret;

				// If the min node is a sentinel node, then take the left child
				// and set the sentinel node to the rightmost subtree.
				if ( successor->is_sentinel() ){
					successor = target->left;
					successor->parent = target->parent;

					rightmost = successor;
					// Go to the right most node and set right sentinel
					while ( rightmost->right != NULL ){
						rightmost = rightmost->right;
					}
					// Not sure ?
					ret.first = successor;
					ret.second = target->color;
					rightmost->set_right(m_right_sentinel);
				} else {
					// Find the left most child
					while ( successor->left != NULL && !successor->left->is_sentinel() ){
						successor = successor->left;
					}
					// HERE ?
					ret.first = successor->right;
					ret.second = successor->color;
					if ( successor->parent == target ){
						successor->parent = target->parent;
						successor->set_left(target->left);
						ret.first = successor;
					} else {
						// If successor node has a child, then set the successor parent
						// set this node as his new left child, otherwise, it is set to NULL.
						if ( successor->right != NULL ){
							// Successor has child
							successor->parent->set_left(successor->right);
						} else {
							// Successor is a leaf
							// Set to m_null ?
							// successor->parent->left = NULL;
							successor->parent->left = m_null;
							m_null->parent = successor->parent;
							ret.first = m_null;
						}
						successor->assign( target );
					}
					successor->color = target->color;
				}
				// Set the parent to point to the new successor, otherwise root is set to NULL.
				if ( target->parent != NULL ){
					if ( target->is_left() ){
						target->parent->set_left(successor);
					} else {
						target->parent->set_right(successor);
					}
				}
				else {
					m_root = successor;
				}
				return ret;
			}

			void rb_erase_fix_(node_pointer x){
				node_pointer s;

				while ( x != m_root && (x->color == RB_COLOR_BLACK || x->color == RB_COLOR_SENTINEL) ){
					if ( x->is_left() ){
						s = x->parent->right;
						if (s->color == RB_COLOR_RED){
							// Case 3.1
							s->color = RB_COLOR_BLACK;
							x->parent->color = RB_COLOR_RED;
							rb_rotate_left_(x->parent);
							s = x->parent->right;
						}
						if (
                                ( s->left == NULL || s->left->color == RB_COLOR_BLACK || s->is_sentinel() ) &&
                                ( s->right == NULL || s->right->color == RB_COLOR_BLACK || s->is_sentinel() ) ){
							// Case 3.2
							s->color = RB_COLOR_RED;
							x = x->parent;
						}
						else {
							if ( s->right->color == RB_COLOR_BLACK ){
								// Case 3.3
								s->left->color = RB_COLOR_BLACK;
								s->color = RB_COLOR_RED;
								rb_rotate_right_(s);
								s = x->parent->right;
							}
							// Case 3.4
							s->color = x->parent->color;
							x->parent->color = RB_COLOR_BLACK;
							if ( !s->right->is_sentinel() )
								s->right->color = RB_COLOR_BLACK;
							rb_rotate_left_(x->parent);
							x = m_root;
						}
					} else {
						s = x->parent->left;
						
						if (s->color == RB_COLOR_RED){
							// Case 3.1
							s->color = RB_COLOR_BLACK;
							x->parent->color = RB_COLOR_RED;
							rb_rotate_right_(x->parent);
							s = x->parent->left;
						}

						if ( 
							( s->left == NULL || s->left->color == RB_COLOR_BLACK || s->left->is_sentinel() ) &&
							( s->right == NULL || s->right->color == RB_COLOR_BLACK || s->right->is_sentinel() ) ){
							// Case 3.2
							s->color = RB_COLOR_RED;
							x = x->parent;
						}
						else {
							if ( s->left->color == RB_COLOR_BLACK ){
								// Case 3.3
								s->right->color = RB_COLOR_BLACK;
								s->color = RB_COLOR_RED;
								rb_rotate_left_(s);
								s = x->parent->left;
							}
							// Case 3.4
							s->color = x->parent->color;
							x->parent->color = RB_COLOR_BLACK;
							if ( !s->left->is_sentinel() )
								s->left->color = RB_COLOR_BLACK;
							rb_rotate_right_(x->parent);
							x = m_root;
						}
					}
				}
				if ( !x->is_sentinel() ){
					x->color = RB_COLOR_BLACK;
				}
			}

			bool is_equal_key_(const key_type &a, const key_type &b){
				return !m_comp(a, b) && !m_comp(b, a);
			}

			bool is_equal_val_(const value_type &a, const value_type &b){
				return !m_comp(a.first, b.first) && !m_comp(b.first, a.first);
			}

			/**
			 * Red black tree utils
			 */

			void rb_insert_case_1_(node_pointer node){
				if ( node->parent == NULL )
					node->color = RB_COLOR_BLACK;
			}

			void rb_insert_case_2_(node_pointer node){
				(void)node;
				return ; // Uncessary call
			}

			void rb_insert_case_3_(node_pointer node){
				node->parent->color = RB_COLOR_BLACK;
				node->uncle()->color = RB_COLOR_BLACK;
				

				node_pointer gp = node->grand_parent();
				gp->color = RB_COLOR_RED;
				rb_insert_fix_tree_(gp);
			}

			void rb_insert_case_4_(node_pointer node){
				node_pointer p = node->parent;
				node_pointer gp = node->grand_parent();

				if ( gp != NULL ){
					if ( gp->left != NULL && node == gp->left->right ){
						rb_rotate_left_(p);
						node = node->left;
					}
					else if ( gp->right != NULL && node == gp->right->left ){
						rb_rotate_right_(p);
						node = node->right;
					}

					rb_insert_case_5_(node);
				}
			}

			void rb_insert_case_5_(node_pointer node){
				node_pointer p = node->parent;
				node_pointer gp = node->grand_parent();

				if ( node->is_left() ){
					rb_rotate_right_(gp);
				} else {
					rb_rotate_left_(gp);
				}

				p->color = RB_COLOR_BLACK;
				gp->color = RB_COLOR_RED;
			}

			void rb_replace_sentinels_(){
				node_pointer leftmost = m_root;
				node_pointer rightmost = m_root;

				// Leftmost
				node_pointer lp = m_left_sentinel->parent;

				if ( lp != NULL ){
					lp->left = NULL;
				}

				while ( leftmost->left != NULL )
					leftmost = leftmost->left;

				leftmost->set_left(m_left_sentinel);

				// Rightmost
				node_pointer rp = m_right_sentinel->parent;

				if ( rp != NULL ){
					rp->right = NULL;
				}

				while ( rightmost->right != NULL )
					rightmost = rightmost->right;

				rightmost->set_right(m_right_sentinel);
			}

			void rb_insert_fix_tree_(node_pointer node){
				node_pointer uncle = node->uncle();

				if ( node->parent == NULL ){
					rb_insert_case_1_(node);
				}
				else {
					if ( node->parent->color == RB_COLOR_BLACK ){
					rb_insert_case_2_(node);
					}
					else if ( uncle != NULL && uncle->color == RB_COLOR_RED ){
						rb_insert_case_3_(node);
					}
					else {
						rb_insert_case_4_(node);
					}
					rb_replace_sentinels_();
				}
			}

			void rb_rotate_left_(node_pointer x){
				node_pointer y = x->right;
				x->right = y->left;

				if ( y->left != NULL ){
					y->left->parent = x;
				}
				y->parent = x->parent;
				if ( x->parent == NULL ){
					m_root = y;
				}
				else if ( x == x->parent->left ){
					x->parent->left = y;
				} else {
					x->parent->right = y;
				}
				y->left = x;
				x->parent = y;
			}

			void rb_rotate_right_(node_pointer y){
				node_pointer x = y->left;
				y->left = x->right;

				if ( x->right != NULL ){
					x->right->parent = y;
				}
				x->parent = y->parent;
				if ( y->parent == NULL ){
					m_root = x;
				}
				else if ( y == y->parent->right ){
					y->parent->right = x;
				} else {
					y->parent->left = x;
				}
				x->right = y;
				y->parent = x;
			}
	};

	template<class T>
	rb_node<T> *_rb_tree_decrement(rb_node<T> *ptr){
		if ( ptr->left != NULL ){
			ptr = ptr->left;
			while ( ptr->right != NULL )
				ptr = ptr->right;
		} else {
			rb_node<T> *y = ptr->parent;
			while ( ptr == y->left ){
				ptr = y;
				y = y->parent;
			}
			if ( ptr->left != y )
				ptr = y;
		}
		return ptr;
	}

	template<class T>
	rb_node<T> *_rb_tree_increment(rb_node<T> *ptr){
		if ( ptr->right != NULL ){
			ptr = ptr->right;
			while ( ptr->left != NULL )
				ptr = ptr->left;
		} else {
			rb_node<T> *y = ptr->parent;
			while ( ptr == y->right ){
				ptr = y;
				y = y->parent;
			}
			if ( ptr->right != y )
				ptr = y;
		}
		return ptr;
	}

	template<class T>
	struct rb_iterator;

	template<class T>
	struct rb_const_iterator {
		typedef T								value_type;
		typedef const T&						reference;
		typedef const T*						pointer;

		typedef rb_iterator<T>						iterator;
		
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef rb_const_iterator<T>				_self;
		typedef rb_node<T>*						_base_ptr;
		typedef const rb_node<T>*				_link_type;

		/* Constructors */
		rb_const_iterator() : m_ptr( NULL ) { }
		rb_const_iterator(_base_ptr ptr) : m_ptr(ptr) { }
		rb_const_iterator(const iterator &it) : m_ptr( it.m_ptr ) { }

		~rb_const_iterator() {}

		/* Accesses operators */
		reference	operator*() const {
			return m_ptr->data;
		}
		pointer		operator->() const { return &m_ptr->data; }

		/* Increment / Decrement */
		_self &operator++() {
			m_ptr = _rb_tree_increment(m_ptr);
			return *this; 
		}

		_self operator++(int) { 
			_self tmp = *this; 
			m_ptr = _rb_tree_increment(m_ptr); 
			return tmp; 
		}

		_self &operator--() {
			m_ptr = _rb_tree_decrement(m_ptr);
			return *this;
		}
		_self operator--(int) {
			_self tmp = *this;
			m_ptr = _rb_tree_decrement(m_ptr);
			return tmp;
		}

		/* Comparison operators */
		bool operator!=(_self const &it) const { return m_ptr != it.m_ptr; }
		bool operator==(_self const &it) const { return m_ptr == it.m_ptr; }
		
		_base_ptr m_ptr;
	};

	template<class T>
	struct rb_iterator {
		typedef T								value_type;
		typedef T&								reference;
		typedef T*								pointer;
		
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef rb_iterator<T>						_self;
		typedef rb_node<T>*						_base_ptr;
		typedef rb_node<T>*						_link_type;

		/* Constructors */
		rb_iterator() : m_ptr( NULL ) { }
		rb_iterator(_base_ptr ptr) : m_ptr(ptr) { }

		~rb_iterator() {}

		/* Accesses operators */
		reference	operator*() const 
		{ return m_ptr->data; }

		pointer		operator->() const 
		{ return &m_ptr->data; }

		/* Increment / Decrement */
		_self &operator++() {
			m_ptr = _rb_tree_increment(m_ptr);
			return *this; 
		}

		_self operator++(int) { 
			_self tmp = *this; 
			m_ptr = _rb_tree_increment(m_ptr); 
			return tmp; 
		}

		_self &operator--() {
			m_ptr = _rb_tree_decrement(m_ptr);
			return *this;
		}
		_self operator--(int) {
			_self tmp = *this;
			m_ptr = _rb_tree_decrement(m_ptr);
			return tmp;
		}

		/* Comparison operators */
		bool operator!=(_self const &it) const { return m_ptr != it.m_ptr; }
		bool operator==(_self const &it) const { return m_ptr == it.m_ptr; }
		
		_base_ptr m_ptr;
	};

	template<typename T>
	struct rb_reverse_iterator {
		typedef T								value_type;
		typedef T&								reference;
		typedef T*								pointer;
		
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef rb_reverse_iterator<T>				_self;
		typedef rb_node<T>*						_base_ptr;
		typedef const rb_node<T>*				_link_type;

		/* Constructors */
		rb_reverse_iterator() : m_ptr( NULL ) { }
		rb_reverse_iterator(_base_ptr ptr) : m_ptr(ptr) { }
		~rb_reverse_iterator() {}

		/* Accesses operators */
		reference	operator*() const 
		{ return m_ptr->data; }

		pointer		operator->() const 
		{ return &m_ptr->data; }

		/* Increment / Decrement */
		_self &operator++() {
			m_ptr = _rb_tree_decrement(m_ptr);
			return *this; 
		}

		_self operator++(int) { 
			_self tmp = *this; 
			m_ptr = _rb_tree_decrement(m_ptr); 
			return tmp; 
		}

		_self &operator--() {
			m_ptr = _rb_tree_increment(m_ptr);
			return *this;
		}
		_self operator--(int) {
			_self tmp = *this;
			m_ptr = _rb_tree_increment(m_ptr);
			return tmp;
		}

		/* Comparison operators */
		bool operator!=(_self const &it) const { return m_ptr != it.m_ptr; }
		bool operator==(_self const &it) const { return m_ptr == it.m_ptr; }
		
		_base_ptr m_ptr;
	};

	template<typename T>
	struct rb_const_reverse_iterator {
		typedef T								value_type;
		typedef const T&						reference;
		typedef const T*						pointer;

		typedef rb_reverse_iterator<T>				iterator;
		
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef rb_const_reverse_iterator<T>		_self;
		typedef rb_node<T>*						_base_ptr;
		typedef const rb_node<T>*				_link_type;

		/* Constructors */
		rb_const_reverse_iterator() : m_ptr( NULL ) { }
		rb_const_reverse_iterator(_base_ptr ptr) : m_ptr(ptr) { }
		rb_const_reverse_iterator(const iterator &it) : m_ptr( it.m_ptr ) { }
		~rb_const_reverse_iterator() {}

		/* Accesses operators */
		reference	operator*() const {
			return m_ptr->data;
		}
		pointer		operator->() const { return &m_ptr->data; }

		/* Increment / Decrement */
		_self &operator++() {
			m_ptr = _rb_tree_decrement(m_ptr);
			return *this; 
		}

		_self operator++(int) { 
			_self tmp = *this; 
			m_ptr = _rb_tree_decrement(m_ptr); 
			return tmp; 
		}

		_self &operator--() {
			m_ptr = _rb_tree_increment(m_ptr);
			return *this;
		}
		_self operator--(int) {
			_self tmp = *this;
			m_ptr = _rb_tree_increment(m_ptr);
			return tmp;
		}

		/* Comparison operators */
		bool operator!=(_self const &it) const { return m_ptr != it.m_ptr; }
		bool operator==(_self const &it) const { return m_ptr == it.m_ptr; }
		
		_base_ptr m_ptr;
	};

	template<class Key, class T, class Compare, class Alloc>
	void swap(map<Key, T, Compare, Alloc> &x, map<Key, T, Compare, Alloc> &y){
		x.swap(y);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator==( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return x.size() == y.size() && equal(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return !(x == y);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator< ( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return !( y < x );
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator> ( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return y < x;
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=( const map<Key,T,Compare,Alloc>& x, const map<Key,T,Compare,Alloc>& y ){
		return !( x < y );
	}
}


#endif
