#include "SocketServer.hpp"

using namespace boost::asio;
using ip::tcp;
using namespace std;
using namespace boost;

namespace otter {

string SocketServer::read(tcp::socket* socket)
{
    boost::asio::streambuf buf;
    boost::asio::read_until(*socket, buf, COMMAND_MESSAGE_END);
    string data = boost::asio::buffer_cast<const char*>(buf.data());

    return data.substr(0, data.length() - 2);
}

SocketServer::SocketServer(SharedState* sharedState)
	: sharedState(sharedState)
{
}


void SocketServer::runServer()
{
    boost::asio::io_service io_service;
    tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 41414));

    while (!sharedState->abort)
    {
        tcp::socket* socket = new tcp::socket(io_service);
        acceptor.accept(*socket);

        const string message = SocketServer::read(socket);
        std::shared_ptr<Command> command = dynamic_pointer_cast<Command>(SerializableFactory::createPtr(message));

        if (!command->expectsReturn)
        {
            socket->close();
        }

        std::shared_ptr<ReceivedCommand> receivedCommand = std::make_shared<ReceivedCommand>(command, socket);
        sharedState->commands.push(receivedCommand);
    }
}

}
