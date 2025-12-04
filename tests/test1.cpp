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
    std::cout << "Day 1 Passed: Database works!" << std::endl;
    return 0;

}