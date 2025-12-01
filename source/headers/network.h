/** 
* @file network.h
* @brief Определения классов для управления сетевым взаимодействием.
* @details Этот файл содержит определения классов для управления сетевыми подключениями и передачей данных.
* @date 01.12.2025
* @version 1.0
* @author Лынев А. С.
*/

#pragma once

#include <map>
#include <string>
#include "hash.h"
#include "exceptions.h"

using namespace std;

/** 
* @brief Класс для управления сетевым подключением и взаимодействием.
*/

class Network
{
public:
	/**
    * @brief Конструктор класса Network.
    * @param address Адрес сервера.
    * @param port Порт сервера.
    * @param database База данных пользователей.
    * @param log_path Путь к файлу логов.
    */
    Network(
        const string &address,
        uint16_t port,
        const map<string, string> &database,
        const string &log_path);

	/**
    * @brief Метод для получения адреса сервера.
    * @return Адрес сервера.
    */
    string &getAddress();
    
    /**
    * @brief Метод для получения порта сервера.
    * @return Порт сервера.
    */
    uint16_t &getPort();
    
    /**
    * @brief Метод для получения базы данных пользователей.
    * @return База данных пользователей.
    */
    map<string, string> &getDatabase();
    
    /**
    * @brief Метод для старта работы сервера.
    */
    void start();
    
    /**
    * @brief Метод для закрытия основного соединения.
    */
    void stop();
    
    /**
    * @brief Метод для ожидания соединений.
    * @throw NetworkException Если не удалось создать, привязать или слушать сокет.
    */
    void wait();
    
    /**
    * @brief Метод для аутентификации пользователя.
    * @throw NetworkException Если не удалось прочитать логин, найти логин в базе данных, отправить или получить данные от клиента.
    */
    void auth();
    
    /**
    * @brief Метод для вычисления суммы значений в векторах.
    * @throw NetworkException Если не удалось прочитать данные от клиента или отправить результат.
    */
    void sum();

private:
    string address;               ///< Адрес сервера.
    uint16_t port;                ///< Порт сервера.
    map<string, string> database; ///< База данных пользователей.
    int socket;                   ///< Сокет сервера.
    int client;                   ///< Сокет клиента.
    string log_path;              ///< Путь к файлу логов.
};


