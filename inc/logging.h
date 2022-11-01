#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <iostream>
#include <string>

#if defined(__GNUC__) || defined(__GUNG__)
    #define PRINT_FUNC      __PRETTY_FUNCTION__
#else
    #define PRINT_FUNC      __func__ 
#endif

#define __FILE_FORMAT__         "\033[0m\033[1;37m"
#define __FUNC_FORMAT__         "\033[0m\033[1;94m"
#define __LINE_FORMAT__         "\033[0m\033[92m"
#define __WARN_FORMAT__         "\033[0m\033[34m"
#define __CRIT_FORMAT__         "\033[0m\033[1;101;37m"

#define __LOG__FORMAT_SHORT__   __FUNC_FORMAT__ << PRINT_FUNC << "\033[0m "
#define __LOG__FORMAT_LONG__    __FILE_FORMAT__ << __FILE__ << __LOG__FORMAT_SHORT__ << __LINE_FORMAT__ << "[ line: " << __LINE__ << "]\033[0m "

#define __DEBUG_MSG__           "\033[0m(DEBUG) " << __LOG__FORMAT_SHORT__
#define __INFO_MSG__            "\033[0m(INFO) " << __LOG__FORMAT_SHORT__
#define __WARN_MSG__            "\033[0m\033[1;34m(WARNING) " << __LOG__FORMAT_LONG__
#define __CRIT_MSG__            "\033[0m\033[1;91m(CRITICAL) " << __LOG__FORMAT_LONG__

#define DEBUG_0()         { std::cout << __DEBUG_MSG__ << std::endl; }
#define DEBUG_1(arg)      { std::cout << __DEBUG_MSG__ << arg << std::endl; }
#define INFO_0()          { std::cout << __INFO_MSG__ << std::endl;}  
#define INFO_1(arg)       { std::cout << __INFO_MSG__ << arg << std::endl; }
#define WARNING_0()       { std::cout << __WARN_MSG__ << std::endl; }
#define WARNING_1(arg)    { std::cout << __WARN_MSG__ << __WARN_FORMAT__ << arg << "\033[0m" << std::endl; }
#define CRITICAL_0()      { std::cout << __CRIT_MSG__ << std::endl; }
#define CRITICAL_1(arg)   { std::cout << __CRIT_MSG__ << __CRIT_FORMAT__ << arg << "\033[0m" << std::endl; }

#define GET_MACRO(_0,_1,NAME,...) NAME
#define DEBUG(...) GET_MACRO(_0 __VA_OPT__(,)__VA_ARGS__,DEBUG_1,DEBUG_0)(__VA_ARGS__)
#define INFO(...) GET_MACRO(_0 __VA_OPT__(,)__VA_ARGS__,INFO_1,INFO_0)(__VA_ARGS__)
#define WARNING(...) GET_MACRO(_0 __VA_OPT__(,)__VA_ARGS__,WARNING_1,WARNING_0)(__VA_ARGS__)
#define CRITICAL(...) GET_MACRO(_0 __VA_OPT__(,)__VA_ARGS__,CRITICAL_1,CRITICAL_0)(__VA_ARGS__)

//GET_MACRO(_0,INFO_1,INFO_0)()

#endif // __LOGGING_H__