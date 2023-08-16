#include "database.h"
#include "config.h"



int initDataBase(const char* filePath){
    sqlite3* DB;
    createDB(filePath);
    createTable(filePath);
    insertData(filePath, 1234567891234567890, 1234567891234567891, 1);
    return 0;
}

int createDB(const char* filePath){
    sqlite3* DB;
    int exit = 0;

    exit = sqlite3_open(filePath, &DB);
    sqlite3_close(DB);
    return 0;
}

int createTable(const char* filePath) {
    sqlite3* DB;

    std::string sql = "CREATE TABLE IF NOT EXISTS servers(id BIGINT UNIQUE NOT NULL, main_channel BIGINT, nsfw_enabled BOOL DEFAULT 0);";

    try {
        int exit = 0;
        exit = sqlite3_open(filePath, &DB);

        char* messageError;
        exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

        if (exit != SQLITE_OK) {
            std::cerr << "Error CREATE TABLE" << std::endl;
            sqlite3_free(messageError);
        }
        else {
            std::cout << "Table created succesfully" << std::endl;
        }
        sqlite3_close(DB);
    }
    catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

int insertData(const char* filePath, std::uint64_t server_id, std::uint64_t main_channel_id = NULL, bool nsfw_enabled = NULL){
    sqlite3* DB;
    char* messageError;

    int exit = sqlite3_open(filePath, &DB);

    // INSERT OR IGNORE INTO servers: this will create a row with the provided server id on id if not exists
    std::string sql = "INSERT OR IGNORE INTO servers (id, main_channel, nsfw_enabled) VALUES('" + std::to_string(server_id) + "', '" + std::to_string(main_channel_id) + "', " + std::to_string(nsfw_enabled) + ");" +
                      "UPDATE servers SET nsfw_enabled = " + std::to_string(nsfw_enabled) + ", main_channel = " + std::to_string(main_channel_id) + " WHERE id = " + std::to_string(server_id) + ";";
    std::cout << sql << std::endl;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (exit != SQLITE_OK){
        std::cerr << "Error INSERT INTO" << std::endl;
        sqlite3_free(messageError);
    }
    

    return 0;
} 