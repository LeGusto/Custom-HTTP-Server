#include <cstdint>
#include <map>
#include <list>
#include <vector>
#include <optional>

enum class SIDE
{
    ASK,
    BID
};

struct Order
{
private:
    inline static uint32_t orderID = 0;

public:
    uint32_t id = 0;
    uint32_t price;
    uint32_t customerID;
    SIDE side;

    Order(SIDE _side, uint32_t _price) : side(_side), price(_price), id(orderID++) {};
};

struct Match
{
public:
    uint32_t askID = -1;
    uint32_t bidID = -1;

    Match(uint32_t _askID, uint32_t _bidID) : askID(_askID), bidID(_bidID) {};
};

struct mapNavigation
{
    std::list<int>::iterator it;
};

class OrderBook
{
private:
    std::map<uint32_t, std::list<Order>> askMap;
    std::map<uint32_t, std::list<Order>> bidMap;

    std::map<uint32_t, mapNavigation> orderIDMap;

    std::optional<uint32_t> highest_bid();
    std::optional<uint32_t> lowest_ask();

    std::vector<Match> match_orders();

    std::vector<Match> process_order(SIDE side, uint32_t price, uint32_t customerID);
    std::optional<Order> cancel_order(uint32_t orderID);

    std::vector<Order> get_orders(uint32_t customerID);
};