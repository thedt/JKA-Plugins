// g++ -g3 -ggdb -O0 -DDEBUG -I/usr/include/cryptopp Driver.cpp -o Driver.exe -lcryptopp -lpthread
// g++ -g -O2 -DNDEBUG -I/usr/include/cryptopp Driver.cpp -o Driver.exe -lcryptopp -lpthread

//Jass header
#include "Sha512.h"
#include "Jass/jassapi.h"

string Gen_salt()
{
	string encoded;
	AutoSeededRandomPool prng;
	SecByteBlock salt(AES::DEFAULT_KEYLENGTH);
	prng.GenerateBlock(salt, salt.size());
	StringSource(salt, salt.size(), true, new HexEncoder(new StringSink(encoded)));
	return encoded;
}

string Gen_Sha256(string source)
{
	CryptoPP::SHA256 hash;
	byte digest[CryptoPP::SHA256::DIGESTSIZE];
	hash.CalculateDigest(digest, (const byte*)source.c_str(), source.size());
	std::string output;
	CryptoPP::HexEncoder encoder;
	CryptoPP::StringSink test = CryptoPP::StringSink(output);
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();
	return output;
}

string Gen_Sha512(string source)
{
	CryptoPP::SHA512 hash;
	byte digest[CryptoPP::SHA512::DIGESTSIZE];
	hash.CalculateDigest(digest, (const byte*)source.c_str(), source.size());
	std::string output;
	CryptoPP::HexEncoder encoder;
	CryptoPP::StringSink test = CryptoPP::StringSink(output);
	encoder.Attach(new CryptoPP::StringSink(output));
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();
	return output;
}

string Gen_hash_loop(string source, string salt, int times)
{
	string sk = source;
	string cc;
	for (int i = 0; i < times; ++i)
	{
	Start:
		i++;
		if (i == times)
		{
			cc = Gen_Sha512(salt + sk);
			return cc;
		}
		else {
			cc = Gen_Sha512(sk);
		}
		sk = cc;
		goto Start;
	}
}

string Gen_Sha256_user(string source, int times)
{
	string sk = source;
	string cc;
	for (int i = 0; i < times; ++i)
	{
	Start:
		i++;
		if (i == times)
		{
			cc = Gen_Sha256(sk);
			return cc;
		}
		else {
			cc = Gen_Sha256(sk);
		}
		sk = cc;
		goto Start;
	}
}

string Get_delimiter(string text, char S)
{
	std::size_t index1, index2;
	std::string myString = text;
	std::string sub = "";
	index1 = myString.find(S);

	if (index1 != std::string::npos&& index1<myString.length() - 1)
		index2 = myString.find(S, index1 + 1);
	if (index2 != std::string::npos)
	{
		sub = myString.substr(index1+1, index2-1 - index1);
	}
	//sub.erase(std::remove(sub.begin(), sub.end(), S), sub.end());
	return sub;
}

string Last_delimter(string input, string delimiter)
{
	vector<string> output;
	size_t start = 0;
	size_t end = 0;

	while (start != string::npos && end != string::npos)
	{
		start = input.find_first_not_of(delimiter, end);

		if (start != string::npos)
		{
			end = input.find_first_of(delimiter, start);

			if (end == string::npos)
			{
				output.push_back(input.substr(start));
			}
		}
	}

	for (string t : output)
	{
		return t;
	}
}
