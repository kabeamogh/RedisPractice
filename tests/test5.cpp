#include "RedisServer.h"
#include <iostream>

int main() {
    std::cout << "--- Starting Day 5 Server (Integration Test) ---" << std::endl;
    
    // 1. Create the Server Object (Port 6379)
    RedisServer server(6379);
    
    // 2. Start the Loop
    server.run();
    
    return 0;
}