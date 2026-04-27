#include "latency_tracker.h"
#include "enum_name.h"
#include <algorithm>
#include <print>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>

#ifndef PROJECT_ROOT
#define PROJECT_ROOT "."
#endif

LatencyTracker::LatencyTracker()
{
    global_samples.reserve(SAMPLE_CAP);
    for (auto t : {MessageType::SUBMIT_ORDER, MessageType::CANCEL_ORDER, MessageType::GET_ORDERS})
        by_type[static_cast<uint8_t>(t)].reserve(SAMPLE_CAP);
}

void LatencyTracker::record(MessageType type, uint64_t ns)
{
    if (global_samples.size() < global_samples.capacity())
        global_samples.push_back(ns);
    auto &bucket = by_type[static_cast<uint8_t>(type)];
    if (bucket.size() < bucket.capacity())
        bucket.push_back(ns);
}

static void report_bucket(std::vector<uint64_t> &samples, std::string_view label,
                          std::ostream *file_out, const char *ts)
{
    if (samples.empty())
        return;
    std::sort(samples.begin(), samples.end());
    auto pct = [&](double p)
    {
        size_t idx = static_cast<size_t>(samples.size() * p);
        if (idx >= samples.size())
            idx = samples.size() - 1;
        return samples[idx];
    };
    std::print("[bench {}] count={} min={}ns p50={}ns p95={}ns p99={}ns p99.9={}ns max={}ns\n",
               label, samples.size(), samples.front(),
               pct(0.50), pct(0.95), pct(0.99), pct(0.999), samples.back());
    if (file_out)
        std::print(*file_out, "{} {} count={} min={} p50={} p95={} p99={} p99.9={} max={}\n",
                   ts, label, samples.size(), samples.front(),
                   pct(0.50), pct(0.95), pct(0.99), pct(0.999), samples.back());
}

void LatencyTracker::dump()
{
    if (global_samples.empty())
    {
        std::print("[bench] no samples collected\n");
        return;
    }

    std::filesystem::path log_dir = std::filesystem::path(PROJECT_ROOT) / "logs";
    std::filesystem::create_directory(log_dir);
    std::ofstream out(log_dir / "bench.txt", std::ios::app);
    std::ostream *file_out = out ? &out : nullptr;

    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char ts[32];
    std::strftime(ts, sizeof(ts), "%Y-%m-%d_%H:%M:%S", std::localtime(&now));

    report_bucket(global_samples, "global", file_out, ts);
    for (size_t i = 0; i < by_type.size(); i++)
    {
        if (by_type[i].empty())
            continue;
        report_bucket(by_type[i],
                      enum_name(static_cast<MessageType>(i)),
                      file_out, ts);
    }
}
