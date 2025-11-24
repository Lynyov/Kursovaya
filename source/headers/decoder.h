
#pragma once

#include <map>
#include <string>
#include "exceptions.h"
#include "network.h"

using namespace std;

class Decoder
{
public:

    Decoder();
    void parse(int argc, char *argv[]);
    void showHelp() const;
    string &getDataBasePath();
    string &getLogPath();
    string &getAddres();
    int &getPort();

private:
    string db_path;
    string log_path;
    int port;        
    string address;
};


