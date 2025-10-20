#include "network.h"

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <cstdint>
#include <vector>

// Конструктор
Network::Network(
    const string &address, uint16_t port,
    const map<string, string> &database,
    const string &log_path)
{
    this->address = address;
    this->port = port;
    this->database = database;
    this->log_path = log_path;
    this->client = -1;
    this->socket = -1;
}

void Network::start()
{
    this->socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket == -1)
        throw NetworkException("Failed to create socket", "Network::Network", this->log_path, true);

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);
    server_addr.sin_addr.s_addr = inet_addr(this->address.c_str());

    if (bind(this->socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        this->stop();
        throw NetworkException("Failed to bind socket", "Network::Network", this->log_path, true);
    }

    if (listen(this->socket, 5) == -1)
    {
        this->stop();
        throw NetworkException("Failed to listen on socket", "Network::Network", this->log_path, true);
    }
}

string &Network::getAddress()
{
    return this->address;
};

uint16_t &Network::getPort()
{
    return this->port;
};

map<string, string> &Network::getDatabase()
{
    return this->database;
};

// Метод для ожидания соединений
void Network::wait()
{
    cout << "Server listening on " << this->address << ":" << this->port << endl;
    this->client = accept(this->socket, nullptr, nullptr);
    if (this->client == -1)
    {
        throw NetworkException("Failed to accept connection", "Network::wait", this->log_path, false);
    }
    cout << "Accepted connection from client" << endl;
}

// Метод для аутентификации
void Network::auth()
{
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes_read = recv(this->client, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
        throw NetworkException("Failed to read login from client", "Network::auth", this->log_path, false);
    string login(buffer);
    cout << "Received login: " << login << endl;

    auto it = this->database.find(login);
    if (it == this->database.end())
    {
        string error_message = "ERR";
        send(this->client, error_message.c_str(), error_message.size(), 0);
        throw NetworkException("Login not found in database: " + login, "Network::auth", this->log_path, false);
    }

    string salt = getSalt();
    send(this->client, salt.c_str(), salt.size(), 0);
    cout << "Sent salt: " << salt << endl;

    memset(buffer, 0, sizeof(buffer));
    bytes_read = recv(this->client, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read <= 0)
        throw NetworkException("Failed to read hash from client", "Network::auth", this->log_path, false);

    string client_hash(buffer);
    cout << "hash: " << client_hash << endl;

    string password = it->second;
    string data_to_hash = salt + password;
    string server_hash = getHash(data_to_hash);
    cout << "hash: " << server_hash << endl;

    if (client_hash != server_hash)
    {
        string error_message = "ERR";
        send(this->client, error_message.c_str(), error_message.size(), 0);
        throw NetworkException("Hash mismatch", "Network::auth", this->log_path, false);
    }
    string success_message = "OK";
    send(this->client, success_message.c_str(), success_message.size(), 0);
    cout << "Client authenticated successfully" << endl;
}

// Метод для вычисления суммы векторов
void Network::sum()
{
    uint32_t num_vectors;
    uint32_t num_values;
    int64_t value;

    // Чтение количества векторов от клиента
    ssize_t bytes_read = recv(this->client, &num_vectors, sizeof(num_vectors), 0);
    if (bytes_read <= 0)
    {
        cerr << "Error: Failed to read number of vectors from client\n";
        throw NetworkException("Failed to read number of vectors from client", "Network::calc", this->log_path, false);
    }
    cout << "Received number of vectors: " << num_vectors << endl;

    // Обработка каждого вектора
    for (uint32_t i = 0; i < num_vectors; ++i)
    {
        // Чтение количества значений в векторе от клиента
        bytes_read = recv(this->client, &num_values, sizeof(num_values), 0);
        if (bytes_read <= 0)
            throw NetworkException("Failed to read number of values from client", "Network::calc", this->log_path, false);

        cout << "Received number of values for vector " << i + 1 << ": " << num_values << endl;

        int64_t sum = 0;
        for (uint32_t j = 0; j < num_values; ++j)
        {
            // Чтение значения вектора от клиента
            bytes_read = recv(this->client, &value, sizeof(value), 0);
            if (bytes_read <= 0)
                throw NetworkException("Failed to read vector value from client", "Network::calc", this->log_path, false);

            cout << "Received value for vector " << i + 1
                 << ", element " << j + 1 << ": " << value << endl;
            sum += value;
        }

        // Отправка суммы обратно клиенту
        bytes_read = send(this->client, &sum, sizeof(sum), 0);
        if (bytes_read <= 0)
            throw NetworkException("Failed to send sum to client", "Network::calc", this->log_path, false);

        cout << "Sent sum for vector " << i + 1 << ": " << sum << endl;
    }

    cout << "Calculated and sent sums for all vectors" << endl;
}

// Метод для закрытия клиентского соединения
void Network::stop()
{
    ::close(this->client);
    ::close(this->socket);
    this->client = -1;
    this->socket = -1;
}
