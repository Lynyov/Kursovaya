#include "../../Kursovaya/source/headers/network.h"
#include "../../Kursovaya/source/headers/decoder.h"
#include "../../Kursovaya/source/headers/exceptions.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>

using namespace std;

string LOG_PATH;

/** 
* @brief Читает файл базы данных и заполняет карту парами "имя пользователя - пароль".
* @param path Ссылка на строку, представляющую путь к файлу базы данных.
* @return Карта, содержащая пары "имя пользователя - пароль"
* @throw IOException Если файл не может быть открыт.
* @throw DataDecodeException Если строка в файле не соответствует ожидаемому формату или если произошла ошибка при чтении из файла.
*/

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

    return database;  // Return the database map
}

/**
* @brief Основной цикл для обработки сетевых операций. Эта функция запускает сетевой сервис и входит в бесконечный цикл, в котором она
 * ожидает входящих соединений, аутентифицирует пользователей и выполняет операции.
 * Любые сетевые исключения обрабатываются и регистрируются. Если возникает неожиданное
 * исключение, сетевой сервис останавливается.
 * @param network Указатель на объект Network, который обрабатывает сетевые операции.
 */

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

/**
* @brief Главная точка входа в приложение.
*
* Эта функция инициализирует объект Decoder для парсинга аргументов командной строки,
* получает путь к журналу и базу данных из указанной конфигурации,
* инициализирует объект Network с этими настройками и входит в цикл
* для обработки сетевых операций. Также включает обработку ошибок для сетевых исключений.
*
* @param argc Количество аргументов командной строки.
* @param argv Массив строк аргументов командной строки.
* @return Целое число, представляющее статус завершения программы (0 для успешного завершения).
*/

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
