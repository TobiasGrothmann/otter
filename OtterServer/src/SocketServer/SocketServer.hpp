#pragma once
#include "stl.hpp"
#include "../Shared/SharedState.hpp"
#include "../Shared/ReceivedCommand.hpp"

namespace otter {

class SocketServer
{
public:

    SocketServer(SharedState* sharedState);

    void runServer();

    static std::string read(boost::asio::ip::tcp::socket* socket);

private:

    SharedState* sharedState;

};

}
