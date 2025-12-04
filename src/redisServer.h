
#ifndef redisServer_h
#define rediServer_h

#include <atomic> // make the variable operation indivisible (entire taks done by the cpu is done as it is as being told not like i++)
#include <vector>
#include <thread>
#include "RedisCommandHandler.h"


class RedisServer{
    public:
    RedisServer(int port);
    void run();
    void shutdown();

    private: // create them here. we use them in the structure in cpp file for void run.
    int port;
    int system_socket;
    std::atomic<bool> running;

    RedisCommandHandler cmdHandler;
    std::vector<std::thread> threads;
};

#endif

