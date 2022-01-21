#pragma once
#include <string>
#include <vector>
#include <any>

using namespace std;

static class Crypto
{
public:
	static int collatz(int x);
	static string enc(string plaintext);
	static vector<vector<char> > dec(string ciphertext, int type_of_asc);
};