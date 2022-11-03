#include <Profiler.h>
#include <chrono>
#include <thread>
#include <string>
#include <iostream>

using namespace std;
using namespace RTT;

Profiler diag;

void testDelay(int time_ms, string tag);

int main()
{
    cout << "=== START TEST ===" << endl;
    diag.setFile("diag.log");
    diag.reg();
    cout << "Output file: " << diag.fileName() << endl;

    cout << "1ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(1, "1ms");

    cout << "10ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(10, "10ms");

    cout << "20ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(20, "20ms");

    cout << "50ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(50, "50ms");

    cout << "100ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(100, "100ms");

    cout << "200ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(200, "200ms");

    cout << "500ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(500, "500ms");

    cout << "1000ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(1000, "1000ms");

    cout << "1ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(1, "1ms");
    
    cout << "10ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(10, "10ms");

    cout << "20ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(20, "20ms");

    cout << "50ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(50, "50ms");

    cout << "100ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(100, "100ms");

    cout << "200ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(200, "200ms");

    cout << "500ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(500, "500ms");

    cout << "1000ms delay x5 ..." << endl;
    for(int i = 0; i < 5; i++) testDelay(1000, "1000ms");

    diag.save();

    cout << "=== END TEST ===" << endl;
    return 0;
}

void testDelay(int time_ms, string tag)
{
    diag.start(tag);
    this_thread::sleep_for(chrono::milliseconds(time_ms));
    diag.stop();
}