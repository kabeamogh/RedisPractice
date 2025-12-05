#include <iostream>
#include "RedisDatabase.h"
#include <unordered_map>
#include <fstream> // File Stream (Input/Output)
#include <sstream> // String Stream (Parsing)
#include <mutex>

// Singleton Function. Now we dont have to make a new obj in every cpp file to call the get, set etc functions.
// just RedisDatabase::getInstance().get is enough. getinstance is a func that returns obj.
RedisDatabase& RedisDatabase::getInstance(){ // RedisDatabase::getinstance() class and function taken from the header file.
    // Since we made it static we don't need to create a object first to call the getinstance function in the main function. because the function returns the object itself.
    // RedisDatabase obj; -> obj.getInstance();
    // RedisDatabase& - this is a data type like int and string which holds entire functions like set, get, etc.
    static RedisDatabase instance; // object instance created. with class as Redisdatabase.
    return instance; // returning the object.
}

// creating set and get operations.

// Here RedisDatabase is a class. set is a function.
void RedisDatabase::set(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex>lock(db_mutex);
    kv_store[key]=value;
}

bool RedisDatabase::get(const std::string& key, std::string& value){
    std::lock_guard<std::mutex> lock(db_mutex);

    auto it = kv_store.find(key);

    if(it != kv_store.end()) {
        value = it->second;
        return true;
    }
    return false;
}

// How will delete work .clear func if found then clear
bool RedisDatabase::del(const std::string& key){
    auto it = kv_store.find(key);

    if(it != kv_store.end()){
        kv_store.erase(it);
        return true;
    }
    return false;
}

void RedisDatabase::flushall(){
    if(!kv_store.empty()){
        kv_store.clear();
    }
}

// what are we doing in the lset and lget, we are giving the input in the lset and it stores the value in the form of a string and a hashmap

void RedisDatabase::lpush(const std::string& key, const std::string& value){
    list_store[key].insert(list_store[key].begin(), value);
}

void RedisDatabase::rpush(const std::string& key, const std::string& value){
    list_store[key].push_back(value);
}

void RedisDatabase::hset(const std::string& key, const std::string& field, const std::string& value){
    hash_store[key][field] = value;
}

std::string RedisDatabase::hget(const std::string& key, const std::string& field){
    hash_store.count(key) && hash_store.count(field);
    return hash_store[key][field];
}

bool RedisDatabase::dump(const std::string& filename) {
    std::lock_guard<std::mutex> lock(db_mutex);

    std::ofstream ofs(filename);
    if (!ofs) return false;

    // Save Key-Values (Format: L key value)

    for (const auto& pair : kv_store) {
        ofs << "K " << pair.first << " " << pair.second << "\n";
    }

    // for (int i = 0; i < kv_store.size(); i++){
    //     pair = kv_store[i];
    //     ofs << "K " << pair.first << " " << pair.second << "\n";
    // }

    // B. Save Lists (Format: L key val1 val2)

    for (const auto& pair : list_store) {
        ofs << "L " << pair.first;
        for (const auto& item : pair.second) {
            ofs << " " << item;
        }
        ofs << "\n";
    }

    // C. Save Hashes (Format: H key field value)
    for (const auto& pair : hash_store) {
        for (const auto& field_val : pair.second) {
            ofs << "H " << pair.first << " " << field_val.first << " " << field_val.second << "\n";
        }
    }

    return true;

}

// 2. LOAD FROM DISK (Startup)
bool RedisDatabase::load(const std::string& filename){
    std::lock_guard<std::mutex> lock(db_mutex);

    std::ifstream ifs(filename);
    if (!ifs) return false;

    kv_store.clear();
    list_store.clear();
    hash_store.clear();

    std::string line;
    
    while(std::getline(ifs, line)) {
        std::stringstream ss(line);
        char type;
        ss >> type;

        if (type == 'K') {
            std::string key, val;
            ss >> key >> val;
            kv_store[key] = val;
        }

        else if (type == 'L') {
            std::string key, val;
            ss >> key >> val;
            kv_store[key] = val;
        }

        else if (type == 'L') {
            std::string key, val;
            ss >> key;
            while (ss >> val) {
                list_store[key].push_back(val);
            }
        }
        else if (type == 'H') {
            std::string key, field, val;
            ss >> key >> field >> val;
            hash_store[key][field] = val;
        }
    }
    return true;

}



