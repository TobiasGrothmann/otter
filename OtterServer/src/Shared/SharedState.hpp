#pragma once
#include "stl.hpp"
#include "ReceivedCommand.hpp"

namespace otter {

struct SharedState
{
    SharedState()
        : abort(false)
    {};

    std::queue<std::shared_ptr<ReceivedCommand>> commands;

    std::atomic<bool> abort;
};

}
