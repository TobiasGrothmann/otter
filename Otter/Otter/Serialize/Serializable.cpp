#include "Serializable.hpp"

using namespace std;

namespace otter {

ostream& operator<<(ostream& os, const Serializable& object)
{
    os << "<" << object.getName() << "|";
    object.serialize(os);
    os << ">";
    return os;
}
ostream& operator<< (ostream& os, const shared_ptr<Serializable>& object)
{
    os << "<" << object->getName() << "|";
    object->serialize(os);
    os << ">";
    return os;
}
ostream& operator<< (ostream& os, Serializable* object)
{
    os << "<" << object->getName() << "|";
    object->serialize(os);
    os << ">";
    return os;
}
ostream& operator<< (ostream& os, const Serializable* object)
{
    os << "<" << object->getName() << "|";
    object->serialize(os);
    os << ">";
    return os;
}



bool Serializable::parseElement(const string& element, string& name, vector<string>& subElements)
{
    const size_t splitIndex = element.find('|');
    if (splitIndex == string::npos)
    {
        cout << "ERROR: failed to find | in '" << element << "'" << endl;
        return false;
    }
    name = element.substr(1, splitIndex - 1);
    
    uint64_t levelsIn = 0;
    uint64_t lastSplitLocation = splitIndex + 1;
    uint64_t thisElementLength = 0;
    for (uint64_t i = lastSplitLocation; i < element.size(); i++)
    {
        thisElementLength++;
        
        if (element[i] == '<')
        {
            levelsIn++;
        }
        else if (element[i] == '>')
        {
            levelsIn--;
        }
        if ((levelsIn == 0 && element[i] == splitter) || (i == element.size() - 1))
        {
            const string& newString = element.substr(lastSplitLocation, thisElementLength - 1);
            if (!newString.empty())
            {
                subElements.push_back(newString);
            }
            lastSplitLocation = i + 1;
            thisElementLength = 0;
        }
    }
    
    return true;
}

}
