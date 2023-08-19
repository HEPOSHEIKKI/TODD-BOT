#ifndef JSON_PROCESSOR_H
#define JSON_PROCESSOR_H

#include <string>
#include <vector>
#include "../libs/json.hpp"
#include <random>

namespace JsonProcessor
{
    std::string getRandomQuestion(const std::string &category, std::uint64_t server_id);
}

#endif // JSON_PROCESSOR_H