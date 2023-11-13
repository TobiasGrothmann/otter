#pragma once
#include "stl.hpp"

#if WITH_GPIO
#include "gpio.hpp"
#endif

#include "Otter.hpp"
#include "State.hpp"
#include "Settings.hpp"
#include "Accelleration.hpp"
#include "SpeedDelayHandler.hpp"
#include "../Shared/Util.hpp"
#include "../Shared/SharedState.hpp"
#include "../Shared/ReceivedCommand.hpp"

namespace otter {

enum Axis
{
	X = 0,
	Y = 1,
	Z = 2
};

class OtterServer
{
public:
	OtterServer(SharedState* sharedState);
	~OtterServer();

	State state;

	void setupSettings(const QueueSettings& newSettings);
	const QueueSettings* settings = nullptr;
	SpeedDelayHandler* speedDraw = nullptr;
	SpeedDelayHandler* speedTravel = nullptr;
	SpeedDelayHandler* speedHeadDown = nullptr;
	SpeedDelayHandler* speedHeadUp = nullptr;

// methods
	void runServer();
	void handleCommandQueue();
	std::string handleCommand(std::shared_ptr<Command> command);
	void answerToSocket(tcp::socket* socket, const std::string& message);

	// gpio
	void setup();
	void cleanup();

	// movement
	void enable(bool newEnableValue);
	void stepWithDelay(Axis axis, SpeedDelayHandler* speedDelayHandler);
	void step(Axis axis);

	void setDir(Axis axis, int direction);
	void setHead(bool value);

	// audio
	void playChord(const std::vector<float>& midis, float duration);
	void playFreq(Axis axis, float freq, float duration);
	void playNote(Axis axis, float midi, float duration);

	// high level movement
    void runQueueAsync(const std::shared_ptr<Queue>& queue);
	void stopQueueThread();
	void runQueue(const std::shared_ptr<Queue>& queue);

	void pauseQueue();
	void resumeQueue();
	void abortQueue();

	bool getPausedQueue(std::shared_ptr<Queue>& outQueue);
	void deletePauseFile();

	void moveTo(float fromSpeed, const SpeedPoint& to, SpeedDelayHandler* speedDelayHandler);
	void move(int x, int y, float speedStart, float speedEnd, SpeedDelayHandler* speedDelayHandler);
	void travelToWithAccelleration(const Vec2& to);

private:
	bool isLeft(float ax, float ay, float bx, float by, float cx, float cy);

	std::chrono::time_point<std::chrono::high_resolution_clock> lastStep[3];
	void updateLastStepToNow(Axis axis);

	long long getMicrosSinceLastStep(Axis axis);

	SharedState* sharedState;

	// sounds
	enum class Sound
	{
		Startup,
		Run,
		Pause,
		Abort,
		Success,
	};
	void playSound(Sound sound);
};

}
