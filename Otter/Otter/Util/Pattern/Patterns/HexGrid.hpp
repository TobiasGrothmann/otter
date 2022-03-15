#pragma once
#include "../GeneralPattern.hpp"

namespace otter {

class HexGrid : public GeneralPattern
{
public:
    HexGrid(double lineDist);
    
    virtual Queue create(const std::shared_ptr<Rectangle>& bounds) const override;
    
    double lineDist;
};

}
