#include "ReceivedCommand.hpp"

using namespace std;

namespace otter {

ReceivedCommand::ReceivedCommand(shared_ptr<Command> command, tcp::socket* socket)
    : command(command)
    , socket(socket)
{
}

ReceivedCommand::~ReceivedCommand()
{
    invalidate();
}

void ReceivedCommand::invalidate()
{
    isValid = false;
    if (socket && socket->is_open())
    {
        socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket->close();
    }
    if (socket)
    {
        delete socket;
        socket = nullptr;
    }
}

}
