#pragma once
#include "stl.hpp"
#include "Otter.hpp"

using namespace boost::asio;
using ip::tcp;

namespace otter {

class ReceivedCommand
{
public:
    ReceivedCommand(std::shared_ptr<Command> command, tcp::socket* socket);
    ~ReceivedCommand();

    void invalidate();

    std::shared_ptr<Command> command = nullptr;

    bool isValid = true;
    tcp::socket* socket = nullptr;
};

}
