#ifndef __STRING_OPERATIONS_H__
#define __STRING_OPERATIONS_H__

#include <vector>
#include <string>

typedef std::vector<std::string> StringList;


StringList split(std::string str, char delim)
{
    StringList strings;
    return strings;
}

StringList split(std::string str, std::string delim)
{
    StringList strings;
    return strings;
}

StringList split(std::string str, const char* delim)
{
    int offset = 0;
    StringList strings;
    while(offset != std::string::npos)
    {
        int sep = str.find(delim, offset);
        if(sep == std::string::npos) break;
        strings.push_back(str.substr(offset, sep - offset));
    }
    return strings;
}

#endif