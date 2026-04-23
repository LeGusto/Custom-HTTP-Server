#pragma once

#include <cstdint>
#include <vector>
#include <array>
#include "protocol.h"

class LatencyTracker
{
public:
    static constexpr size_t SAMPLE_CAP = 1'000'000;

    LatencyTracker();

    void record(MessageType type, uint64_t ns);

    void dump();

private:
    std::vector<uint64_t> global_samples;
    std::array<std::vector<uint64_t>, 256> by_type{};
};
