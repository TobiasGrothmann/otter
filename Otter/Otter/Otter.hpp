#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

// constants
#define M_GR 1.6180339887498948482

// includes
#include "Queue/Queue.hpp"

#include "Geometry/Vec2.hpp"
#include "Geometry/Vec2i.hpp"
#include "Geometry/Line.hpp"

#include "Plottables/Plottable.hpp"
#include "Plottables/Circle.hpp"
#include "Plottables/Path.hpp"
#include "Plottables/Rectangle.hpp"
#include "Plottables/Bezier.hpp"

namespace otter {
using PlottablePtr = std::shared_ptr<otter::Plottable>;
using CirclePtr = std::shared_ptr<otter::Circle>;
using PathPtr = std::shared_ptr<otter::Path>;
using RectanglePtr = std::shared_ptr<otter::Rectangle>;
using BezierPtr = std::shared_ptr<otter::Bezier>;
}

#include "Structures/Frame.hpp"
#include "Structures/Grid.hpp"

#include "Math/Math.hpp"
#include "Math/Geometry.hpp"

#include "Rand/Rand.hpp"

#include "Util/Distortion/Distortion.hpp"
#include "Util/Distortion/BarrelDistortion.hpp"
#include "Util/Distortion/DistortionAroundCenter.hpp"
#include "Util/Pattern/GeneralPattern.hpp"
#include "Util/Pattern/Patterns/ParallelLines.hpp"
#include "Util/Pattern/Patterns/ParallelGrid.hpp"
#include "Util/Pattern/Patterns/ConcentricCircles.hpp"
#include "Util/Pattern/Patterns/SpiralPoints.hpp"
#include "Util/Pattern/Patterns/SpiralLine.hpp"
#include "Util/Pattern/Patterns/ConcentricSquares.hpp"
#include "Util/Pattern/Patterns/HexGrid.hpp"
#include "Util/Pattern/Patterns/RandomDots.hpp"
#include "Util/Pattern/GeneralPointDistribution.hpp"
#include "Util/Pattern/PointDistributions/HexGridVertices.hpp"
#include "Util/QuadTree/QuadTree.hpp"
#include "Util/SampleFunction/SampleFunction.hpp"
#include "Util/FlowField/FlowField.hpp"
#include "Util/Filling/Filling.hpp"

#include "Serialize/Serializable.hpp"
#include "Serialize/SerializableFactory.hpp"

#include "Queue/Command/Command.hpp"

#pragma GCC visibility pop
