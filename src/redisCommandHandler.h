// check for the input and then parse it, convert it into tokens and store the token in the set function of database.

//#ifndef rediscommandhandler_h
//#define rediscommandhandler_h
//
//#include <iostream>
//#include <vector> // To store the tokens
//#include <algorithm>
//#include <string>
//
//class RedisCommandHandler{
//    public:
//
//    // Check the input.
//
//    std::vector<std::string> checkInput(const std::string& input);
//
//    private:
//
//    // Parse the input when exists.
//
//    std::vector<std::string> parseInput(const std::string& input);
//
//    // Set the Parsedtoken to the existing commands of the Database.
//
//    void handleSet(const std::vector<std::string>& tokens);
//    std::string handleGet(const std::vector<std::string>& tokens);
//
//};
//
//#endif

#ifndef REDIS_COMMAND_HANDLER_H
#define REDIS_COMMAND_HANDLER_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

class RedisCommandHandler {
public:
    // Main entry point
    std::string processCommand(const std::string& input);

private:
    // Parser helper
    std::vector<std::string> parseInput(const std::string& input);

    // Command Handlers
    void handleSet(const std::vector<std::string>& tokens);
    std::string handleGet(const std::vector<std::string>& tokens);
};

#endif