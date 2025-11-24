#include "decoder.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cstring>

// Конструктор принимает параметры командной строки
Decoder::Decoder()
{
    this->address = "127.0.0.1";
    this->port = 33333;
    this->db_path = "/etc/vcalc.conf";
    this->log_path = "/var/log/vcalc.log";
}

string &Decoder::getDataBasePath()
{
    return this->db_path;
};

string &Decoder::getLogPath()
{
    return this->log_path;
};

string &Decoder::getAddres()
{
    return this->address;
};

int &Decoder::getPort()
{
    return this->port;
};

// Метод для парсинга аргументов
void Decoder::parse(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            this->showHelp();
            exit(0);
        }
        else if ((strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--database") == 0) && (i + 1 < argc))
        {
            this->db_path = argv[i + 1];
            ++i;
        }
        else if ((strcmp(argv[i], "-l") == 0 || strcmp(argv[i], "--log") == 0) && (i + 1 < argc))
        {
            this->log_path = argv[i + 1];
            ++i;
        }
        else if ((strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) && (i + 1 < argc))
        {
            this->port = stoi(argv[i + 1]);
            ++i;
        }
        else if ((strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--address") == 0) && (i + 1 < argc))
        {
            this->address = argv[i + 1];
            ++i;
        }
        else
        {
            throw ArgsDecodeException("Invalid argument", "Decoder::parse", this->log_path, true);
        }
    }

    if (this->port <= 0 || this->port > 65535)
        throw ArgsDecodeException("Invalid port number", "Decoder::parse", this->log_path, true);

    std::cout << "Parsed arguments: --database " << this->db_path
              << " --og " << this->log_path
              << " --address " << this->address
              << " --port " << this->port << endl;
}

// Метод для вывода справки
void Decoder::showHelp() const
{
    std::cout << "Usage: program [options]\nOptions:\n"
              << "  -h, --help                 Show this help message and exit\n"
              << "  -d, --path_to_database     Path to the database file (default /etc/vcalc.conf)\n"
              << "  -l, --path_to_log          Path to the log file (default /var/log/vcalc.log)\n"
              << "  -p, --port                 Port number (default: 33333)\n"
              << "  -a, --address              Address (default: 127.0.0.1)\n";
}

