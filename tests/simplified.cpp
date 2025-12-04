#include <iostream>
#include <string>
#include <unordered_map>

class RedisDatabase {
public:
    // ---------------------------------------------------------
    // 1. THE SINGLETON (The "RedisDatabase&" Datatype is here)
    // ---------------------------------------------------------
    static RedisDatabase& getInstance() {
        static RedisDatabase instance; // The one true object
        return instance;
    }

    // ---------------------------------------------------------
    // 2. THE LOGIC (Set/Get)
    // ---------------------------------------------------------
    void set(std::string key, std::string value) {
        kv_store[key] = value;
    }

    bool get(std::string key, std::string& value) {
        if (kv_store.find(key) != kv_store.end()) {
             value = kv_store[key];
             return true;
        }
        return false;
    }

private:
    // 3. THE DATA
    RedisDatabase() = default; // Private constructor
    std::unordered_map<std::string, std::string> kv_store;
};