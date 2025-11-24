#pragma once

#include <exception>
#include <string>

using namespace std;

class Exception : public std::exception {
public:

    Exception(const string &name, const string &message, const string &func, const string &log_path, bool critical);
    
    const char *what() const noexcept override;

protected:

    void logException() const;

    string name;
    string func;
    mutable string message;
    string log_path;
    bool critical;
};

class IOException : public Exception {
public:

    IOException(const string &message, const string &func, const string &log_path, bool critical);
};

class ArgsDecodeException : public Exception {
public:

    ArgsDecodeException(const string &message, const string &func, const string &log_path, bool critical);
};

class DataDecodeException : public Exception {
public:

    DataDecodeException(const string &message, const string &func, const string &log_path, bool critical);
};


class AuthException : public Exception {
public:
    AuthException(const string &message, const string &func, const string &log_path, bool critical);
};

class NetworkException : public Exception {
public:

    NetworkException(const string &message, const string &func, const string &log_path, bool critical);
};

