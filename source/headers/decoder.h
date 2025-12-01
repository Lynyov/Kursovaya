/** 
* @file decoder.h
* @brief Определение класса интерфейса.
* @details Этот файл содержит определения классов для обработки параметров командной строки, чтения базы данных паролей и запуска сервера.
* @date 01.12.2025
* @version 1.0
* @author Лынев А. С.
*/

#pragma once

#include <map>
#include <string>
#include "exceptions.h"
#include "network.h"

using namespace std;

/** 
* @brief Класс для управления интерфейсом программы.
*/
class Decoder
{
public:

	/**
    * @brief Конструктор принимает параметры командной строки.
    * @throw ArgsDecodeException Если аргументы командной строки некорректны.
    */
    Decoder();

	/**
    * @brief Метод для парсинга аргументов командной строки.
    * @param argc Количество аргументов командной строки.
    * @param argv Аргументы командной строки.
    * @throw ArgsDecodeException Если аргументы командной строки некорректны.
    */
    void parse(int argc, char *argv[]);
    
    /**
    * @brief Метод для получения справки.
    * @return Справка по использованию.
    */
    void showHelp() const;
    
    /**
    * @brief Метод для получения пути к базе данных.
    * @return Ссылка на строку с путем к базе данных.
    */
    string &getDataBasePath();
    
    /**
    * @brief Метод для получения пути к файлу логов.
    * @return Ссылка на строку с путем к файлу логов.
    */
    string &getLogPath();
    
    /**
    * @brief Метод для получения адреса сервера.
    * @return Ссылка на строку с адресом сервера.
    */
    string &getAddres();
    
    /**
    * @brief Метод для получения порта сервера.
    * @return Ссылка на целое число с номером порта сервера.
    */
    int &getPort();

private:
    string db_path;  ///< Путь к файлу базы данных.
    string log_path; ///< Путь к файлу логов.
    int port;        ///< Порт.
    string address;  ///< Адрес.
};


