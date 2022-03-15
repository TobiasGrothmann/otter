#pragma once
#include "stl.hpp"
#include "../../Serialize/Serializable.hpp"

namespace otter { class Queue; }

#define COMMAND_MESSAGE_END "||"

namespace otter {

class Command : public Serializable
{
public:
    Command() = default;
    Command(const std::string& verb, bool expectsReturn, const std::string& data);
    
    const static Command Pause;
    const static Command Resume;
    const static Command Abort;
    static Command Run(const Queue& q);
    
    std::string verb = {};
    bool expectsReturn = false;
    std::string data = {};
    
    bool send() const;
private:
    bool getIp(const std::string& hostName, const std::string& port, std::string& outIp) const;

public:
// serializable
    virtual void serialize(std::ostream& os) const override;
    virtual std::shared_ptr<Serializable> deserialize(const std::vector<std::string>& subElements) override;
    virtual std::string getName() const override;
};

}
