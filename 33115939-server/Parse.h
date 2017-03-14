// Kuan Hao Yu     33115939
// Kimberly Nguy   86242309
// Pafoua Fang     84351619
// Kuanping Change 71018021

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