#include "Command.hpp"
#include "../Queue.hpp"
#include <boost/asio.hpp>

using namespace boost;
using namespace boost::asio;
using ip::tcp;
using namespace std;

namespace otter {

const Command Command::Pause = Command("pause", false, "nothing");
const Command Command::Resume = Command("resume", false, "nothing");
const Command Command::Abort = Command("abort", false, "nothing");

Command::Command(const string& verb, bool expectsReturn, const string& data)
    : verb(verb)
    , expectsReturn(expectsReturn)
    , data(data)
{
}

void Command::serialize(ostream& os) const
{
    os << verb << splitter << expectsReturn << splitter << data;
}

std::shared_ptr<Serializable> Command::deserialize(const vector<string>& subElements)
{
    std::shared_ptr<Command> command = make_shared<Command>(subElements[0],
                                                            (bool)atoi(subElements[1].c_str()),
                                                            subElements[2]);
    
    return command;
}

string Command::getName() const
{
    return "command";
}


Command Command::Run(const Queue& q)
{
    ostringstream stream;
    stream << q;
    const string queueString = stream.str();
    
    return Command("run", false, queueString);
}


bool Command::getIp(const string& hostName, const string& port, string& outIp) const
{
    try
    {
        io_service io_service;
        ip::tcp::resolver resolver(io_service);
        ip::tcp::resolver::query ipQuery(ip::tcp::v4(), hostName, port);
        ip::tcp::resolver::iterator iter = resolver.resolve(ipQuery);

        ip::tcp::endpoint endpoint(*iter);
        
        outIp = endpoint.address().to_v4().to_string();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool Command::send() const
{
    const static string hostName = "otter.local";
    const static string port = "41414";
    
    io_service io_service;
    ip::tcp::resolver resolver(io_service);
    string ip = {};
    if (!getIp(hostName, port, ip))
    {
        cout << "ERROR: failed to reach host" << endl;
        return false;
    }

    try
    {
        ip::tcp::resolver::query ipQuery(ip, port);
        ip::tcp::resolver::iterator iter = resolver.resolve(ipQuery);
        
        ip::tcp::endpoint endpoint(*iter);
        ip::tcp::socket socket(io_service, endpoint.protocol());
        
        socket.connect(endpoint);
        
        ostringstream message;
        message << this << COMMAND_MESSAGE_END;
        
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(message.str()), error);
        
        return !error;
    }
    catch (...)
    {
        return false;
    }
}

}
