#include<iostream>
#include<fstream> // 读取文件 
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<any>
#include "crypto.h"
#include <chrono>


#define ORIGINAL_ASC 128
#define EXTEND_ASC 256

using namespace std::chrono;
using namespace std;

bool checker(string filename, string username);
void write_file(string url, string data);
string read_file(string url);
void split(const string& s, vector<string>& tokens, const string& delimiters);
bool user_authenticating(string table, string username);
vector<char> get_rand_serie();
vector<string> first_half(vector<char> serie);
vector<char> asc_256();
vector<string> second_10000(vector<char> asc_256_table);
vector<string> random_pass(string url);
vector<int> operator+(const vector<int>& v1, const vector<int>& v2);
void analysis_dec(vector<string> message);

// main function
int main()
{
	char choice = 0;
	string username = "";
	string password = "";
	string ciphertext = "";
	string data_line = "";
	vector<string> message;
	vector<vector<char> > result;

	do {
		cout << endl
			<< "---------------------------------------------------------\n"
			<< "Welcome to Collatz Password Manager System\n"
			<< " 1 - Create username/password.\n"
			<< " 2 - Check username and password.\n"
			<< " 3 - Generate password strength analysis file.\n"
			<< " 4 - Analys password strength analysis file.\n"
			<< " 5 - Sentence decryption.\n"
			<< " 6 - Exit.\n"
			<< "---------------------------------------------------------\n"
			<< " Enter your choice and press return: ";
		cin >> choice;


		switch (choice) {
		case '1':
			//code to create username
			cout << "Enter your username: ";
			cin >> username;
			data_line.append(username);
			data_line.append(" ");
			cout << "Enter your password: ";
			cin >> password;
			ciphertext = Crypto::enc(password);
			data_line.append(ciphertext);
			data_line.append("\n");
			write_file("password.txt", data_line);
			break;

		case '2':
			//code to check username and password
			cout << "Please enter your username and password.\n"
				<< "Username:";
			cin >> username;
			cout << "Checking username..." << endl;
			user_authenticating(read_file("password.txt"), username);
			break;

		case '3':

			cout << "Gnerating passwords..." << endl;
			message = random_pass("passwordtest.txt");
			cout << "Passwords created!" << endl;

			break;
		case '4':
			analysis_dec(message);
			break;
		case '5':
			//27322810313331033910211452912207344136146925461033281533271031012815108114101
			//27 32 28 103 13 33 103 39 102 114 52 9 12207344136146925461033281533271031012815108114101
			result = Crypto::dec("273228103133310339102114529122073441361469254610332815332710310128151081141018", ORIGINAL_ASC);
			
			for (auto& plaintext_set: result)
			{
				for (char& plaintext : plaintext_set)
				{
					cout << plaintext << " ";
				}
				cout << endl << "----------------------------------------------------" << endl;
			}

			// By semantic, for the fourth character ciphertext space should not be 2.
			
			cout << "---------------------------------------------------------\n";
			cout << "A friend to all is a friend to none." << endl;
			
			if (Crypto::enc("A friend to all is a friend to none") == "27322810313331033910211452912207344136146925461033281533271031012815108114101")
			{
				cout << "Successfull!" << endl;
			}
			
			break;
		case '6':
			cout << "End of Program.\n";
			break;
		default:
			cout << "Not a Valid Choice. \n"
				<< "Choose again.\n";
			break;
		}

	} while (choice != '6');

	return 0;
}

// Username checker

bool checker(string filename, string username) {
	ifstream infile(filename);
	infile.open(filename);
	int status = 0;
	string line = "";
	string data = "";
	string separator = " ";

	while (getline(infile, line)) {
		cout << "-----------------------" << endl;
		size_t found = line.find(separator); // find the position of separators
		if (found != std::string::npos) {
			std::string name = line.substr(0, found); // take the substrings before separator as usernames
			
			if (username == name) {
				cout << "Valid username!" << endl;
				status = 1;
				infile.close();
				return true;
			}

		}
		if (status == 0) {
			cout << "failure!" << endl;
			infile.close();
			return false;
		}
	}
}

// Open a file a writing into it

void write_file(string url, string data) {
	ofstream outfile;
	outfile.open(url, ios::app);
	outfile << data; // write in the data
	outfile.close();
}

// Open a file with reading mode

string read_file(string url) {
	string content = "", temp = "";
	ifstream infile;
	// file exception!
	try {
		infile.open(url);
		while (getline(infile, temp)) {
			content.append(temp);
			content.append("\n");
		}
	}
	catch (exception e) {
		cout << "error" << endl;
	} 
	
	
	return content;
}


// Split function
void split(const string& s, vector<string>& tokens, const string& delimiters = " ")
{
	string::size_type lastPos = s.find_first_not_of(delimiters, 0);// first position of string that is not delimeters
	string::size_type pos = s.find_first_of(delimiters, lastPos);// first position of string that is delimeters
	while (string::npos != pos || string::npos != lastPos)
	{
		tokens.push_back(s.substr(lastPos, pos - lastPos)); // append the substring to the token 
		lastPos = s.find_first_not_of(delimiters, pos); // update first position of string that is not delimeters
		pos = s.find_first_of(delimiters, lastPos); // update first position of string that is delimeters
	}
}

// Check username and password

bool user_authenticating(string table, string username)
{
	vector<string> lines, all_subs;
	string password = "";
	int count = 0;
	split(table, lines, "\n");// split the table by lines "\n"
	for (string s : lines)
	{
		vector<string> temp;
		split(s, temp, " "); // split the lines by space " "
		for (string s_1 : temp)
		{
			all_subs.push_back(s_1); // put the password to the next line of corresponding username 
		}
	}
	for (unsigned int i = 0; i < all_subs.size(); i++)
	{
		if (all_subs[i] == username)
		{
			cout << "Username valid!" << "\n";
			do {
				cout << "Now key in your password:" << "\n";
				cin >> password;
				count++;
				if (Crypto::enc(password) == all_subs[i + 1])
				{
					cout << "success!!" << endl;
					return true;
				}
				else
				{
					cout << "Oops! Wrong password, please try again!" << endl;
				}
			} while (count < 3); // retry 3 times
			if (count >= 3)
			{
				cout << "You have used up your chance to try the password." << endl;
				return false;
			}
		}
	}
	cout << "failure!" << endl;
	return false;
}


// Randomly select 10 lowercase

vector<char> get_rand_serie() {
	vector<char> table, serie;
	srand((unsigned)time(NULL));
	for (int i = 97; i < 123; i++) // for 'a'~'z'
	{
		table.push_back((char)i);// put 'a'~'z' into the table
	}
	for (int j = 0; j < 10; j++)
	{
		unsigned int pos = rand() % table.size(), size = table.size();// find a randome position in the table 
		char tmp = table[pos]; // allocate the randome character in the table 
		serie.push_back(tmp); // put the random character into serie
		// change the position of the random character with the last character in table
		table[pos] = table[size - 1];
		table[size - 1] = tmp;
		table.pop_back(); // delete the last character in table
	}
	//for (char c : serie) {
	//	cout << c;
	//}
	return serie;
}

// first 10000

vector<string> first_half(vector<char> serie)
{
	vector<string> result;
	int length = 1;

	for (int i = 1; i <= 10000; i++) 
	{
		string tmp = "";
		// increase the length when reach 101,201,301,401...
		if ((i % 100 == 1) && (i != 1))
		{
			length++;
		}
		// add random character to the string tmp
		for (int j = 0; j < length; j++)
		{
			tmp.append(string(1, serie[rand() % serie.size()]));
		}
		// Put the random string(password) to the result vector
		result.push_back(tmp); 
	}
	/*
	for (unsigned int k = 0; k < result.size(); k++)
	{
		cout << k << ": " << result[k] << endl;
	}
	*/
	
	return result;
}


// Ascii 256

vector<char> asc_256()
{
	vector<char> table;
	// put extended 256 ascii charactors to the table
	for (int i = 0; i < 257; i++) {
		// Any type
		any c = (char)i;
		table.push_back(any_cast<char>(c));
	}
	return table;
}

// the rest 10000

vector<string> second_10000(vector<char> asc_256_table)
{
	vector<string> result;
	int length = 1;
	for (int i = 1; i <= 10000; i++)
	{
		string tmp = "";
		// increase the length when reach 101,201,301,401...
		if ((i % 100 == 1) && i != 1)
		{
			length++;
		}
		// same method as in get_rand_serie(), to get rid of repeated
		for (int j = 0; j < length; j++)
		{
			vector<char> copy_of_table = asc_256_table;
			unsigned int pos = rand() % copy_of_table.size(), size = copy_of_table.size();
			char tmp_char = copy_of_table[pos];
			tmp.append(string(1, tmp_char));
			copy_of_table[pos] = copy_of_table[size - 1];
			copy_of_table[size - 1] = tmp_char;
			copy_of_table.pop_back();
		}
		result.push_back(tmp);	
	}
	/*
	for (unsigned int k = 0; k < 500; k++)
	{
		cout << k << ": " << result[k] << endl;
	}
	*/
	return result;
}


// Create 20000 random password

vector<string> random_pass(string url) 
{	
	vector<string> enc_first;
	vector<string> enc_second;
	vector<string> first = first_half(get_rand_serie());
	vector<string> second = second_10000(asc_256());
	vector<string> message = first;
	// Encrypting the 20000 passwords
	for (int i = 0; i < 10000; i++)
	{
		enc_first.push_back(Crypto::enc(first[i])); 
		enc_second.push_back(Crypto::enc(second[i]));
	}
	// Write in the 20000 encrypted passwords
	ofstream outfile;
	outfile.open(url);
	for (auto& s : enc_first)
	{
		outfile << s << endl;
	}
	for (auto& s : enc_second)
	{
		outfile << s << endl;
	}
	outfile.close();

	// Keep the 20000 unencrypted passwords for analysis
	for (string& s : second)
	{
		message.push_back(s);
	}
	return message; 
}



// Analyse the password decrypting
void analysis_dec(vector<string> message)
{
	vector<string> ciphertext;
	int asc = ORIGINAL_ASC, i = 0, accuracy = 0, status = 0;
	double total_time = 0.0;
	vector<double> average_time;
	vector<int> acc;
	split(read_file("passwordtest.txt"), ciphertext, "\n");
	for (string c : ciphertext)
	{
		clock_t start, finish;
		start = clock();
		vector<vector<char> > plaintext_charset = Crypto::dec(c, asc);
		unsigned int j = 0, k = 0;
		for (auto& p : plaintext_charset)
		{
			for (auto& plaintext_char : p)
			{
				if (plaintext_char == message[i][j])
				{
					status = 1;
					break;
				}
			}
			j++;
			if (status == 1)
			{
				k++;
				status = 0;
				continue;
			}
			else if (status == 0)
			{
				break;
			}
		}
		if (k == plaintext_charset.size())
		{
			accuracy++;
		}
		finish = clock();
		total_time += (float)(finish - start) / CLOCKS_PER_SEC;
		i++;
		if (i == 10000)
		{
			asc = EXTEND_ASC;
		}
		if (i % 100 == 0 && i != 0)
		{
			acc.push_back(accuracy);
			accuracy = 0;
			average_time.push_back(total_time / 100);
			total_time = 0;
		}
		
	}
	for (unsigned int n = 0; n < acc.size(); n++)
	{
		cout << "The accuracy of category " << n + 1 << " is " << acc[n] << "%, and the average time to crack this category is " << average_time[n] << " s." << endl;
	}
}




