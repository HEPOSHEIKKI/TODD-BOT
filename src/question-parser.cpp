#include "config.h"
#include "question-parser.h"
#include "database.h"
using json = nlohmann::json;

namespace JsonProcessor
{
    static json loadDataFromJson(const std::string &filename)
    {
        std::ifstream ifs(filename);
        if (!ifs.is_open())
        {
            throw std::runtime_error("Error opening JSON file.");
        }

        json data;
        ifs >> data;

        return data;
    }

    static std::vector<std::string> getQuestions(const json &data, const std::string &category)
    {
        if (!data.contains(category))
        {
            throw std::runtime_error("JSON file does not contain the specified category.");
        }

        return data[category].get<std::vector<std::string>>();
    }

    std::string getRandomQuestion(const std::string &category, std::uint64_t server_id)
    {
        std::vector<std::string> questions;
        static json data = loadDataFromJson("questions.json");
        if (getNsfwSetting("database.db", server_id) == 3){
            questions = getQuestions(data, category + "-nsfw");
        }
        else if (getNsfwSetting("database.db", server_id) == 2){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::discrete_distribution<> dist({40, 60});

            int choice = dist(gen);

            if (choice == 0){
                questions = getQuestions(data, category + "-nsfw");
            }
            else {
                questions = getQuestions(data, category);
            }
        }
        else {
            questions = getQuestions(data, category);
        }
        

        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> dist(0, questions.size() - 1);

        int index = dist(rng);
        return questions[index];
    }
}