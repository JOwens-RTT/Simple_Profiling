#ifndef __DIAGNOSTICS_H__
#define __DIAGNOSTICS_H__

#include <iostream>
#include <string>
#include <chrono>
#include <list>
#include <queue>
#include <memory>
#include <stdexcept>
#include <math.h>

// #include <QLoggingCategory>
// Q_DECLARE_LOGGING_CATEGORY(DiagnosticsCategory)


namespace RTT 
{

struct Stamp
{
    std::string functionName;
    std::chrono::milliseconds startTime;
    std::chrono::milliseconds timeSpan_ms;
    std::chrono::microseconds timeSpan_us;
    std::chrono::nanoseconds timeSpan_ns;

    std::string toString();

    std::string toCsv();
};

class Diagnostics;
class DiagnosticsHandler;

class Diagnostics 
{
public:
    Diagnostics();
    ~Diagnostics();
    // Get start stamp
    void start(std::string name = __PRETTY_FUNCTION__);
    // Get end stamp, calculate duration, check buffer overflow
    std::string stop();
    // Get buffer size
    unsigned int size();
    // Get entry
    std::string pop();
    // Clear stamps out of buffers
    void clear();
    // Set file
    void setFile(std::string filename);
    // Get file
    std::string fileName();
    // Force the log to save
    void save();
    // Register instance
    void reg();
private:
    std::string funcName;
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point stopTime;
    std::chrono::high_resolution_clock::time_point launchTime;
    std::queue<Stamp> buffer;
    static DiagnosticsHandler handler;
};

class DiagnosticsHandler
{
public:
    DiagnosticsHandler(unsigned int bufferLimit = 1000);
    // Set the output file
    void setFile(std::string filename);
    // Get file
    std::string fileName();
    // Connect to a Diagnostics object
    void connect(Diagnostics* member);
    // Disconnect from a Diagnostics object
    void disconnect(Diagnostics* member);
    // Save buffers to file and dump buffers
    void save();
    // Check buffer size and save if necessary
    void check();
private:
    unsigned int limit;
    std::list<Diagnostics*> members;
    std::string logFile = "";
};

}

#endif // __DIAGNOSTICS_H__
