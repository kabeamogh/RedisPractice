#include "RedisDatabase.h"
#include <iostream>
#include <cassert> // use assert function.

int main() {
    RedisDatabase& db = RedisDatabase::getInstance();
    db.set("mykey", "100");
    
    std::string val; // val is empty.
    bool found = db.get("mykey", val); // get the username mykey
    
    assert(found == true); // if true or not. my key there or not.
    assert(val == "100"); // check if value val 100 or not

    bool found2 = db.del("mykey");

    assert(found2 == true);
    
    db.flushall();

    db.rpush("Apples", "10");
    db.lpush("bananas", "20");

    db.hset("user100", "John", "23");
    
    
    assert(db.hget("user100", "John") == "23");
    // here we check the value of the key so we passed key and field in arg and kept it as const.

    std::cout << "Day 2 Passed: Database works!" << std::endl;
    return 0;
}