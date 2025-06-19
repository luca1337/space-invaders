#include <random.h>

std::mt19937 rng::m_mt_generator = std::mt19937(std::random_device{}());