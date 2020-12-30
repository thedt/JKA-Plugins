/*
DT-Plugin - AES_GCM Functions
*/

using namespace std;
#include "Crypto/osrng.h"
using CryptoPP::AutoSeededRandomPool;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "Crypto/cryptlib.h"


#include "Crypto/hex.h"
using CryptoPP::HexEncoder;

#include "Crypto/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "Crypto/aes.h"
using CryptoPP::AES;


#include "Crypto/secblock.h"
using CryptoPP::SecByteBlock;

extern string Gen_salt();
extern string Gen_Sha256(string source);
extern string Gen_Sha512(string source);
extern string Gen_hash_loop(string source, string salt, int times);
extern string Get_delimiter(string text, char S);
extern string Last_delimter(string input, string delimiter);
extern string Gen_Sha256_user(string source, int times);