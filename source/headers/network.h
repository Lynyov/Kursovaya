#pragma once

#include <map>
#include <string>
#include "hash.h"
#include "exceptions.h"

using namespace std;

class Network
{
public:

    Network(
        const string &address,
        uint16_t port,
        const map<string, string> &database,
        const string &log_path);

    string &getAddress();
    

    uint16_t &getPort();
    

    map<string, string> &getDatabase();

    void start();

    void stop();
    
    void wait();
    
    void auth();
    
    void sum();

private:
    string address;  
    uint16_t port;              
    map<string, string> database;
    int socket;                  
    int client;                 
    string log_path;             
};


