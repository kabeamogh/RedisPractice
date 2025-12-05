#include "../include/RedisServer.h"
#include "../include/RedisDatabase.h"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[]) {
    int port = 6379;
    
    // Allow custom port: ./redis_server 8080
    if (argc >= 2) port = std::stoi(argv[1]);

    std::cout << "--- My Custom Redis Server ---\n";

    // 1. Load Data on Startup
    if (RedisDatabase::getInstance().load("dump.rdb")) {
        std::cout << "📂 Data loaded from dump.rdb\n";
    } else {
        std::cout << "📂 No save file found. Starting fresh.\n";
    }

    RedisServer server(port);

    // 2. Start Server
    server.run();

    return 0;
}