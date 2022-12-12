#include <iostream>
#include <vector>

int main ()
{
//    std::vector<int> a;
    std::vector<int>::iterator it;
//
//    it = a.begin();
//    it = a.insert ( it , 10, 200 );
//
//    std::vector<int> b;
//    b.push_back(1);
//    b.push_back(2);
//
//    a.insert(it + 5, b.begin(), b.end());
//    int arr[] = {4,5,6};
//    a.insert(a.begin(), arr, arr + 3);
//    for (it = a.begin(); it<a.end(); it++)
//        std::cout << " " << *it;
//    std::cout << "\n\n\n";

    std::vector<int> test;

    test.push_back(1);
    test.push_back(2);
    test.push_back(3);

    std::size_t b = std::distance(test.begin(), test.begin() + 1);
    std::size_t e = std::distance(test.begin(), test.begin() + 2);

    std::size_t size = test.size() - (e - b);
    std::cout << "Size: " << size << "\n";


    for (it = test.begin(); it<test.end(); it++)
        std::cout << " " << *it;
    std::cout << "\n";
    test.erase(test.begin(), test.begin() + 2);
    for (it = test.begin(); it<test.end(); it++)
        std::cout << " " << *it;
    std::cout << test.size();

//    std::int32_t diff = std::distance(test.begin(), test.begin() + 1);
//    std::size_t index = diff;
//
//    for (it = test.begin(); it<test.end(); it++)
//        std::cout << " " << *it;
//
//    for (; index < 3 - 1; index++)
//        test[index] = test[index + 1];
//
//    for (it = test.begin(); it<test.end(); it++)
//        std::cout << " " << *it;
//
//    test.erase(test.begin()+index);
//    std::cout << "\nresult: ";
//    for (it = test.begin(); it<test.end(); it++)
//        std::cout << " " << *it;
//

//
//    for (it = test.begin(); it<test.end(); it++)
//        std::cout << " " << *it;
//    std::cout << test.capacity() << "   " << test.size() << "\n";
//    std::cout << "\n\nc:";
//    std::size_t c = test.begin() - test1.begin();
//    std::cout << c;

    return 0;
}
