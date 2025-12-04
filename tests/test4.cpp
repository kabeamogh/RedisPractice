#include "../include/RedisServer.h"
#include <iostream>

int main() {
    std::cout << "--- Starting Day 4 Server Test ---" << std::endl;
    
    // 1. Create the Server Object (Port 6379)
    RedisServer server(6379);
    
    // 2. Start the Loop (This will block forever)
    server.run();
    
    return 0;
}