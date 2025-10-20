#pragma once

#include <exception>
#include <string>

using namespace std;

/**
* @brief Базовый класс для исключений.
*/
class Exception : public std::exception {
public:
	/**
    * @brief Конструктор класса Exception.
    * @param name Имя исключения.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    Exception(const string &name, const string &message, const string &func, const string &log_path, bool critical);
    
    /**
    * @brief Метод для получения сообщения об ошибке.
    * @return Сообщение об ошибке.
    */
    const char *what() const noexcept override;

protected:

	/**
    * @brief Метод для логирования исключения.
    */
    void logException() const;

    string name; ///< Имя исключения.
    string func; ///< Имя функции, в которой возникло исключение.
    mutable string message; ///< Сообщение об ошибке.
    string log_path; ///< Путь к файлу журнала.
    bool critical; ///< Флаг критичности исключения.
};

/**
* @brief Класс для исключений ввода-вывода.
*/
class IOException : public Exception {
public:
	/**
    * @brief Конструктор класса IOException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    IOException(const string &message, const string &func, const string &log_path, bool critical);
};

/**
* @brief Класс для исключений при декодировании аргументов.
*/
class ArgsDecodeException : public Exception {
public:
	/**
    * @brief Конструктор класса ArgsDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    ArgsDecodeException(const string &message, const string &func, const string &log_path, bool critical);
};

/**
* @brief Класс для исключений при декодировании данных.
*/
class DataDecodeException : public Exception {
public:
	/**
    * @brief Конструктор класса DataDecodeException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    DataDecodeException(const string &message, const string &func, const string &log_path, bool critical);
};

/**
* @brief Класс для исключений аутентификации.
*/
class AuthException : public Exception {
public:
	/**
    * @brief Конструктор класса AuthException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    AuthException(const string &message, const string &func, const string &log_path, bool critical);
};

/**
* @brief Класс для сетевых исключений.
*/
class NetworkException : public Exception {
public:
	/**
    * @brief Конструктор класса NetworkException.
    * @param message Сообщение об ошибке.
    * @param func Имя функции, в которой возникло исключение.
    * @param log_path Путь к файлу журнала.
    * @param critical Флаг критичности исключения.
    */
    NetworkException(const string &message, const string &func, const string &log_path, bool critical);
};

