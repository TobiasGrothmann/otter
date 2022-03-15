#pragma once
#include "../../Queue/Queue.hpp"
#include "../../Plottables/Plottable.hpp"

namespace otter {

class Filling
{
public:
    static Queue getSimpleFilling(const std::shared_ptr<Plottable>& shape, double lineDist);
};

}
