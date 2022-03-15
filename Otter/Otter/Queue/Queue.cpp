#include "Queue.hpp"
#include "../Plottables/Path.hpp"
#include "../Plottables/Rectangle.hpp"
#include "../Plottables/Circle.hpp"
#include "../Plottables/Bezier.hpp"
#include "../Math/Geometry.hpp"
#include "Command/Command.hpp"

#include <fstream>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

namespace otter {

Queue::Queue(const vector<shared_ptr<Plottable>>& items)
{
    this->items = items;
}
Queue::Queue(const Queue& other)
{
    this->settings = other.settings;
    for (const shared_ptr<Plottable>& item : other.items)
    {
        this->add(item->copy());
    }
}
Queue::Queue(Queue&& other)
{
    this->settings = other.settings;
    this->items = other.items;
    other.items.clear();
}
Queue& Queue::operator=(const Queue& other)
{
    this->settings = other.settings;
    for (const shared_ptr<Plottable>& item : other.items)
    {
        this->add(item->copy());
    }
    return *this;
}
Queue& Queue::operator=(Queue&& other)
{
    this->settings = other.settings;
    this->items = other.items;
    other.items.clear();
    return *this;
}

void Queue::add(shared_ptr<Plottable> newItem)
{
    if (newItem)
    {
        items.push_back(newItem);
    }
    else
    {
        cout << "WARNING: tried to add invalid plottable item to queue" << endl;
    }
}

void Queue::add(const vector<shared_ptr<Plottable>>& newItems)
{
    for (const shared_ptr<Plottable>& newItem : newItems)
    {
        this->add(newItem);
    }
}

void Queue::add(const Queue& otherQueue)
{
    items.insert(items.end(), otherQueue.items.begin(), otherQueue.items.end());
}
   
void Queue::addFront(shared_ptr<Plottable> item)
{
    if (item)
    {
        items.insert(items.begin(), item);
    }
    else
    {
        cout << "WARNING: tried to addFront invalid plottable item to queue" << endl;
    }
}

void Queue::circle(const Vec2& center, double radius)
{
    add(Circle::create(center, radius));
}
void Queue::rectangle(const Vec2& bottomLeft, const Vec2& topRight)
{
    add(Rectangle::create(bottomLeft, topRight));
}
void Queue::path(const vector<Vec2>& points)
{
    add(Path::create(points));
}
void Queue::dot(const Vec2& point)
{
    add(Circle::create(point, 0.01));
}
void Queue::bezier(const vector<Vec2>& points)
{
    add(Bezier::create(points));
}

void Queue::save(string filePath) const
{
    ofstream outfile;
    outfile.open(filePath);
    outfile << this;
    outfile.close();
}

Queue Queue::load(string filePath)
{
    ifstream inFile;
    inFile.open(filePath);
    string inFileString((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();
    
    shared_ptr<Queue> queueSer = dynamic_pointer_cast<Queue>(SerializableFactory::createPtr(inFileString));
    return *queueSer.get();
}

bool Queue::send() const
{
    Command run = Command::Run(*this);
    return run.send();
}

void Queue::preview() const
{
    int fd_p2c[2], fd_c2p[2];
    ssize_t bytes_read;
    pid_t childpid;
    char readbuffer[800];
    string otterUIPath = "/usr/local/bin/otterUI";
    stringstream ss;
    ss << this;
    string queueString = ss.str();

    string receive_output = "";

    if (pipe(fd_p2c) != 0 || pipe(fd_c2p) != 0)
    {
        cout << "ERROR: Failed to pipe" << endl;
        return;
    }
    childpid = fork();

    if (childpid < 0)
    {
        cout << "ERROR: Fork failed" << endl;
        return;
    }
    else if (childpid == 0)
    {
        if (dup2(fd_p2c[0], 0) != 0 ||
            close(fd_p2c[0]) != 0 ||
            close(fd_p2c[1]) != 0)
        {
            cout << "ERROR: Failed to set up standard input." << endl;
            return;
        }
        if (dup2(fd_c2p[1], 1) != 1 ||
            close(fd_c2p[1]) != 0 ||
            close(fd_c2p[0]) != 0)
        {
            cout << "ERROR: Failed to set up standard input." << endl;
            return;
        }

        execl(otterUIPath.c_str(), otterUIPath.c_str(), (char *) 0);
        cout << "ERROR: Failed to execute " << otterUIPath << endl;
        return;
    }
    else
    {
        close(fd_p2c[0]);
        close(fd_c2p[1]);

        size_t nbytes = queueString.length();
        if (write(fd_p2c[1], queueString.c_str(), nbytes) != (int)nbytes)
        {
            cout << "ERROR: Parent: short write to child" << endl;
            return;
        }
        close(fd_p2c[1]);

        while (1)
        {
            bytes_read = read(fd_c2p[0], readbuffer, sizeof(readbuffer)-1);

            if (bytes_read <= 0)
                break;

            readbuffer[bytes_read] = '\0';
            receive_output += readbuffer;
        }
        close(fd_c2p[0]);
    }
}

const vector<double> Queue::getPaintAndTravelDistance() const
{
    Vec2 location = Vec2(0.0, 0.0);
    double paintDist = 0.0;
    double travelDist = 0.0;
    
    for (const shared_ptr<Plottable>& plottalbe : items)
    {
        const vector<Vec2> path = plottalbe->getPlottablePoints(SampleSettings(5.0, location));
        if (path.size() <= 1) continue;
        
        travelDist += location.distance(path[0]);
        
        for (VINDEX(path) i = 0; i < path.size() - 1; i++)
        {
            paintDist += path[i].distance(path[i + 1]);
        }
        
        location = path[path.size() - 1];
    }
    
    return { paintDist, travelDist };
}


const array<Vec2, 2> Queue::getStartAndEndFromPlottalbe(const shared_ptr<Plottable> plottable) const
{
    const vector<Vec2> points = plottable->getPlottablePoints(SampleSettings(0.5));
    const array<Vec2, 2> startAndEnd = {
        points[0],
        points[points.size() - 1]
    };
    return startAndEnd;
}

void Queue::optimize()
{
    vector<array<Vec2, 2>> startsAndEnds;
    startsAndEnds.reserve(items.size());
    for (const shared_ptr<Plottable>& plottalbe : items)
    {
        startsAndEnds.push_back(getStartAndEndFromPlottalbe(plottalbe));
    }
    
    vector<shared_ptr<Plottable>> newItems;
    newItems.reserve(items.size());
    Vec2 currrentPos = Vec2(0.0, 0.0);
    while (items.size() > 0)
    {
        bool initialized = false;
        VINDEX(startsAndEnds) closestIndex = 0;
        double closestDist = 0.0;
        bool reversed = false;
        for (VINDEX(startsAndEnds) i = 0; i < startsAndEnds.size(); i++)
        {
            const double distToStart = currrentPos.distanceSquared(startsAndEnds[i][0]);
            const double distToEnd = currrentPos.distanceSquared(startsAndEnds[i][1]);
            if (distToStart < closestDist || !initialized)
            {
                closestIndex = i;
                closestDist = distToStart;
                initialized = true;
                reversed = false;
            }
            if (distToEnd < closestDist)
            {
                closestIndex = i;
                closestDist = distToEnd;
                reversed = true;
            }
        }
        newItems.push_back(items[closestIndex]);
        currrentPos = startsAndEnds[closestIndex][reversed ? 0 : 1];
        
        startsAndEnds.erase(startsAndEnds.begin() + (long)closestIndex);
        items.erase(items.begin() + (long)closestIndex);
    }
    items = newItems;
}


void Queue::serialize(ostream& os) const
{
    os << settings;
    if (items.size() > 0) os << splitter;
    for (VINDEX(items) i = 0; i < items.size(); i++)
    {
        os << items[i];
        if (i != items.size() - 1)
        {
            os << splitter;
        }
    }
}
shared_ptr<Serializable> Queue::deserialize(const vector<string>& subElements)
{
    shared_ptr<Queue> newQueue = make_shared<Queue>();
    newQueue->settings = *dynamic_pointer_cast<QueueSettings>(SerializableFactory::createPtr(subElements[0])).get();
    
    for (VINDEX(subElements) i = 1; i < subElements.size(); i++)
    {
        shared_ptr<Serializable> itemSer = SerializableFactory::createPtr(subElements[i]);
        shared_ptr<Plottable> item = dynamic_pointer_cast<Plottable>(itemSer);
        newQueue->add(item);
    }
    return newQueue;
}
string Queue::getName() const
{
    return "Queue";
}


Rectangle Queue::getBounds() const
{
    Rectangle totalBounds = Rectangle(Vec2(0, 0), Vec2(0, 0));
    bool bInitialized = false;
    for (const shared_ptr<Plottable>& item : items)
    {
        const Rectangle& bounds = item->getBounds();
        if (!bInitialized)
        {
            totalBounds = bounds;
            bInitialized = true;
            continue;
        }
        
        if (bounds.bottomLeft.x < totalBounds.bottomLeft.x)
        {
            totalBounds.bottomLeft.x = bounds.bottomLeft.x;
        }
        if (bounds.bottomLeft.y < totalBounds.bottomLeft.y)
        {
            totalBounds.bottomLeft.y = bounds.bottomLeft.y;
        }
        if (bounds.topRight.x > totalBounds.topRight.x)
        {
            totalBounds.topRight.x = bounds.topRight.x;
        }
        if (bounds.topRight.y > totalBounds.topRight.y)
        {
            totalBounds.topRight.y = bounds.topRight.y;
        }
    }
    if (!bInitialized)
    {
        cout << "WARNING: returning zero Bounds for empty Queue" << endl;
    }
    return totalBounds;
}

vector<Vec2> Queue::getConvexHull(const SampleSettings& sampleSettings) const
{
    vector<Vec2> points = {};
    for (const shared_ptr<Plottable>& p : items)
    {
        const vector<Vec2> pPoints = p->getPlottablePoints(sampleSettings);
        points.insert(points.end(), pPoints.begin(), pPoints.end());
    }
    return Geometry::findConvexHull(points);
}

void Queue::offset(const Vec2& dist)
{
    for (shared_ptr<Plottable>& item : items)
    {
        item->offset(dist);
    }
}
void Queue::scale(const double factor)
{
    for (shared_ptr<Plottable>& item : items)
    {
        item->scale(factor);
    }
}

void Queue::rotate(const Vec2& pivot, double angle)
{
    for (VINDEX(items) i = 0; i < items.size(); i++)
    {
        if (items[i]->canBeRotated())
        {
            items[i]->rotate(pivot, angle);
        }
        else // turn into path and rotate
        {
            items[i] = make_shared<Path>(items[i]->getPlottablePoints());
            items[i]->rotate(pivot, angle);
        }
    }
}

void Queue::normalize(const Rectangle& targetBounds, bool moveToMid)
{
    Rectangle allBounds = this->getBounds();
    Rectangle allBoundsNorm = allBounds;
    allBoundsNorm.normalize(targetBounds, moveToMid);
    
    const double scale = allBoundsNorm.getWidth() / allBounds.getWidth();
    
    this->offset(allBounds.getBottomLeft() * -1.0);
    this->scale(scale);
    this->offset(allBoundsNorm.getBottomLeft());
}

void Queue::combinePlottables(const SampleSettings& sampleSettings)
{
    size_t lastNumPaths = 0;
    vector<shared_ptr<Path>> output = {};
    output.reserve(items.size());
    vector<shared_ptr<Plottable>> closedPlottables = {};
    closedPlottables.reserve(items.size());
    vector<shared_ptr<Path>> paths = {};
    closedPlottables.reserve(items.size());
    for (const shared_ptr<Plottable>& plottable : items)
    {
        if (plottable->getIsClosed())
        {
            closedPlottables.push_back(plottable);
        }
        else
        {
            paths.push_back(Path::create(plottable->getPlottablePoints(sampleSettings)));
        }
    }
    int tries = 0;
    while (paths.size() != lastNumPaths)
    {
        if (tries++ == 100)
        {
            cout << "WARNING: tried combining too often, stopping" << endl;
            break;
        }
        lastNumPaths = paths.size();
        
        output.clear();
        
        const double distThresh = 0.0001;
        for (const shared_ptr<Path>& path : paths)
        {
            const Vec2& firstPoint = path->points[0];
            const Vec2& lastPoint = path->points[path->points.size() - 1];
            
            bool foundConnection = false;

            for (const shared_ptr<Path>& combinedPath : output)
            {
                const Vec2& firstPointCombined = combinedPath->points[0];
                const Vec2& lastPointCombined = combinedPath->points[combinedPath->points.size() - 1];
                
                if (lastPointCombined.distanceManhattan(firstPoint) < distThresh)
                {
                    combinedPath->add(path, false);
                    foundConnection = true;
                    break;
                }
                else if (lastPointCombined.distanceManhattan(lastPoint) < distThresh)
                {
                    combinedPath->add(path, true);
                    foundConnection = true;
                    break;
                }
                else if (firstPointCombined.distanceManhattan(lastPoint) < distThresh)
                {
                    combinedPath->addFront(path, false);
                    foundConnection = true;
                    break;
                }
                else if (firstPointCombined.distanceManhattan(firstPoint) < distThresh)
                {
                    combinedPath->addFront(path, true);
                    foundConnection = true;
                    break;
                }
            }
            
            if (!foundConnection)
            {
                output.push_back(path);
            }
        }
        
        paths = output;
    }
    
    
    items.clear();
    items.insert(items.end(), closedPlottables.begin(), closedPlottables.end());
    items.insert(items.begin(), paths.begin(), paths.end());
}

Queue Queue::getMaskedInside(const shared_ptr<Plottable>& mask) const
{
    Queue out = Queue();
    out.items.reserve(items.size());
    for (const shared_ptr<Plottable>& item : items)
    {
        out.add(item->mask(mask).inside);
    }
    return out;
}

Queue Queue::getMaskedOutside(const shared_ptr<Plottable>& mask) const
{
    Queue out = Queue();
    out.items.reserve(items.size());
    for (const shared_ptr<Plottable>& item : items)
    {
        out.add(item->mask(mask).outside);
    }
    return out;
}

}
