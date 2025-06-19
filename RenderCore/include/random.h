#pragma once

#include <random>
#include <algorithm>

#include <export.h>

class RENDER_API rng
{
public:
    template <typename Type>
    static Type generate_random_number(const Type min, const Type max) { return generate_random_number_internal(min, max, std::is_integral<Type>()); }

    template <typename T>
    static void shuffle_array(std::vector<T> &arr) { std::shuffle(arr.begin(), arr.end(), m_mt_generator); }

private:
    template <typename Type>
    static Type generate_random_number_internal(const Type min, const Type max, std::true_type)
    {
        auto distribution = std::uniform_int_distribution<Type>(min, max);
        return distribution(m_mt_generator);
    }

    template <typename Type>
    static Type generate_random_number_internal(const Type min, const Type max, std::false_type)
    {
        auto distribution = std::uniform_real_distribution<Type>(min, max);
        return distribution(m_mt_generator);
    }

    static std::mt19937 m_mt_generator;
};
