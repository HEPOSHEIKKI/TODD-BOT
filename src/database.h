#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <bits/stdc++.h>

int initDataBase(const char *filePath);
int createDB(const char *filePath);
int createTable(const char *filePath);
int insertData(const char *filePath, std::uint64_t server_id, std::uint64_t main_channel_id, bool nsfw_enabled);

#endif