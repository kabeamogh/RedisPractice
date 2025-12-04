#ifndef redisDatabase_h
#define redisDatabase_h

#include <unordered_map>
#include <iostream>
//#include <mutex>
#include <string>
#include <vector>

class RedisDatabase {
    public:
        // class is RedisDatabase and function is getinstance
    // creating singleton logic
    static RedisDatabase& getInstance(); //this is what we need from the cpp file. the main file take this.
    // static here is for the main so we dont have to create object there when calling getInstance.
    
    // creating functions set, get, del, flushall
    void set(const std::string& key, const std::string& value);
    // the variable value in the set function and variable in the get function is different eventhough they have same names.
    bool get(const std::string& key, std::string& value);// we removed const from here because we are getting the value from the set, initially it was empty so it changed.
    bool del(const std::string& key);
    void flushall(); // No need to pass any argument. kv store is private main cant see it flushall can see through private.


    
    void lpush(const std::string& key, const std::string& value);
    void rpush(const std::string& key, const std::string& value);

    void hset(const std::string& key, const std::string& field, const std::string& value);
    std::string hget(const std::string& key, const std::string& field);

    
    private:
    RedisDatabase() = default;
    // creating mutex and kv_store
    
    //std::mutex db_mutex;
    std::unordered_map<std::string,std::string> kv_store;
    std::unordered_map<std::string,std::vector<std::string>> list_store;
    std::unordered_map<std::string,std::unordered_map<std::string,std::string>> hash_store;
    
    // creating get and set functions
};
#endif // redisDatabase_h
// ERROR 4 FIX: You cannot write text after #endif unless it's a comment


