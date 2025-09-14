#pragma once
#include "SDL.h"
#include "planner.h"
#include <atomic>

namespace AI{
    bool run(Planner &planner); //std::atomic<bool> &shutdown_flag
};
