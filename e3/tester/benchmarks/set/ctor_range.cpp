#include "set_prelude.hpp"

int main()
{
    SETUP;

    NAMESPACE::vector<int> data;

    for (std::size_t i = 0; i < MAXSIZE; ++i) {
        data.push_back(rand());
    }

    timer t;

    for (int i = 0; i < 5; ++i) {
        NAMESPACE::set<int> s(data.begin(), data.begin() + 2500000);
    }

    PRINT_TIME(t);
}
