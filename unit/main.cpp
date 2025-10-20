#include "../../Kursovaya/source/headers/network.h"
#include "../../Kursovaya/source/headers/decoder.h"
#include "../../Kursovaya/source/headers/exceptions.h"
#include "../../Kursovaya/source/headers/hash.h"
#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <map>

// Функция для сравнения карт
bool compareMaps(const std::map<std::string, std::string>& map1, const std::map<std::string, std::string>& map2) {
    if (map1.size() != map2.size()) return false;
    for (auto& pair : map1) {
        if (map2.find(pair.first) == map2.end() || map2.at(pair.first) != pair.second) {
            return false;
        }
    }
    return true;
}

// Тесты для класса Decoder

// Тестирование конструктора и методов получения значений
TEST(DecoderConstructorAndGetters) {
    Decoder decoder;

    CHECK_EQUAL("127.0.0.1", decoder.getAddres());
    CHECK_EQUAL(33333, decoder.getPort());
    CHECK_EQUAL("/etc/vcalc.conf", decoder.getDataBasePath());
    CHECK_EQUAL("/var/log/vcalc.log", decoder.getLogPath());
}

// Тестирование парсинга аргументов командной строки
TEST(DecoderParseArguments) {
    Decoder decoder;
    const char* argv[] = {"program", "--database", "test_db.txt", "--log", "test_log.txt", "--port", "12345", "--address", "192.168.1.1"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    decoder.parse(argc, const_cast<char**>(argv));
    
    CHECK_EQUAL("192.168.1.1", decoder.getAddres());
    CHECK_EQUAL(12345, decoder.getPort());
    CHECK_EQUAL("test_db.txt", decoder.getDataBasePath());
    CHECK_EQUAL("test_log.txt", decoder.getLogPath());
}

// Тестирование вызова справки
TEST(DecoderShowHelp) {
    Decoder decoder;
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    
    decoder.showHelp();
    
    std::cout.rdbuf(old);
    
    std::string output = buffer.str();
    CHECK(output.find("Usage: program [options]") != std::string::npos);
    CHECK(output.find("-h, --help") != std::string::npos);
    CHECK(output.find("-d, --path_to_database") != std::string::npos);
    CHECK(output.find("-l, --path_to_log") != std::string::npos);
    CHECK(output.find("-p, --port") != std::string::npos);
    CHECK(output.find("-a, --address") != std::string::npos);
}

// Тестирование исключений при некорректных аргументах
TEST(DecoderInvalidArguments) {
    Decoder decoder;
    const char* argv[] = {"program", "--invalid", "argument"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    CHECK_THROW(decoder.parse(argc, const_cast<char**>(argv)), ArgsDecodeException);
}

// Тестирование некорректного номера порта
TEST(DecoderInvalidPort) {
    Decoder decoder;
    const char* argv[] = {"program", "--port", "99999"};
    int argc = sizeof(argv) / sizeof(argv[0]);
    
    CHECK_THROW(decoder.parse(argc, const_cast<char**>(argv)), ArgsDecodeException);
}

// Тесты для функций getSalt и getHash

// Тест функции getSalt()
TEST(GetSalt) {
    std::string salt1 = getSalt();
    std::string salt2 = getSalt();

    // Проверка длины соли
    CHECK_EQUAL(16, salt1.size());
    CHECK_EQUAL(16, salt2.size());

    // Проверка, что сгенерированные соли не совпадают
    CHECK(salt1 != salt2);
}

// Тест функции getHash()
TEST(GetHash) {
    std::string data = "Hello, World!";
    std::string hash = getHash(data);

    // Проверка длины хэша SHA1
    CHECK_EQUAL(40, hash.size());

    // Проверка хэша для известных данных
    CHECK_EQUAL("0A0A9F2A6772942557AB5355D76AF442F8F65E01", hash);
}

// Тест на одинаковый хэш для одного и того же ввода
TEST(SameInputSameHash) {
    std::string data = "test";
    std::string hash1 = getHash(data);
    std::string hash2 = getHash(data);

    // Проверка, что хэши совпадают
    CHECK_EQUAL(hash1, hash2);
}

// Тесты для класса Network

// Тестирование конструктора и методов получения значений
TEST(NetworkConstructorAndGetters) {
    std::map<std::string, std::string> database = {{"user1", "password1"}};
    Network network("127.0.0.1", 33333, database, "./log.txt");

    CHECK_EQUAL("127.0.0.1", network.getAddress());
    CHECK_EQUAL(33333, network.getPort());
    CHECK(compareMaps(database, network.getDatabase()));
}

int main() {
    return UnitTest::RunAllTests();
}
