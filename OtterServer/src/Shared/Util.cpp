#include "Util.hpp"

namespace otter {

void Util::sleepSeconds(float seconds)
{
	usleep((useconds_t) (seconds * 1000.0f * 1000.0f));
}

}
