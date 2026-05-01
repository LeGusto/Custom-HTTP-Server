#include "latency.h"

#include <algorithm>
#include <ctime>
#include <execution>
#include <filesystem>
#include <format>
#include <fstream>
#include <numeric>
#include <print>

#ifndef PROJECT_ROOT
#define PROJECT_ROOT "."
#endif

LatencyTracker::LatencyTracker(size_t _n) : n(_n)
{
    entries.reserve(n);
}

void LatencyTracker::insert_entry(std::string_view op_name, uint64_t ns)
{
    if (entries.size() < n)
        entries.emplace_back(op_name, ns);
}

long double LatencyTracker::avg(std::vector<Entry> &v)
{
    if (v.empty())
        return 0;
    uint64_t tot = std::transform_reduce(
        std::execution::par_unseq,
        v.begin(), v.end(),
        uint64_t{0},
        std::plus<>{},
        [](const Entry &e) { return e.ns; });
    return static_cast<long double>(tot) / v.size();
}

uint64_t LatencyTracker::min(std::vector<Entry> &v)
{
    if (v.empty())
        return 0;
    return v.front().ns;
}

uint64_t LatencyTracker::max(std::vector<Entry> &v)
{
    if (v.empty())
        return 0;
    return v.back().ns;
}

uint64_t LatencyTracker::pxx(long double percentile, std::vector<Entry> &v)
{
    if (v.empty())
        return 0;
    long double div = (v.size() * percentile) / 100;
    size_t idx = static_cast<size_t>(div);
    if (idx >= v.size())
        idx = v.size() - 1;
    return v[idx].ns;
}

void LatencyTracker::partition_entries()
{
    for (auto &e : entries)
        buckets[e.op_name].push_back(e);
}

void LatencyTracker::print_entries(std::ofstream &out, std::vector<Entry> &v)
{
    std::print(out,
               "Average: {}\n"
               "Min: {}\n"
               "Max: {}\n"
               "P50: {}\n"
               "P95: {}\n"
               "P99: {}\n"
               "P99.99: {}\n\n",
               avg(v), min(v), max(v),
               pxx(50, v), pxx(95, v), pxx(99, v), pxx(99.99, v));
}

void LatencyTracker::print_buckets(std::ofstream &out)
{
    for (auto &[name, vec] : buckets)
    {
        std::println(out, "==========={}===========", name);
        print_entries(out, vec);
    }
}

void LatencyTracker::dump_entries()
{
    std::sort(entries.begin(), entries.end(),
              [](const Entry &a, const Entry &b) { return a.ns < b.ns; });

    std::time_t t = std::time(nullptr);
    char buf[32];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t));

    std::filesystem::path log_dir = std::filesystem::path(PROJECT_ROOT) / "logs";
    std::filesystem::create_directory(log_dir);
    std::ofstream out(log_dir / std::format("bench {}.txt", buf));
    if (!out)
        return;

    partition_entries();
    std::println(out, "===========GLOBAL===========");
    print_entries(out, entries);
    print_buckets(out);
}
