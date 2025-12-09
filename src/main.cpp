#include "redisServer.h"
#include "redisDatabase.h"
#include <iostream>
#include <thread>
#include <chrono> // C++ Standard Library header that handles Time.


int main(int argc, char* argv[]) {
    int port = 6379;

    if (argc >= 2) port = std::stoi(argv[1]);

    std::cout << "--- My Custom Redis Server ---\n";

    if (RedisDatabase::getInstance().load("dump.rdb")) {
        std::cout << "Data loaded from dump.rdb\n";
    } else {
        std::cout << "No save file found. Starting fresh.\n";
    }

    

    RedisServer server(6379);
    
    // 2. Start the Loop
    server.run();
    
    return 0;
}