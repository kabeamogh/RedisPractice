#include <iostream>
#include "RedisDatabase.h"
#include <unordered_map>
//#include <mutex>

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
    //std::lock_guard<std::mutex>lock(db_mutex);
    kv_store[key]=value;
}

bool RedisDatabase::get(const std::string& key, std::string& value){
    //std::lock_guard<std::mutex> lock(db_mutex);

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



