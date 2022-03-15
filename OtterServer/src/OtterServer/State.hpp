#pragma once
#include "stl.hpp"

#include "Otter.hpp"
#include "Accelleration.hpp"
#include "Settings.hpp"

namespace otter {

enum class PlotterState
{
	Idle,
	Running,
	Pausing,
};

struct QueueState
{
	QueueState(const std::shared_ptr<Queue>& queue, const QueueSettings* settings)
	{
		plottables = {};
		plottables.reserve(queue->items.size());
		for (const PlottablePtr& plottable : queue->items)
		{
			plottables.push_back(plottable);
		}
		switchToNextPlottable(Vec2(0, 0), settings, true /* only for initialization, will not jump to second plottable */);
	}
	~QueueState()
	{
		if (currentAccPath) delete currentAccPath;
	}
private:

	std::vector<PlottablePtr> plottables = {};
	AccellerationPath* currentAccPath = nullptr;
	VINDEX(plottables) currentPlottableIndex = 0;
	VINDEX_TYPE(Vec2) currentPointIndex = 0;

public:
	bool isDone() const
	{
		return currentPlottableIndex >= plottables.size();
	}
	void switchToNextPlottable(const Vec2& computeFrom, const QueueSettings* settings, bool onlyInit = false)
	{
		if (!onlyInit)
			currentPlottableIndex++;
		currentPointIndex = 0;
		if (currentAccPath)
		{
			delete currentAccPath;
			currentAccPath = nullptr;
		}

		if (currentPlottableIndex < plottables.size())
		{
			const std::vector<Vec2>& points = plottables[currentPlottableIndex]->getPlottablePoints(SampleSettings(30.0f, computeFrom));
			currentAccPath = new AccellerationPath(
				points,
				settings->edgeSlowdownPow, settings->accellerationDistDraw
			);
		}
	}
	void setCurrentPointIndex(VINDEX_TYPE(Vec2) index)
	{
		currentPointIndex = index;
	}
	const AccellerationPath* getCurrentAccellerationPath() const
	{
		return currentAccPath;
	}
	Queue getLeftToDo() const
	{
		Queue leftToDo = Queue();

		PathPtr abortedPath = Path::create();
		for (VINDEX(currentAccPath->points) i = currentPointIndex; i < currentAccPath->points.size(); i++)
		{
			abortedPath->add(currentAccPath->points[i].point);
		}
		if (abortedPath->points.size() > 2)
		{
			leftToDo.add(abortedPath);
		}

		for (VINDEX(plottables) i = currentPlottableIndex + 1; i < plottables.size(); i++)
		{
			leftToDo.add(plottables[i]);
		}
		return leftToDo;
	}
};

struct State
{
	int x = 0;
	int y = 0;

	bool headDown = false;
	bool enabled = false;

	std::atomic<PlotterState> plotterState = { PlotterState::Idle };

	QueueState* queueState = nullptr;

	boost::thread* runThread = nullptr;
	std::atomic<bool> threadRunning = { false };
	std::atomic<bool> shouldStopRunThread = { false };

	void startNewQueue(const std::shared_ptr<Queue>& queue, const QueueSettings* settings)
	{
		if (this->queueState) delete this->queueState;
		this->queueState = new QueueState(queue, settings);
	}
	Vec2 getCurrentVec()
	{
		return Vec2(x * distPerStepAxis, y * distPerStepAxis);
	}
};

}
