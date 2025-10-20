#include "hash.h"

string getSalt()
{
    const size_t SALT_SIZE = 8;
    CryptoPP::byte salt[SALT_SIZE];
    CryptoPP::AutoSeededRandomPool prng;
    prng.GenerateBlock(salt, SALT_SIZE);
    string salt_hex;
    CryptoPP::ArraySource(
        salt, sizeof(salt), true,
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(salt_hex), true));

    while (salt_hex.size() < 16)
        salt_hex = "0" + salt_hex;

    return salt_hex;
}

string getHash(const string &data)
{
    CryptoPP::SHA1 sha1;
    string hash;
    CryptoPP::StringSource(
        data, true,
        new CryptoPP::HashFilter(
            sha1, new CryptoPP::HexEncoder(
                      new CryptoPP::StringSink(hash), true)));

    return hash;
}
