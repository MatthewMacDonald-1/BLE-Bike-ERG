#pragma once
#include <iostream>
#include <string>

namespace MattsUtils::Number {

    double parseDouble(std::string str);

    float parseFloat(std::string str);

    long parseLong(std::string str);

    int parseInt(std::string str);

    unsigned char parseCharNum(std::string str);
	
}
