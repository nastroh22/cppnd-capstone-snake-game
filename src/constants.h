#pragma once

#include <cstddef>

// constexpr std::size_t kFramesPerSecond{60};
constexpr std::size_t kFramesPerSecond{60}; // For Debugging
constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
constexpr std::size_t kScreenWidth{640};
constexpr std::size_t kScreenHeight{640};
constexpr std::size_t kGridWidth{32};
constexpr std::size_t kGridHeight{32};