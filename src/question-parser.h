#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <string>
#include <vector>
#include "../libs/json.hpp"
#include <random>

namespace JsonProcessor
{
    std::string getRandomQuestion(const std::string &category);
}

#endif // JSON_PROCESSOR_H