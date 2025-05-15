#include <iostream>
#include "MyVector.hpp"
 
int main()
{
    try
    {
        MyVector<int> arr;
        arr.print();
        std::cout << std::endl;
 
        arr.push_back(4);
        arr.push_back(5);
        arr.insert(7,1);
        arr.insert(9, 1);
        arr.print();
        std::cout << std::endl;
        arr.push_back(6);
        arr.print();
        std::cout << std::endl;
        int b = arr.pop_back();
        arr.print();
        std::cout << std::endl;
 
        std::cout << "Deleted element: " << b << std::endl;
        int a = arr[1];
        std::cout << a;
        arr.pop_back();arr.pop_back();arr.pop_back();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
 
}