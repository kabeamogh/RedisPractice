#include "redisServer.h"
#include "redisDatabase.h"
#include <iostream>
#include <sys/socket.h>
// kernel library 
// gives func like socket, setsockopt, bind, listen, accept, constant sockstream also comes from here.
// its a os api connects us to the hardware directly
#include <netinet/in.h>
// internet address structure
// gives us AF_INET INADDR_ANY
#include <unistd.h>
// function it give close()
#include <cstring> // gives memset
#include <signal.h> // For signal() --> Day 6 // gives signal() and SIGINT
// "When SIGINT (Ctrl+C) happens, run 'signalHandler'"
//    signal(SIGINT, signalHandler); // In the main function


#include <vector> // --> Day 5  
#include <thread> // --> Day 5 // wait times (performance)

static RedisServer* globalServer = nullptr;

void signalHandler(int signum) {
    std::cout << "\n Interrupt Received (" << signum << "). Saving & Stopping...\n";
    if (globalServer) {
        globalServer->shutdown();
    }
    exit(signum);
}

// we call the void run from the header
// create atruct run which include all the int from the private
// and we call it as class constructors they are different from the structs.

//struct RedisServer::run{
//    port, system_socket(-1), atomic<bool> running = false;
//};

RedisServer::RedisServer(int port): port(port), system_socket(-1), running(false) {

    globalServer = this;

    signal(SIGINT, signalHandler);

} // Remember class constructor.

void RedisServer::shutdown() {
    running = false;

    if (RedisDatabase::getInstance().dump("dump.rdb")) {
        std::cout << "Database saved to dump.rdb\n";
    } else {
        std::cerr << "Failed to save database.\n";
    }

    close(system_socket);
}

// now we arite the func code for this run func.

void RedisServer::run(){
    running = true;

    // Create Socket -> Tell the kernel

    // Domain (AF_INET) -> "Where are we driving?" (Network Layer)(IPv4/IPv6) -> give contract to network layers in the kernel itself

    // We got AF_INET - address for the internet - IPv4 - Internet Protocol version 4 (layer 3 network layer), sockstream - TCP (layer 4 transport layer).
    // AF_INET (IPv4): "I want to talk to computers on the internet using addresses like 192.168.1.1
    // AF_INET6 (IPv6): "I want to talk using modern 128-bit addresses."

    // Type (SOCK_STREAM) -> "What vehicle are we using?" (Transport Layer)(TCP/UDP)

    // SOCK_STREAM (TCP): "I need a Truck. Load everything in, drive it there, and ensure nothing falls out. If it does, go back and get it." (Reliable, Ordered, Connection-based).
    // SOCK_DGRAM (UDP): "I need a Motorcycle courier. Give them a single packet and tell them to drive fast. If they crash, oh well." (Unreliable, Fast, Connection-less).

    // Protocol (0) -> "Which engine?"
    // Deault one that SOCK_STREAM provides for 0 is TCP.

    // 0 tells OS: "I chose SOCK_STREAM, so auto-select TCP."
    // socket(AF_INET, SOCK_STREAM, 0); 
    //
    // 6 tells OS: "I chose SOCK_STREAM, and I demand TCP."
    // socket(AF_INET, SOCK_STREAM, 6); // Works perfectly.

    system_socket = socket(AF_INET, SOCK_STREAM, 0);
    // inside the func its just the argument and a constant integer which acts as a validation code for the kernel.
    // in the bind function it is where we actually tell the kernel to look up the actual code and bind together with the target.

    // Socket: We want IPv4.
    // Bind : We told the struct. we are providing you an IPv4 address.

    if (system_socket < 0){
        std::cerr << "Failed to create socket";
        return;
    }

    // When you close the connection in the terminal using ctrl + c, and immediately run the program again
    // it fails because after stopping it wont dirctly free the port 6379. it takes up 60 seconds of wait. for this problem
    // the solution is to use So_REUSEADDR function.
    // I authorize this program to hijack the port, even if it's technically still in 'timeout' mode.



    // SET OPTIONS

    int opt = 1;
    setsockopt(system_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Our Target is system_socket where we opened the socket.
    // SOL_SOCKET = Socket Option Level stay in the kernel dont go to os model
    // SO_REUSEADDR = Socket Option reuse the same address that we used to connect in the terminal. the port we used in the terminal. if it is true opt = 1 also send its size 4 bytes

    // BIND

    sockaddr_in server_addr{}; // sockaddr_in is the struct which we have made blank using a main box called server_addr.
    server_addr.sin_family = AF_INET; // inside server_addr we create an actual integer called .sin_family. which hold ip
    server_addr.sin_addr.s_addr = INADDR_ANY; // here we first created sin_addr and inside it is the integer s_addr. which holds universal adaptor and translates ip address to 0.0.0.0
    server_addr.sin_port = htons(port); // integer sin_port inside server_addr which holds port number

    // here we binded all using bind function the system_socket that we make with the structure and the size of struct is 16 bytes and if it doesnt exist <0 then throw error

    if (bind(system_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "❌ Failed to bind to port " << port << "\n";
        return;
    }

    // ## The Universal Signature
    // Almost every socket function looks like this:
    // int syscall_name( TARGET, INSTRUCTIONS, PAYLOAD, SIZE );

    // LISTEN

    if (listen(system_socket, 5) < 0) {
        std::cerr << "❌ Failed to listen\n";
        return;
    }
    // listen function takes the target and sets the limit that it will only take 1 + 5 terminal request to connection and rest all are kept in limbo.

    std::cout << "🎧 Server listening on port " << port << "...\n";

    // ACCEPT LOOP

    while (running) {
        sockaddr_in client_addr;
        // "Here is a blank form (client_addr)."
        // "Accept the next call and write the caller's details onto this form.
        socklen_t client_len = sizeof(client_addr);
        
        
        int client_socket = accept(system_socket, (struct sockaddr*)&client_addr, &client_len);
        // Okay, someone connected from IP 192.168.1.5, Port 49152. I have written this into your struct."

        if (client_socket < 0){
            if (running) std::cerr << "Error Accepting\n";
            continue;
        }

        std::cout << "Client Connected! Spawning Thread...\n";

        // Understand the functions below tomorrow

        threads.emplace_back([this, client_socket]() { // By including <thread>, you allow User A to be in a while(true) loop talking to you, while User B connects simultaneously. Without it, User B would have to wait until User A disconnects.
            // [this] means the commandHandler which takes argument for inside response to work.
            // it is a pointer to find variable cmdHandler.
            char buffer[1024]; // array of chars filled with 0s

            while (true) {
                memset(buffer, 0, sizeof(buffer)); // memoryset taking arg 1 the memory we wanna clean the value to write and its size.
                // Round 1: Client sends "HELLO".

                // You write on the board: [H][E][L][L][O]
                 
                // You read 5 letters. Perfect.
                 
                // Round 2: Client sends "BYE".
                 
                // Without memset: You overwrite the first 3 letters.
                 
                // Board looks like: [B][Y][E][L][O]
                
                // With memset:

                // Before Round 2, you wipe the board clean (fill with zeros).
                 
                // Board: [0][0][0][0][0]...
                 
                // Write "BYE": [B][Y][E][0][0]...

                int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

                // Network: Packets arrive with "SET A 10".

                // recv():
                 
                // Copies "SET A 10" into buffer.
                 
                // Returns 8 (the length).
                 
                // std::string request(buffer, 8):
                 
                // Looks at buffer.
                 
                // Counts 8 characters.
                 
                // Creates a string object "SET A 10".
                 
                // Now your cmdHandler can inspect this string!

                if (bytes <= 0){
                    std::cout << "Client Disconnected.\n";
                    break;
                }

                std::string request(buffer, bytes);

                std::string response = cmdHandler.processCommand(request);

                send(client_socket, response.c_str(), response.size(), 0);
            }

            close(client_socket);
        });

        //if (client_socket >= 0) { // true 
        //    std::cout << "✅ A Client Connected!\n";
        //    
        //    
        //    std::string msg = "+Hello from Day 4 Server\r\n";
        //    send(client_socket, msg.c_str(), msg.size(), 0);
        //    
        //    close(client_socket); // close the accept.
        //    std::cout << "👋 Client Disconnected.\n";
        //}
    }
}





