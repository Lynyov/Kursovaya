#include "../../Kursovaya/source/headers/network.h"
#include "../../Kursovaya/source/headers/decoder.h"
#include "../../Kursovaya/source/headers/exceptions.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

string LOG_PATH;
map<string, string> getDataBase(string &path)
{
    ifstream input_file(path);
    if (!input_file.is_open())
        throw IOException("Failed to open database file for reading", "getDataBase", LOG_PATH, true);

    string line;
    map<string, string> database;

    while (getline(input_file, line))
    {
        auto delimiter_pos = line.find(':');
        if (delimiter_pos != string::npos)
        {
            string username = line.substr(0, delimiter_pos);
            string password = line.substr(delimiter_pos + 1);
            database[username] = password;
        }
        else
            throw DataDecodeException("Invalid line format in database file", "getDataBase", LOG_PATH, true);
    }

    if (input_file.bad())
        throw DataDecodeException("Error reading database file", "getDataBase", LOG_PATH, true);

    input_file.close();

    cout << "Read database from file: " << path << endl;

    return database;
}


void loop(Network *network)
{
    network->start();
    while (true)
    {
        try
        {
            network->wait();
            network->auth();
            network->sum();
        }
        catch (const NetworkException &e)
        {
            cerr << e.what() << endl;
        }
        catch (const exception &e)
        {
            network->stop();
            cerr << e.what() << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    Decoder decoder;
    decoder.parse(argc, argv);
    LOG_PATH = decoder.getLogPath();
    map<string, string> database = getDataBase(decoder.getDataBasePath());

    Network network(
        decoder.getAddres(), decoder.getPort(),
        database, decoder.getLogPath());
    network.start();

    while (true)
    {
        try
        {
            network.wait();
            network.auth();
            network.sum();
        }
        catch (const NetworkException &e)
        {
            cerr << e.what() << endl;
        }
        catch (const exception &e)
        {
            network.stop();
            cerr << e.what() << endl;
        }
    }

    return 0;
}
