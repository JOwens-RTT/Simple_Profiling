#include "Profiler.h"
#include <fstream>
#include <stdio.h>
#include <logging.h>
#include <sstream>
// Q_LOGGING_CATEGORY(ProfilerCategory, "[Profiler]")
// #define CATEGORY ProfilerCategory
// #include "../inc/logging.h"

using namespace RTT;
using namespace std;
using namespace std::chrono;

Stamp::Stamp(string csv)
{
    
}

string Stamp::toString()
{
    stringstream ret; 
    ret << "Name: " << functionName << ", Start (ms): " << startTime.count() << ", Duration (ms): " << timeSpan_ms.count()
        << ", Duration (us): " << timeSpan_us.count() << ", Duration (ns): " << timeSpan_ns.count();
    return ret.str();
}

string Stamp::toCsv()
{
    stringstream ret;
    ret << functionName << ";" << startTime.count() << ";" << timeSpan_ms.count() << ";" << timeSpan_us.count() << ";" << timeSpan_ns.count() << ";";
    return ret.str();
}

ProfilerHandler Profiler::handler((5));

Profiler::Profiler()
{
    // INFO()
    launchTime = high_resolution_clock::now();
    // handler.connect(this);
}

Profiler::~Profiler()
{
    // INFO()
    handler.disconnect(this);
}

void Profiler::start(string name)
{
    // INFO()
    funcName = name;

    // Start recording. MUST BE LAST COMMAND IN FUNCTION!!!
    startTime = high_resolution_clock::now();
}

string Profiler::stop()
{
    // INFO()
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
    // INFO("Buffer Size: " << buffer.size())
    return buffer.size();
}

string Profiler::pop()
{
    string ret = buffer.front().toCsv();
    buffer.pop();
    // INFO(ret)
    return ret;
}

void Profiler::clear() 
{
    // INFO()
    while(buffer.size() > 0) buffer.pop();
}

void Profiler::setFile(std::string filename)
{
    // INFO()
    handler.setFile(filename);
}

std::string Profiler::fileName()
{
    return handler.fileName();
}

void Profiler::save()
{
    // INFO()
    handler.save();
}

void Profiler::reg()
{
    handler.connect(this);
}

ProfilerHandler::ProfilerHandler(unsigned int bufferLimit)
{
    // INFO()
    limit = bufferLimit;
}

void ProfilerHandler::setFile(string filename)
{
    // INFO(filename.c_str())
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
    // INFO("Registering new member... (Number of registered members: " << members.size() << ")")
    members.push_back(member);
    // INFO("New member registered. (Number of registered members: " << members.size() << ")")
}

void ProfilerHandler::disconnect(Profiler* member)
{
    // INFO()
}

void ProfilerHandler::save()
{
    // INFO("Saving...")
    if(logFile == "") return;

    ofstream outfile;

    outfile.open(logFile, std::ios_base::out | std::ios_base::app);
    if(!outfile.is_open())
    {
        // CRITICAL("Failed to open Profiler file!")
        throw std::runtime_error("Failed to open Profiler file!");
    }
    // INFO("Number of member registered: " << members.size())
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
    // INFO()
    unsigned int bufferCount = 0;
    for(Profiler* member : members)
    {
        bufferCount += member->size();
    }

    if(bufferCount < limit) return;
    
    save();
}

