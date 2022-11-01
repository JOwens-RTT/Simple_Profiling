#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <iostream>
#include <string>

//#define INFO()      { std::cout << __PRETTY_FUNC__ << " [ " << __LINE__ << " ] "<< std::endl; }
#define INFO(msg)   { std::cout << __PRETTY_FUNCTION__ << " [ " << __LINE__ << " ] " << msg << std::endl; }

#endif // __LOGGING_H__