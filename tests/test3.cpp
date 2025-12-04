#include <iostream>
#include "redisCommandHandler.h"
#include "redisDatabase.h"
#include <cassert>
#include <string>
#include <vector>

int main(){

    RedisCommandHandler handler;

    // Test 1 : Simple Telnet Style.

    std::string input1 = "SET name Amogh";

    handler.checkInput(input1);

    std::string val;

    if (RedisDatabase::getInstance().get("name", val)) {
        std::cout << val << " Sucess" << std::endl;
    } else {
        std::cout << "VERIFICATION: Key not found (Fail!)" << std::endl;
    }

    // Test 2 : Complex RESP Style.

    std::string rawRESP = "*3\r\n$3\r\nSET\r\n$4\r\ncity\r\n$5\r\nParis\r\n";

    handler.checkInput(rawRESP);

    std::string cityVal;

    RedisDatabase::getInstance().get("city", cityVal);
    std::cout << cityVal << " Success" << std::endl;

    return 0;

}