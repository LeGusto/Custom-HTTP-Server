#include "order_book.h"
#include "serializer.cpp"

int main()
{
    std::string a = "";
    Order order(SIDE::ASK, 10, 100, 1);
    pack(a, order);
    for (unsigned char c : a)
    {
        printf("%02x ", c);
    }
    printf("\n");
}