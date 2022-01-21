// Collatz conjecture
#include "crypto.h"

inline int Crypto::collatz(int x) {
	int count = 0;
	while (x != 1 && x != 0) {
		if (x % 2 == 0) {
			x = x / 2;
			count = count + 1;
			continue;
		}
		else if (x % 2 == 1) {
			x = x * 3 + 1;
			count = count + 1;
			continue;
		}
	}
	return  count;
}

// Encrytion

string Crypto::enc(string plaintext) {
	unsigned int offset = 0;
	string ciphertext = "";

	for (unsigned char x : plaintext) {
		unsigned int asc = 0;
		unsigned int seed = 0;
		asc = (unsigned int)x; // asc values of plaintext characters
		seed = asc + offset; // seed for Collatz conjecture
		offset = collatz(seed);
		ciphertext.append(to_string(offset));
	}
	return ciphertext;
}

// Decrypting

vector<vector<char> > Crypto::dec(string ciphertext, int type_of_asc)
{
	vector<vector<char> > plaintext_charset;
	int i = 1, offset = 0;
	string tmp_1 = "", plaintext = "";
	for (char c : ciphertext)
	{
		string tmp_2(1, c);
		tmp_1.append(tmp_2);
		if (i % 2 == 1)
		{
			i++;
			continue;
		}
		else if (i % 2 == 0)
		{
			int j = 0;
			int fragment_of_cipher = stoi(tmp_1);
			vector<char> tmp_vector;
			for (j = offset; j < (type_of_asc + offset); j++)
			{
				if (collatz(j) == fragment_of_cipher)
				{
					tmp_vector.push_back((char)(j - offset));
				}
			}
			plaintext_charset.push_back(tmp_vector);
			offset = fragment_of_cipher;
			tmp_1 = "";
			tmp_2 = "";
			i = 1;
		}
	}
	
	return plaintext_charset;
}