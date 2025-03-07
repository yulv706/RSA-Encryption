#include <iostream>
#include <string>
#include <vector>
#include <ctime>

// a little help from stackoverflow to check if the nth number is prime
bool isPrime(int n) {
	if (n % 2 == 0) return n == 2;
	if (n % 3 == 0) return n == 3;
	int step = 4, m = (int)pow(n, 0.5) + 1;
	for (int i = 5; i < m; step = 6 - step, i += step) {
		if (n % i == 0) {
			return false;
		}
	}
	return true;
}

// C function for extended Euclidean Algorithm taken from geeksforgeeks.com
int gcdExtended(int a, int b, int *x, int *y)
{
	// Base Case 
	if (a == 0)
	{
		*x = 0, *y = 1;
		return b;
	}

	int x1, y1; // To store results of recursive call 
	int gcd = gcdExtended(b%a, a, &x1, &y1);

	// Update x and y using results of recursive 
	// call 
	*x = y1 - (b / a) * x1;
	*y = x1;

	return gcd;
}

// modular multiplicative inverse taken from geeksforgeeks.com
int modInverse(int a, int m)
{
	int x, y;
	int g = gcdExtended(a, m, &x, &y);
	if (g != 1)
		return -1;
	else
	{
		// m is added to handle negative x 
		int res = (x%m + m) % m;
		return res;
	}
}

// a little help from stakoverflow to get the nth prime number
int nthPrime(int n)
{
	int candidate, count;
	for (candidate = 2, count = 0; count < n; ++candidate) {
		if (isPrime(candidate)) {
			++count;
		}
	}
	return candidate - 1;
}

// modular exponentiation a^b % c
unsigned long long emod(unsigned long long a, unsigned long long b, unsigned long long c)
{
	if (b == 0)
	{
		return 1;
	}
	else if (b % 2 == 0)
	{
		unsigned long long d = emod(a, b / 2, c);
		return (d * d) % c;
	}
	else {
		return ((a % c) * emod(a, b - 1, c)) % c;
	}
}

// encrypt function which takes the msg blocks, n and e to encrypt the message or d and e to decrypt the message
std::vector<unsigned long long> encrypt_decrypt(std::vector<unsigned long long> msgV, unsigned long long n_or_d, unsigned long long e)
{
	std::vector<unsigned long long> c;

	for (std::vector<unsigned long long>::iterator it = msgV.begin(); it != msgV.end(); ++it)
	{
		unsigned long long v = emod(*it, e, n_or_d);
		c.push_back(v);
	}

	return c;
}

// a function to iterate over vectors and display their data
template<class T>
void iterateAndDisplay(std::vector<T> aVector, int incrementValue)
{
	for (std::vector<T>::iterator it = aVector.begin(); it != aVector.end(); it = it + incrementValue)
	{
		std::cout << "'" << *it << "'" << " ";
	}
}

int main()
{
	srand(time(NULL));
	std::string message;                               // message in string
	std::vector<int> msgVectorN;                       // message to be converted into separate characters and stores number values in a vector
	std::vector<char> msgVectorC;                      // message to be converted into separate characters and stores in a vector
	std::vector<unsigned long long> msgBlocks;         // stores message blocks
	std::vector<int> recoveredMsg;                     // stores recovered message numbers
	
	std::cout << "Enter a message: ";
	std::getline(std::cin, message);
	
	for (int i = 0; i < message.length(); i++)         // converting the message into numbers and storing in a vector
	{
		msgVectorC.push_back((message[i]));
		if (message[i] != ' ') {
			msgVectorN.push_back(int(message[i] - 65));
		}
		else {
			msgVectorN.push_back(26);
		}
	}

	std::cout << "Message:              ";
	iterateAndDisplay(msgVectorC, 1);                                                                   // displaying message char by char
	std::cout << std::endl;

	std::cout << "Number values:         ";                                                              // displaying number values
	iterateAndDisplay(msgVectorN, 1);
	std::cout << std::endl;

	if (msgVectorN.size() % 2 == 0) {
		for (std::vector<int>::iterator it = msgVectorN.begin(); it != msgVectorN.end(); it = it + 2)  // converting numbers into message blocks
		{
			msgBlocks.push_back(*it * 100 + (*(it + 1)));
		}
	}
	else {
		std::vector<int>::iterator it = msgVectorN.begin();
		for (it; it != msgVectorN.end() - 1; it = it + 2)
		{
			msgBlocks.push_back(*it * 100 + (*(it + 1)));
		}
		msgBlocks.push_back(*it * 100);
	}

	std::cout << "Message Blocks:        ";                                                            // displaying message blocks
	iterateAndDisplay(msgBlocks, 1);
	std::cout << std::endl;

	unsigned long nthRandPrime1 = 1000 + (std::rand() % (2000 - 1000 + 1));                            // a random number between 1000 and 2000
	unsigned long nthRandPrime2 = 1000 + (std::rand() % (2000 - 1000 + 1));                            // a random number between 1000 and 2000

	unsigned long p = nthPrime(nthRandPrime1);                                                         // using the random number to get the nth prime
	unsigned long q = nthPrime(nthRandPrime2);                                                         // using the random number to get the nth prime

	std::cout << "p (a random prime):    " << p << std::endl;                                          // printing the nth prime p
	std::cout << "q (a random prime):    " << q << std::endl;                                          // printing the nth prime q

	unsigned long long n = p * q;                                                                      // calculating n = p * q

	unsigned long long totient = (p - 1)*(q - 1);                                                      // calculating phi = (p - 1) * (q - 1)

	unsigned long long e_coPrime = nthPrime(std::rand() % totient);                                    // calculating e which is a prime between 1 and phi

	unsigned long long d = modInverse(e_coPrime, totient);                                             // calculating modular multiplicative inverse using e and phi

	std::cout << "Public Key  (e , n):   (" << e_coPrime << " , " << n << " )" << std::endl;           // printing public key generated
	std::cout << "Private Key (d , n):   (" << d << " , " << n << " ) " << std::endl;                  // printing private key generated

	std::vector<unsigned long long> encryptedMsg = encrypt_decrypt(msgBlocks, n, e_coPrime);           // encrypting the message using message blocks, n and e

	std::cout << "Encrypted Message:     ";                                                            // displaying the encrypted message
	iterateAndDisplay(encryptedMsg, 1);
	std::cout << std::endl;

	std::vector<unsigned long long> decryptedMsg = encrypt_decrypt(encryptedMsg, n, d);               // decrypting the message using the encrypted numbers, n and d

	std::cout << "Decrypted Message:     ";                                                           // displaying the decrypted message
	iterateAndDisplay(decryptedMsg, 1);
	std::cout << std::endl;

	if (msgVectorN.size() % 2 == 0) {                                                                // breaking message blocks into smaller number values to be converted into characters
		for (std::vector<unsigned long long>::iterator it = decryptedMsg.begin(); it != decryptedMsg.end(); it = it + 1)
		{
			if (*it > 99)
			{
				recoveredMsg.push_back(*it / 100);
				recoveredMsg.push_back(*it % 100);
			}

			else if (*it < 99 && *it % 100 != 0)
			{
				recoveredMsg.push_back(0);
				recoveredMsg.push_back(*it % 100);
			}

			else {
				recoveredMsg.push_back(*it);
			}
		}
	}

	else {
		std::vector<unsigned long long>::iterator it = decryptedMsg.begin();
		for (it; it != decryptedMsg.end() - 1; it = it + 1)
		{
			if (*it > 99)
			{
				recoveredMsg.push_back(*it / 100);
				recoveredMsg.push_back(*it % 100);
			}

			else if (*it < 99 && *it % 100 != 0)
			{
				recoveredMsg.push_back(0);
				recoveredMsg.push_back(*it % 100);
			}

			else {
				recoveredMsg.push_back(*it);
			}
		}

		recoveredMsg.push_back(*it / 100);
	}

	std::cout << "Number values:         ";                                                            // displaying the number values
	iterateAndDisplay(recoveredMsg, 1);
	std::cout << std::endl;

	std::cout << "Message:               ";                                                            // displaying the message decrypted
	for (std::vector<int>::iterator it = recoveredMsg.begin(); it != recoveredMsg.end(); ++it)
	{
		if (char(*it + 65) != '[') {
			std::cout << "'" << char(*it + 65) << "'" << " ";
		}
		else {
			std::cout << "'" << " " << "'" << " ";
		}
	}
	std::cout << std::endl;

	return 0;
}
