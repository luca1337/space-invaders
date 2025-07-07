#include <Random.h>

std::mt19937 Rng::m_mt_generator = std::mt19937(std::random_device{}());