#include "Profiler.h"
#include <fstream>
#include <stdio.h>
#include <logging.h>
// Q_LOGGING_CATEGORY(ProfilerCategory, "[Profiler]")
// #define CATEGORY ProfilerCategory
// #include "../inc/logging.h"

using namespace RTT;
using namespace std;
using namespace std::chrono;

string Stamp::toString()
{
    // char format[] = "Name: %s, Start: %d, Duration: %d";
    // int size = sprintf(nullptr, format, functionName.c_str(), startTime, timeSpan) + 1;
    // if(size <= 0) throw std::runtime_error("Error during formatting.");
    // auto sze = static_cast<size_t>(size);
    // std::unique_ptr<char[]> buf(new char[sze]);
    // sprintf(buf.get(), format, functionName.c_str(), startTime, timeSpan);
    // return std::string(buf.get(), buf.get() + sze - 1);
    return "";
}

string Stamp::toCsv()
{
    const char* format = "%s;%dms;%dms;%dus;%dns";
    unsigned int nameSize = functionName.size();
    unsigned int startSize = ceil(log10(startTime.count()) + 1);
    unsigned int spanSize = ceil(log10(timeSpan_ms.count()) + 1) + ceil(log10(timeSpan_us.count()) + 1) + ceil(log10(timeSpan_ns.count()) + 1);
    unsigned int length = nameSize + startSize + spanSize + 100;
    char* buff = new char[length];
    sprintf(buff, format, functionName.c_str(), startTime.count(), timeSpan_ms.count(), timeSpan_us.count(), timeSpan_ns.count());
    string ret(buff);
    delete[] buff;
    return ret;
    // return "";
}

ProfilerHandler Profiler::handler((5));

Profiler::Profiler()
{
    INFO()
    launchTime = high_resolution_clock::now();
    // handler.connect(this);
}

Profiler::~Profiler()
{
    INFO()
    handler.disconnect(this);
}

void Profiler::start(string name)
{
    INFO()
    funcName = name;

    // Start recording. MUST BE LAST COMMAND IN FUNCTION!!!
    startTime = high_resolution_clock::now();
}

string Profiler::stop()
{
    INFO()
    // Stop recording. MUST BE FIRST COMMAND IN FUNCTION!!!
    stopTime = high_resolution_clock::now();

    // Create stamp
    Stamp tstamp;
    tstamp.functionName = funcName;
    tstamp.startTime = duration_cast<milliseconds>(startTime - launchTime);
    tstamp.timeSpan_ms = duration_cast<milliseconds>(stopTime - startTime);
    tstamp.timeSpan_us = duration_cast<microseconds>(stopTime - startTime);
    tstamp.timeSpan_ns = duration_cast<nanoseconds>(stopTime - startTime);

    // Store stamp in buffer
    buffer.push(tstamp);

    // Have handler check buffer sizes
    handler.check();

    return tstamp.toString();
}

unsigned int Profiler::size()
{
    INFO("Buffer Size: " << buffer.size())
    return buffer.size();
}

string Profiler::pop()
{
    string ret = buffer.front().toCsv();
    buffer.pop();
    INFO(ret)
    return ret;
}

void Profiler::clear() 
{
    INFO()
    while(buffer.size() > 0) buffer.pop();
}

void Profiler::setFile(std::string filename)
{
    INFO()
    handler.setFile(filename);
}

std::string Profiler::fileName()
{
    return handler.fileName();
}

void Profiler::save()
{
    INFO()
    handler.save();
}

void Profiler::reg()
{
    handler.connect(this);
}

ProfilerHandler::ProfilerHandler(unsigned int bufferLimit)
{
    INFO()
    limit = bufferLimit;
}

void ProfilerHandler::setFile(string filename)
{
    INFO(filename.c_str())
    if(filename == "") 
        throw std::runtime_error("Filename is empty!");
    logFile = filename;
    ofstream outfile;
    outfile.open(logFile, std::ios_base::out);
    outfile << "Function;Start;Duration ms;Duration us;Duration ns\n";
    outfile.close();
}

std::string ProfilerHandler::fileName()
{
    return logFile;
}

void ProfilerHandler::connect(Profiler* member)
{
    INFO("Registering new member... (Number of registered members: " << members.size() << ")")
    members.push_back(member);
    INFO("New member registered. (Number of registered members: " << members.size() << ")")
}

void ProfilerHandler::disconnect(Profiler* member)
{
    INFO()
}

void ProfilerHandler::save()
{
    INFO("Saving...")
    if(logFile == "") return;

    ofstream outfile;

    outfile.open(logFile, std::ios_base::out | std::ios_base::app);
    if(!outfile.is_open())
    {
        // CRITICAL("Failed to open Profiler file!")
        throw std::runtime_error("Failed to open Profiler file!");
    }
    INFO("Number of member registered: " << members.size())
    for(Profiler* member : members)
    {
        while(member->size() > 0)
        {
            outfile << member->pop() << "\n";
        }
    }
    outfile.close();
}

void ProfilerHandler::check()
{
    INFO()
    unsigned int bufferCount = 0;
    for(Profiler* member : members)
    {
        bufferCount += member->size();
    }

    if(bufferCount < limit) return;
    
    save();
}

