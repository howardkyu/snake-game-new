#ifndef PARSE_H
#define PARSE_H

#include <string>
#include <vector>
#include <sstream>

class Parse {
    public:
        Parse();
        static std::vector<std::string> split(std::string message, char delimiter);
};

#endif /* PARSE_H */
#pragma once