#include "OtterServer.hpp"
#include <boost/filesystem.hpp>
#include <cstdlib>

using namespace std;

namespace otter {

OtterServer::OtterServer(SharedState* sharedState)
	: sharedState(sharedState)
{
	setup();
	setDir(Axis::X, 0);
	setDir(Axis::Y, 0);
	setDir(Axis::Z, 0);
	enable(false);
	Util::sleepSeconds(0.05f);
	updateLastStepToNow(Axis::X);
	updateLastStepToNow(Axis::Y);
	updateLastStepToNow(Axis::Z);

	playSound(Sound::Startup);

	cout << "data dir is: " << dataDir << endl;
	system(("mkdir -p " + dataDir).c_str());
}
OtterServer::~OtterServer()
{
	enable(false);
	cleanup();
}

void OtterServer::runServer()
{
	while (!sharedState->abort)
	{
		usleep(1000);
		handleCommandQueue();
	}
}

void OtterServer::handleCommandQueue()
{
	while (!sharedState->commands.empty())
	{
		shared_ptr<ReceivedCommand> receivedCommand = sharedState->commands.front();
		cout << "  received command: " << receivedCommand->command->verb << " (expectsReturn: " << receivedCommand->command->expectsReturn << ")" << endl;

		const string& answer = handleCommand(receivedCommand->command);

		if (receivedCommand->command->expectsReturn)
		{
			if (answer.empty())
			{
				cout << "WARNING: trying to answer, but answer is empty" << endl;
			}
			else
			{
				answerToSocket(receivedCommand->socket, answer);
			}
		}
		else if (!answer.empty())
		{
			cout << "WARNING: execution of command returned an answer, but command does not expect an answer" << endl;
		}

		sharedState->commands.pop();
	}
}

string OtterServer::handleCommand(shared_ptr<Command> command)
{
	if (command->verb == "run")
	{
		if (state.plotterState != PlotterState::Idle)
		{
			cout << "Error: Cannot run another Queue while not idling" << endl;
			return string();
		}
		shared_ptr<Queue> queue = dynamic_pointer_cast<Queue>(SerializableFactory::createPtr(command->data));
		runQueueAsync(queue);
	}
	else if (command->verb == "pause")
	{
		if (state.plotterState != PlotterState::Running)
		{
			cout << "WARNING: Cannot pause while not running." << endl;
			return string();
		}
		pauseQueue();
	}
	else if (command->verb == "resume")
	{
		if (state.plotterState != PlotterState::Pausing && state.plotterState != PlotterState::Idle)
		{
			cout << "WARNING: Cannot resume while not pausing or idling." << endl;
			return string();
		}
		resumeQueue();
	}
	else if (command->verb == "abort")
	{
		if (state.plotterState != PlotterState::Running && state.plotterState != PlotterState::Pausing)
		{
			cout << "WARNING: Cannot abort while not running or paused." << endl;
			return string();
		}
		abortQueue();
	}

	return string();
}

void OtterServer::answerToSocket(tcp::socket* socket, const string& message)
{
	if (!socket->is_open())
	{
		cout << "Error: socket is already closed. Cannot answer." << endl;
		return;
	}
    const string msg = message + COMMAND_MESSAGE_END;
    boost::asio::write(*socket, boost::asio::buffer(message));
}


void OtterServer::setup()
{
#if WITH_GPIO
	// setup pins
	for (const char& pin : dirPins)
	{
		GPIOExport(pin);
		GPIODirection(pin, OUT);
		GPIOWrite(pin, LOW);
	}
	for (const char& pin : stepPins)
	{
		GPIOExport(pin);
		GPIODirection(pin, OUT);
		GPIOWrite(pin, LOW);
	}
	for (const char& pin : enablePins)
	{
		GPIOExport(pin);
		GPIODirection(pin, OUT);
		GPIOWrite(pin, LOW);
	}

	// setup microstepping
	for (char motorI = 0; motorI < 4; motorI++)
	{
		const char ms1 = ms1Pins[motorI];
		GPIOExport(ms1);
		GPIODirection(ms1, OUT);
		GPIOWrite(ms1, msValues[motorI][0]);

		const char ms2 = ms2Pins[motorI];
		GPIOExport(ms2);
		GPIODirection(ms2, OUT);
		GPIOWrite(ms2, msValues[motorI][1]);

		const char ms3 = ms3Pins[motorI];
		GPIOExport(ms3);
		GPIODirection(ms3, OUT);
		GPIOWrite(ms3, msValues[motorI][2]);
	}
#endif
}

void OtterServer::cleanup()
{
#if WITH_GPIO
	for (const char& pin : dirPins)
	{
		GPIOUnexport(pin);
	}
	for (const char& pin : stepPins)
	{
		GPIOUnexport(pin);
	}
	for (const char& pin : enablePins)
	{
		GPIOUnexport(pin);
	}

	for (const char& pin : ms1Pins)
	{
		GPIOUnexport(pin);
	}
	for (const char& pin : ms2Pins)
	{
		GPIOUnexport(pin);
	}
	for (const char& pin : ms3Pins)
	{
		GPIOUnexport(pin);
	}
#endif
}

void OtterServer::updateLastStepToNow(Axis axis)
{
	lastStep[axis] = std::chrono::high_resolution_clock::now();
}

long long OtterServer::getMicrosSinceLastStep(Axis axis)
{
	std::chrono::nanoseconds elapsed = std::chrono::high_resolution_clock::now() - lastStep[axis];
	return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
}

bool OtterServer::isLeft(float ax, float ay, float bx, float by, float cx, float cy)
{
	return ((bx - ax)*(cy - ay) - (by - ay)*(cx - ax)) > 0;
}




void OtterServer::setHead(bool value)
{
	if (state.headDown == value) return;

	setDir(Axis::Z, value ? 1 : 0);
	const float headTravel = headTravelToTouch + maxExtraHeadTravel * settings->penPressure;
	const uint stepsTotal = (uint)round(headTravel / (float) distPerStepHead);
	for (uint i = 0; i < stepsTotal; i++)
	{
		const float factor = (float)i / stepsTotal;
		const float speedFactor = fmin(fmin(factor / settings->accellerationDistFactorHead, (1.0f - factor) / settings->accellerationDistFactorHead), 1.0f);
		speedHead->setSpeedFactor(speedFactor);
		stepWithDelay(Axis::Z, speedHead);
	}
	state.headDown = value;
}

void OtterServer::enable(bool newEnableValue)
{
#if WITH_GPIO
	for (const char& pin : enablePins)
	{
		GPIODirection(pin, newEnableValue ? HIGH : LOW);
	}
#endif
	state.enabled = newEnableValue;
}


void OtterServer::step([[maybe_unused]] Axis axis)
{
#if WITH_GPIO
	switch (axis)
	{
		case Axis::X:
			GPIOWrite(stepPins[2], 0);
			usleep(1);
			GPIOWrite(stepPins[2], 1);
			break;
		case Axis::Y:
			GPIOWrite(stepPins[0], 0);
			GPIOWrite(stepPins[1], 0);
			usleep(1);
			GPIOWrite(stepPins[0], 1);
			GPIOWrite(stepPins[1], 1);
			break;
		case Axis::Z:
			GPIOWrite(stepPins[3], 0);
			usleep(1);
			GPIOWrite(stepPins[3], 1);
			break;
	}
#endif
}

void OtterServer::stepWithDelay(Axis axis, SpeedDelayHandler* speedDelayHandler)
{
	const long long microsToWait = (long long)round(speedDelayHandler->getDelay());
	while (getMicrosSinceLastStep(axis) <= microsToWait)
	{
	}

	step(axis);
	updateLastStepToNow(axis);
}

void OtterServer::setDir([[maybe_unused]] Axis axis, [[maybe_unused]] int direction)
{
#if WITH_GPIO
	switch (axis)
	{
	case Axis::X:
		GPIOWrite(dirPins[2], dirVal[2][direction]);
		break;
	case Axis::Y:
		GPIOWrite(dirPins[0], dirVal[0][direction]);
		GPIOWrite(dirPins[1], dirVal[1][direction]);
		break;
	case Axis::Z:
		GPIOWrite(dirPins[3], dirVal[3][direction]);
		break;
	}
#endif
}

void OtterServer::playChord(const vector<float>& midis, float duration)
{
	vector<boost::thread> threads = {};
	threads.reserve(midis.size());
	const vector<Axis> axises = {Axis::Y, Axis::X, Axis::Z};
	for (VINDEX(midis) i = 0; i < midis.size(); i++)
	{
		if (i >= 3) break;
		float midi = midis[i];
		Axis axis = axises[i];
		threads.push_back(boost::thread([this, axis, midi, duration]()
		{
			playNote(axis, midi, duration);
		}));
	}
	for (boost::thread& thread : threads)
	{
		thread.join();
	}
}

void OtterServer::playNote(Axis axis, float midi, float duration)
{
	float freq = pow(2.0f, (midi - 69.0f) / 12.0f) * 440.0f;
	playFreq(axis, freq, duration);
}

void OtterServer::playFreq(Axis axis, float freq, float duration)
{
	float factor = 1000.0f * 1000.0f;
	const uint delay = (uint)round(factor / freq);
	uint iterations = (uint)round((duration * factor) / delay);
	if (iterations % 2 != 0) // end up at same location as start
	{
		iterations += 1;
	}

	int dir = 0;
	for (uint i = 0; i < iterations; i++)
	{
		while (getMicrosSinceLastStep(axis) <= delay)
		{
		}

		updateLastStepToNow(axis);

		setDir(axis, dir);
		step(axis);
		dir = dir == 0 ? 1 : 0;
	}
}

void OtterServer::runQueueAsync(const shared_ptr<Queue>& queue)
{
	if (state.runThread && state.threadRunning)
	{
		cout << "WARNING: Failed to run queue - already running!" << endl;
		return;
	}
	state.plotterState = PlotterState::Running;
	state.runThread = new boost::thread([this, queue]()
	{
		state.threadRunning = true;
		runQueue(queue);
		state.threadRunning = false;
	});
}

void OtterServer::stopQueueThread()
{
	if (state.runThread)
	{
		state.shouldStopRunThread = true;
		state.runThread->join();
		delete state.runThread;
		state.runThread = nullptr;
	}
	state.shouldStopRunThread = false;
}

void OtterServer::runQueue(const shared_ptr<Queue>& queue)
{
	playSound(Sound::Run);

	deletePauseFile();

	setupSettings(queue->settings);
	state.startNewQueue(queue, settings);

	// dequeue
	enable(true);
	while (!state.queueState->isDone())
	{
		if (state.shouldStopRunThread) return; // pausing or aborting

		const AccellerationPath* accPath = state.queueState->getCurrentAccellerationPath();
		if (accPath->points.size() >= 2)
		{
			travelToWithAccelleration(accPath->points[0].point);

			setHead(1);
			for (VINDEX(accPath->points) i = 1; i < accPath->points.size(); i++)
			{
#if !WITH_GPIO
				cout << accPath->points[i].point << endl;
#endif
				moveTo(accPath->points[i - 1].speed, accPath->points[i], speedDraw);
				state.queueState->setCurrentPointIndex(i);
				if (state.shouldStopRunThread) // pausing or aborting
				{
					setHead(0);
					return;
				}
			}
			setHead(0);
		}
		state.queueState->switchToNextPlottable(state.getCurrentVec(), settings);
    }

	// finished all items in queue
    travelToWithAccelleration(Vec2(0, 0));
	playSound(Sound::Success);
    enable(false);
	state.plotterState = PlotterState::Idle;
}

void OtterServer::setupSettings(const QueueSettings& newSettings)
{
	settings = &newSettings;
	if (speedDraw) delete speedDraw;
	if (speedTravel) delete speedTravel;
	if (speedHead) delete speedHead;
	speedDraw = new SpeedDelayHandler(settings->speedDrawMin, settings->speedDrawMax, distPerStepAxis);
	speedTravel = new SpeedDelayHandler(settings->speedTravelMin, settings->speedTravelMax, distPerStepAxis);
	speedHead = new SpeedDelayHandler(settings->speedHeadMin, settings->speedHeadMax, distPerStepHead);
}

void OtterServer::pauseQueue()
{
	stopQueueThread();

	playSound(Sound::Pause);

	const Queue leftToDo = state.queueState->getLeftToDo();
	leftToDo.save(dataLeftAfterPausePath);

	travelToWithAccelleration(Vec2(0, 0));
	enable(false);
	state.plotterState = PlotterState::Pausing;
}

void OtterServer::resumeQueue()
{
	shared_ptr<Queue> queue = nullptr;
	if (!getPausedQueue(queue))
	{
		cout << "WARNING: tried to resume but paused queue file is invalid." << endl;
		return;
	}

	runQueueAsync(queue);
}

bool OtterServer::getPausedQueue(shared_ptr<Queue>& outQueue)
{
	if (!boost::filesystem::exists(dataLeftAfterPausePath)) return false;
	outQueue = make_shared<Queue>(Queue::load(dataLeftAfterPausePath));
	return true;
}

void OtterServer::deletePauseFile()
{
	if (!boost::filesystem::exists(dataLeftAfterPausePath)) return;
	boost::filesystem::remove(dataLeftAfterPausePath);
}

void OtterServer::abortQueue()
{
	if (state.plotterState == PlotterState::Pausing)
	{
		deletePauseFile();
		playSound(Sound::Abort);
	}
	else
	{
		stopQueueThread();

		playSound(Sound::Abort);
		travelToWithAccelleration(Vec2(0, 0));
	}
	enable(false);
	state.plotterState = PlotterState::Idle;
}

void OtterServer::travelToWithAccelleration(const Vec2& to)
{
	setHead(0);
	AccellerationPath accPath = AccellerationPath(
		{ state.getCurrentVec(), to },
		settings->edgeSlowdownPow, settings->accellerationDistTravel
	);
	for (VINDEX(accPath.points) i = 1; i < accPath.points.size(); i++)
	{
		moveTo(accPath.points[i - 1].speed, accPath.points[i], speedTravel);
	}
}

void OtterServer::moveTo(float fromSpeed, const SpeedPoint& to, SpeedDelayHandler* speedDelayHandler)
{
	const float xDelta = ((float)to.point.x / distPerStepAxis) - (float)state.x;
	const float yDelta = ((float)to.point.y / distPerStepAxis) - (float)state.y;
	move((int)round(xDelta), (int)round(yDelta), fromSpeed, to.speed, speedDelayHandler);
}

void OtterServer::move(int x, int y, float speedStart, float speedEnd, SpeedDelayHandler* speedDelayHandler)
{
	setDir(Axis::X, x > 0 ? 1 : 0);
	setDir(Axis::Y, y > 0 ? 1 : 0);

	const int dirToAddX = x > 0 ? 1 : -1;
	const int dirToAddY = y > 0 ? 1 : -1;

	long steppedX = 0;
	long steppedY = 0;

	const long xAbs = abs(x);
	const long yAbs = abs(y);

	const float totalStepsNeeded = (float) xAbs + (float) yAbs;

	speedDelayHandler->setSpeedFactor(speedStart);

	while (steppedX < xAbs || steppedY < yAbs)
	{
		const float factor = (steppedX + steppedY) / totalStepsNeeded;
		speedDelayHandler->setSpeedFactor(speedStart + (speedEnd - speedStart) * factor);
		if (isLeft(0.0f, 0.0f, xAbs, yAbs, steppedX, steppedY))
		{
			steppedX += 1;
			state.x += dirToAddX;
			stepWithDelay(Axis::X, speedDelayHandler);
		}
		else
		{
			steppedY += 1;
			state.y += dirToAddY;
			stepWithDelay(Axis::Y, speedDelayHandler);
		}
	}
}

void OtterServer::playSound(Sound sound)
{
	bool wasEnabled = state.enabled;
	if (!state.enabled)
	{
		enable(true);
	}

	switch (sound) {
		case Sound::Startup:
		{
			const float pause = 0.02f;
			const float note = 0.17f;
			playChord({60, 64, 67}, note);
			Util::sleepSeconds(pause);
			playChord({60, 65, 69}, note);
			Util::sleepSeconds(pause);
			playChord({62, 65, 71}, note);
			Util::sleepSeconds(pause);
			playChord({64, 67, 72}, note * 2.0f);
			playChord({60, 67, 72}, note * 2.0f);
		}
			break;
		case Sound::Run:
		{
			const float soundDur = 0.15f;
			playChord({84}, soundDur);
			Util::sleepSeconds(soundDur * 0.25f);
			playChord({84, 72+7, 84+12}, soundDur);
			Util::sleepSeconds(0.2f);
		}
			break;
		case Sound::Pause:
			playChord({60, 84}, 0.1f);
			Util::sleepSeconds(0.05f);
			playChord({60, 84+12}, 0.1f);
			break;
		case Sound::Abort:
			playChord({60+24, 66+24, 60+36}, 1.5f);
			break;
		case Sound::Success:
		{
			const float soundDur = 0.15f;
			Util::sleepSeconds(0.2f);
			playChord({60}, soundDur);
			playChord({60, 64}, soundDur);
			playChord({60, 67}, soundDur);
			playChord({48, 64, 72}, soundDur * 2.0f);
		}
			break;
	}

	if (!wasEnabled)
	{
		enable(false);
	}
}

}
