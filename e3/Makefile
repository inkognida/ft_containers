CXX = c++
CXXFLAGS = -Wall -Werror -Wextra -std=c++98

SRCS = main.cpp
HEADERS =	vector.hpp \
			stack.hpp \
			map.hpp \
			./utils/functional.hpp \
			./utils/iterator.hpp \
			./utils/RB_tree.hpp \
			./utils/algorithm.hpp \
			./utils/tree_iterator.hpp \
			./utils/type_traits.hpp \
			./utils/utility.hpp

.PHONY: re clean fclean

STL_NAME = stl_containers
FT_NAME = ft_containers

 all: $(STL_NAME) $(FT_NAME)

 $(STL_NAME): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(STL_NAME) $(SRCS)

$(FT_NAME): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(FT_NAME) -DFT $(SRCS)

re: fclean all

clean: fclean

fclean:
	rm -f $(STL_NAME)
	rm -f $(FT_NAME)