#include "Offset.hpp"

using namespace std;

namespace otter {

Queue Offset::getOffset(const PlottablePtr& plottable, double distance, JoinType joinType, EndType endType)
{
    ClipperLib::JoinType clipperJoinType = convertJoinType(joinType);
    ClipperLib::EndType clipperEndType = convertEndType(endType);
    
    if (plottable->getIsClosed())
    {
        clipperEndType = ClipperLib::EndType::etClosedLine;
    }
    
    ClipperLib::ClipperOffset clipperOffset = ClipperLib::ClipperOffset();
    clipperOffset.AddPath(ClipperConvert::pathFromPlottable(plottable), clipperJoinType, clipperEndType);
    ClipperLib::Paths outPaths = {};
    clipperOffset.Execute(outPaths, ClipperConvert::toClipperCoord(distance));
    
    Queue allPaths = Queue();
    for (const ClipperLib::Path& path : outPaths)
    {
        PathPtr offsetPath = ClipperConvert::plottableFromPath(path);
        offsetPath->close();
        allPaths.add(offsetPath);
    }
    allPaths.combinePlottables();
    
    if (allPaths.items.size() == 0)
    {
        cout << "Error: No solution found for offset." << endl;
    }
    return allPaths;
}

ClipperLib::JoinType Offset::convertJoinType(JoinType joinType)
{
    switch (joinType)
    {
        case JoinType::square:
            return ClipperLib::JoinType::jtSquare;
            break;
        case JoinType::round:
            return ClipperLib::JoinType::jtRound;
            break;
        case JoinType::miter:
            return ClipperLib::JoinType::jtMiter;
            break;
            
        default:
            cout << "ERROR: invalid join type" << endl;
            return ClipperLib::JoinType::jtSquare;
            break;
    }
}

ClipperLib::EndType Offset::convertEndType(EndType endType)
{
    switch (endType)
    {
        case EndType::square:
            return ClipperLib::EndType::etOpenSquare;
            break;
        case EndType::round:
            return ClipperLib::EndType::etOpenRound;
            break;
        case EndType::butt:
            return ClipperLib::EndType::etOpenButt;
            break;
            
        default:
            cout << "ERROR: invalid end type" << endl;
            return ClipperLib::EndType::etOpenButt;
            break;
    }
}

}
