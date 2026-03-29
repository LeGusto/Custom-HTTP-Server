#include <list>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

int main()
{
    std::list<int> a = {1, 2, 3};

    int status;
    std::cout << abi::__cxa_demangle(typeid(a.begin()).name(), nullptr, nullptr, &status);
}