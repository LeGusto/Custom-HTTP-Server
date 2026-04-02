#include <cstdint>
#include <algorithm>
#include <iostream>

uint64_t pack754(long double f, uint32_t bits, uint32_t exp_bits)
{
    if (f == 0)
        return 0;
    uint32_t bias = (1 << (exp_bits - 1)) - 1;
    uint64_t sign = (f < 0);

    long double f_tmp = std::abs(f);

    int32_t exp = 0;
    while (f_tmp >= 2)
    {
        f_tmp /= 2;
        exp++;
    }
    while (f_tmp < 1)
    {
        f_tmp *= 2;
        exp--;
    }

    f_tmp -= 1;

    uint64_t significand_bits = bits - exp_bits - 1;
    long double scaled_f = f_tmp * (1LL << significand_bits);     // convert to long long
    uint64_t significand = static_cast<uint64_t>(scaled_f + 0.5); // rounds to the nearest int

    exp += bias;

    uint64_t ans = (sign << (bits - 1));
    ans |= (static_cast<uint64_t>(exp) << (significand_bits));
    ans |= significand;

    return ans;
}

int main()
{
    std::cout << std::hex << pack754(1.5, 64, 11) << "\n";
}