#include "exceptions.h"
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

Exception::Exception(
    const string &name, const string &message,
    const string &func, const string &log_path,
    bool critical)
    : name(name), func(func), message(message), log_path(log_path), critical(critical)
{
    logException();
}

const char *Exception::what() const noexcept
{
    string critical_str = critical ? "Critical: Yes" : "Critical: No";
    message = "[" + critical_str + "] " + name + " in " + func + "\nMessage: " + message + ".\n";
    return message.c_str();
}

void Exception::logException() const
{
    ofstream log_file(log_path, ios_base::app);
    if (!log_file.is_open())
    {
        cerr << "Failed to open log file for writing.\n";
        return;
    }

    time_t now = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
    string critical_str = critical ? "Critical" : "NoCritical";
    log_file << "[" << critical_str << "]\t[" << buffer << "]\t"
             << name << " in " << func << ": " << message << ".\n";

    log_file.close();
}

IOException::IOException(
    const string &message,
    const string &func,
    const string &log_path,
    bool critical)
    : Exception("IOException", message, func, log_path, critical) {}

ArgsDecodeException::ArgsDecodeException(
    const string &message,
    const string &func,
    const string &log_path,
    bool critical)
    : Exception("ArgsDecodeException", message, func, log_path, critical) {}

DataDecodeException::DataDecodeException(
    const string &message,
    const string &func,
    const string &log_path,
    bool critical)
    : Exception("DataDecodeException", message, func, log_path, critical) {}

AuthException::AuthException(
    const string &message,
    const string &func,
    const string &log_path,
    bool critical)
    : Exception("AuthException", message, func, log_path, critical) {}

NetworkException::NetworkException(
    const string &message,
    const string &func,
    const string &log_path,
    bool critical)
    : Exception("NetworkException", message, func, log_path, critical) {}
