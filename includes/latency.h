#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include <iosfwd>
#include <cstdint>

struct Entry
{
    std::string op_name;
    uint64_t ns = 0;

    Entry(std::string_view _op_name, uint64_t _ns)
        : op_name(_op_name), ns(_ns) {}
};

class LatencyTracker
{
private:
    std::vector<Entry> entries;
    size_t n = 0;
    std::unordered_map<std::string, std::vector<Entry>> buckets;

    long double avg(std::vector<Entry> &v);
    uint64_t min(std::vector<Entry> &v);
    uint64_t max(std::vector<Entry> &v);
    uint64_t pxx(long double percentile, std::vector<Entry> &v);

    void partition_entries();
    void print_entries(std::ofstream &out, std::vector<Entry> &v);
    void print_buckets(std::ofstream &out);

public:
    explicit LatencyTracker(size_t _n);

    void insert_entry(std::string_view op_name, uint64_t ns);
    void dump_entries();
};
