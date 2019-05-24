#pragma once
#include <string>

namespace cz_mg_logger
{
    class Logger
    {
    public:
        static std::string filename;

    public:
        static void log(std::string message);
    };
}

inline std::string operator+(std::string left, int right)
{
    return left.append(std::to_string(right));
}

inline std::string operator+(int left, std::string right)
{
    return right.insert(0, std::to_string(left));
}