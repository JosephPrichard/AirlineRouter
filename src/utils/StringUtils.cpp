//
// Created by Ltpri on 4/21/2022.
//

#include "StringUtils.h"

std::vector<std::string> utils::splitString(std::string& str, char delim) {
    std::vector<std::string> splitStr;

    int index;
    std::string substring = str;
    while ((index = (int) substring.find(delim)) >= 0) {
        splitStr.push_back(substring.substr(0, index));
        substring = substring.substr(index + 1);
    }
    splitStr.push_back(substring);

    return splitStr;
}
