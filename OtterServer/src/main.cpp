#include "stl.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>

#include "Otter.hpp"
#include "OtterServer/OtterServer.hpp"
#include "SocketServer/SocketServer.hpp"
#include "Shared/SharedState.hpp"
#include "Shared/ReceivedCommand.hpp"

using namespace std;
using namespace boost;
using namespace otter;

static void runOtterServer(OtterServer *&otterServer, SharedState *sharedState)
{
    try
    {
        otterServer = new OtterServer(sharedState);
        otterServer->runServer();
    }
    catch (std::exception const &e)
    {
        cout << "FATAL ERROR - in Otter" << endl;
        cout << e.what() << endl;
        sharedState->abort = true;
        delete otterServer;
        otterServer = nullptr;
        return;
    }
}

static void runSocketServer(SocketServer *&socketServer, SharedState *sharedState)
{
    try
    {
        socketServer = new SocketServer(sharedState);
        socketServer->runServer();
    }
    catch (std::exception const &e)
    {
        cout << "FATAL ERROR - in SocketServer" << endl;
        cout << e.what() << endl;
        sharedState->abort = true;
        delete socketServer;
        socketServer = nullptr;
        return;
    }
}

int main()
{
#if WITH_GPIO
    if (getuid())
    {
        cout << "ERROR: must run as root" << endl;
        return 1;
    }
#endif

    while (true)
    {
        try
        {
            // INIT
            SharedState *sharedState = new SharedState();

            OtterServer *otterServer = nullptr;
            SocketServer *socketServer = nullptr;

            // START
            cout << "# Setting up Otter" << endl;
            boost::thread otterThread = boost::thread(runOtterServer, otterServer, sharedState);

            cout << "# Setting up SocketServer" << endl;
            boost::thread socketThread = boost::thread(runSocketServer, socketServer, sharedState);

            // IDLE
            while (!sharedState->abort)
            {
                usleep(5000);
            }

            otterThread.join();
            socketThread.join();
        }
        catch (const std::exception &e)
        {
            std::cerr << "restarting otter: " << e.what() << '\n';
        }
    }
    return 0;
}
