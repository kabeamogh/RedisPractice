//#include "redisCommandHandler.h"
//#include "redisDatabase.h"
//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//
//class redisCommandHandler{
//    public:
//
//    // Check the input.
//
//    std::vector<std::string> checkInput(std::vector<std::string>& input, std::vector<std::string>& tokens){
//        if(!input.empty()){
//            return tokens;
//        }
//    }
//
//    private:
//
//    // Parse the input when exists.
//
//    std::vector<std::string> parseInput(std::vector<std::string>& input, std::vector<std::string>& tokens){
//        // The Raw String: *3\r\n$3\r\nSET\r\n$4\r\nname\r\n$5\r\nadmin\r\n
//
//        // Check if our input string starts from * (telnec client) if yes then return tokens as it is.
//
//        std::string s;
//
//        for(int i = 0; i < input.size(); i++){
//            s = input[i];
//            // Now the s becomes the string in the input.
//            if(s[0] != '*'){
//                tokens.push_back(s);
//                return tokens;
//          }
//        }
//
//        // If the string is now a redis language, we convert it into a understood language.
//
//        // If the string starts from the * and the next index is and integer that will be the size of the tokens.
//
//        int n;
//        
//        if(s[0] == '*' && s[1] == 'n'){
//            n = std::stoi(s.substr(1)); // string to integer
//            // n = s[1] - '0';
//            tokens.resize(n);
//        } else {
//            return tokens;
//        }
//
//        // If I reach \r\n and then the next letter is $ then go to next \r\n and if not $ then push the length till the next \r\n.
//
//        int pos = 2;
//        std::string ns = "\r\n";
//        int pos2 = 0;
//
//        // The Raw String: *3\r\n$3\r\nSET\r\n$4\r\nname\r\n$5\r\nadmin\r\n
//
//        int len;
//
//        for (int i = 0; i < s.length(); i++){
//            char c = s[i];
//            if (c >= '0' && c <= '9'){
//                pos++;
//            }
//
//            if (s[i] == '$'){
//                pos++;
//            }
//            size_t parsing = s.find(ns, pos);
//            // parsing will be 2. It points to the start of the \r.
//            // If 'pos' is at '$' and 'parsing' is at '\r'
//            size_t Start = pos; // Skip the '$'
//            size_t Length = parsing - pos; //
//            
//            std::string len = s.substr(Start, Length);
//            
//            tokens.push_back(len);
//
//        }
//        return tokens;
//
//    }
//
//    // Set the Parsedtoken to the existing commands of the Database.
//
//    void redisCommandHandler::handleSet(std::vector<std::string>& input, std::vector<std::string>& tokens){
//        if (tokens.size() < 2){
//            std::cout << "Invalid Input";
//        }
//        
//        if (tokens.size() == 3){
//            std::cout << "Accepted" << std::endl;
//
//            RedisDatabase::getInstance().set(tokens[1], tokens[2]);
//        }
//    }
//
//    std::string redisCommandHandler::handleGet(std::vector<std::string>& input, std::vector<std::string>& tokens){
//        if (tokens.size() < 2){
//            std::cout << "Invalid Input";
//        }
//        
//        if (tokens.size() == 3){
//            std::cout << "Accepted" << std::endl;
//
//            std::string val; 
//
//            bool found = RedisDatabase::getInstance().get(tokens[1], val);
//
//            if(found){
//                return val;
//            } else {
//                std::cout<< "Not Found"; // I promised to return a string.
//            }
//        }
//    }
//
//
//
//};
//


// The Reason my code failed was because the logic was only functionable for the raw string that got provided to me.
// 1. There was no reason to create a vector<string> when only a string was given.
// 2. the first if condition where if empty was wrong logic wise.
// 3. The for loop iterated through every index of the string but pos would get shifted further
// to the \r\n but again because of for loop we would come back.
// 4. And for other parts the logic was only correct for the specific RESP Protocol.

 #include "RedisCommandHandler.h"
 #include "RedisDatabase.h" // Crucial: Include the DB so we can call getInstance()
 #include <iostream>
 #include <vector>
 #include <string>
 #include <algorithm>
 
 // 1. MAIN ENTRY POINT
 std::string RedisCommandHandler::processCommand(const std::string& input) {
     // Safety check: If empty, return empty
     if (input.empty()) {
         return "-Error: Empty input\r\n";
     }
 
     // 1. Parse the raw string into tokens
     std::vector<std::string> tokens = parseInput(input);
 
     // 2. Route to correct handler
     if (!tokens.empty()) {
         // Convert command to Uppercase (so "set" works like "SET")
         std::string cmd = tokens[0];
         std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
 
         if (cmd == "SET") {
             handleSet(tokens);
             return "+OK\r\n";
         } 
         else if (cmd == "GET") {
             // We capture the result (in a real server, we would return this)
             std::string val = handleGet(tokens);
             return val;
         }
     }
     
     return "-Error: Unknown command\r\n";
 }
 
 // 2. THE PARSER
 std::vector<std::string> RedisCommandHandler::parseInput(const std::string& input) {
     std::vector<std::string> tokens;
 
     // --- CASE A: TELNET CLIENT (Simple text like "SET k v") ---
     // Your Manual Loop Logic
     if (input[0] != '*') {
         std::string word = "";
         for (size_t i = 0; i < input.size(); i++) {
             char c = input[i];
             if (c == ' ' || c == '\n' || c == '\r') {
                 if (!word.empty()) {
                     tokens.push_back(word);
                     word = "";
                 }
             } else {
                 word += c;
             }
         }
         // Push the last word if exists
         if (!word.empty()) {
             tokens.push_back(word);
         }
         return tokens;
     }
 
     // --- CASE B: RESP CLIENT (starts with *) ---
     // Your "find/substr" Logic
     
     int n = 0;
     size_t currentPos = 0;
 
     // 1. Get the number of elements (*3)
     if (input[0] == '*') {
         size_t lineEnd = input.find("\r\n", 0);
         std::string numberStr = input.substr(1, lineEnd - 1);
         n = std::stoi(numberStr);
         currentPos = lineEnd + 2; // Move past \r\n
     } else {
         return tokens;
     }
 
     // 2. Loop n times to extract words
     for (int i = 0; i < n; i++) {
         // Expecting $Length\r\nValue\r\n
         if (input[currentPos] == '$') {
             // Read Length
             size_t lineEnd = input.find("\r\n", currentPos);
             int len = std::stoi(input.substr(currentPos + 1, lineEnd - (currentPos + 1)));
             
             currentPos = lineEnd + 2; // Move past \r\n to start of text
 
             // Read Text
            
             tokens.push_back(input.substr(currentPos, len));
             
                     
             currentPos += len + 2; // Move past text + \r\n
         }
     }
 
     return tokens;
 }
 
 // 3. THE HANDLERS
 
 void RedisCommandHandler::handleSet(const std::vector<std::string>& tokens) {
     if (tokens.size() < 3) {
         std::cout << "Invalid Input" << std::endl;
         return;
     }
     
     std::cout << "Accepted SET" << std::endl;
     // Call the Singleton
     RedisDatabase::getInstance().set(tokens[1], tokens[2]);
 }
 
 std::string RedisCommandHandler::handleGet(const std::vector<std::string>& tokens) {
     if (tokens.size() < 2) {
         std::cout << "Invalid Input" << std::endl;
         return "";
     }
     
     std::cout << "Accepted GET" << std::endl;
     std::string val; 
     
     // Call the Singleton
     bool found = RedisDatabase::getInstance().get(tokens[1], val);
     
     if (found) {
         return "$" + std::to_string(val.length()) + "\r\n" + val + "\r\n";;
     } else {
         std::cout << "Not Found" << std::endl;
         return "$-1\r\n";
     }
 }
 
 // Use printf '*2\r\n$3\r\nGET\r\n$6\r\nmyhero\r\n' | nc localhost 6379
 // single quotes.