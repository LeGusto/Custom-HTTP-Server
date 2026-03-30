#include "order_book.h"
#include <cassert>
#include <iostream>

int main()
{
    OrderBook book;
    assert(!book.highest_bid().has_value());
    assert(!book.lowest_ask().has_value());

    book.process_order(SIDE::ASK, 10, 100, 1);
    assert(book.lowest_ask().has_value());
    assert(book.lowest_ask()->price == 100);

    book.process_order(SIDE::BID, 1, 100, 1);
    std::cout << book.lowest_ask()->quantity << "\n";

    assert(book.lowest_ask()->quantity == 9);
    assert(!book.highest_bid().has_value());
    std::cout << "All tests passed!\n";

    return 0;
}