#pragma once

#include <string>

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include <algorithm>

using namespace std;

string getSalt();
string getHash(const string &data);
