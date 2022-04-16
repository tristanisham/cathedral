#include "cathedral.hpp"

std::vector<std::string> ct::split(std::string source, const char &deliminator)
{
    std::stringstream ss(source);
    std::string line;
    std::vector<std::string> lines;

    // int line_num = 0;
    // bool reached_body = false;
    while (std::getline(ss, line, deliminator))
    {
        lines.push_back(line); // I could do this in one pass but call that a future improvement.
    }

    return lines;
}