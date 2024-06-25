#include "number.hpp"

double MattsUtils::Number::parseDouble(std::string str) 
{
    char* endptr;
    double num = strtod(str.c_str(), &endptr);
    if (endptr == str.c_str()) {
        // No digits found
        std::cerr << "ERROR: failed to parse str (" << str << ") to double." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (*endptr != '\0') {
        std::cerr << "ERROR: failed to parse str (" << str << ") to double." << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        return num;
    }
}

float MattsUtils::Number::parseFloat(std::string str)
{
    return (float)parseDouble(str);
}

long MattsUtils::Number::parseLong(std::string str) 
{
    char* endptr;
    long num = strtol(str.c_str(), &endptr, 10);
    if (endptr == str.c_str()) {
        // No digits found
        std::cerr << "ERROR: failed to parse str (" << str << ") to long." << std::endl;
        exit(EXIT_FAILURE);
    }
    else if (*endptr != '\0') {
        std::cerr << "ERROR: failed to parse str (" << str << ") to long." << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        return num;
    }
}

int MattsUtils::Number::parseInt(std::string str) 
{
    return (int)parseLong(str);
}

unsigned char MattsUtils::Number::parseCharNum(std::string str) 
{
    return (unsigned char)parseLong(str);
}

double MattsUtils::Number::average(std::vector<int> array)
{
    double sum = 0;
    for (int i = 0; i < array.size(); i++) {
        sum += array.at(i);
    }

    return sum / (double)array.size();
}
