#include "rsa.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{
	size_t *primes;
	size_t nums[limit];
	size_t i, j;
	int p = 0;
	primes = (size_t *)malloc(limit * sizeof(size_t));
	// fill an array with natural numbers
	for (i = 0; i <= limit; i++)
	{
		if (i == 0 || i == 1)
			nums[i] = 0;
		else
			nums[i] = i;
	}
	// find non-prime numbers exept first two and set them to 0
	for (i = 2; i * i <= limit; i++)
	{
		for (j = i * i; j <= limit; j += i)
		{
			nums[j] = 0;
		}
	}
	// copy the prime(non-zero) numbers to the array to return
	for (i = 2; i < limit && p < limit; i++)
	{
		if (nums[i] != 0)
		{
			primes[p] = nums[i];
			p++;
		}
	}
	*primes_sz = p - 1;
	return primes;
}

/*
* If n is prime number returns 1
*/
int isprime(int n)
{
	int isp = 1;
	for (int i = 2; i < n / 2; i++)
	{
		if (n % i == 0)
		{
			isp = 0;
			break;
		}
	}
	return isp;
}
/*
* Random integer between range
*/
int random_int(int lower, int upper)
{
	const int rand_int = (rand() % (upper - lower + 1)) + lower;
	return rand_int;
}

/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int gcd(int a, int b)
{
	int gcd;
	for (int i = 1; (i <= a && i <= b); i++)
	{
		//if i is common divisorusage
		if ((a % i == 0) && (b % i == 0))
		{
			gcd = i; //update with last(max) divisor
		}
	}
	return gcd;
}

/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n)
{
	size_t e;

	do
	{
		e = (size_t)random_int(2, fi_n - 1);
	} while ((e > 1 && e < fi_n) && (gcd(e, fi_n) == 1) && isprime(e));

	return e;
}

/*
* Extended Euclidean Algorithm to compute Bezout's coefficients
* ax + by = gcd(a,b)
* ex + fi_ny = gcd(e,fi_n) = 1
* ex = -fi_ny + 1
* ex mod fi_n = 1
* so x = d
*/
int gcdX(int a, int b, int *x, int *y)
{
	if (a == 0)
	{
		*x = 0;
		*y = 1;
		return b;
	}
	int x1, y1; // To store results of recursive call
	int gcd = gcdX(b % a, a, &x1, &y1);
	*x = y1 - (b / a) * x1;
	*y = x1;
	return gcd;
}
/*
 * Calculates the modular inverse
 *
 * arg0: first number (e)
 * arg1: second number (f_n)
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)
{
	int x, y;
	int g = gcdX(a, b, &x, &y);
	size_t d = (size_t)x;
	return d;
}
/* 
* combines two variables and writes them as one to file 
*/
void writeToKeyFile(char *name, size_t v1, size_t v2)
{
	FILE *fptr = fopen((const char *)name, "w");
	if (fptr == NULL)
	{
		printf("Error opening file");
	}
	fprintf(fptr, "%ld %ld", v1, v2);
	fclose(fptr);
}
/* 
* writes data from the given buffer[length] to the given output file 
*/
void writeToFile(char *out_file, unsigned char *buffer)
{
	FILE *fptr = fopen((const char *)out_file, "wb");
	if (fptr == NULL)
	{
		printf("Error opening file");
	}
	fprintf(fptr, "%s", buffer);
	fclose(fptr);
}

int readFromFile(char *in_file, unsigned char **buffer)
{
	int fsize = 0;
	FILE *fptr = fopen((const char *)in_file, "r");
	if (fptr == NULL)
		printf("NULL data in file: %s", in_file);
	// access the EOF to declare the size of the file
	fseek(fptr, 0, SEEK_END);
	fsize = ftell(fptr);
	// +1 for \0
	*buffer = (unsigned char *)malloc(sizeof(int) * (fsize + 1));
	//return at the begining of the file
	fseek(fptr, 0, SEEK_SET);
	fread(*buffer, fsize, 1, fptr);
	fclose(fptr);
	return fsize;
}
size_t modular_exp(size_t base, size_t exponent, size_t modulus)
{
	size_t result = 1;
	if (modulus == 1)
		return 0;
	for (size_t i = 0; i < exponent; i++)
	{
		result = (result * base) % modulus;
	}
	return result;
}
/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void rsa_keygen(void)
{
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;

	int r1, r2;
	int limit = RSA_SIEVE_LIMIT;
	int primes_sz;
	size_t *primes;
	primes = sieve_of_eratosthenes(limit, &primes_sz);
	// generate two random positions of the primes table
	//srand(time(NULL));
	r1 = random_int(0, primes_sz);
	do
	{
		r2 = random_int(0, primes_sz);
	} while (r1 == r2);

	// assign the distinct random positions for the prime pair p and q
	p = primes[r1];
	q = primes[r2];
	n = p * q;
	fi_n = (p - 1) * (q - 1);
	e = 3;
	//e = choose_e(fi_n);
	d = mod_inverse(e, fi_n);
	//public key (nd)
	writeToKeyFile("public.key", n, d);
	//private key (ne)
	writeToKeyFile("private.key", n, e);

	free(primes);
}

/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_encrypt(char *input_file, char *output_file, char *key_file)
{
	unsigned char *plaintext = NULL;
	unsigned char *ciphertext;
	int plaintext_len = -1;
	int ciphertext_len = -1;
	size_t mod, exp;
	FILE *key_fp;
	key_fp = fopen(key_file, "rb");
	// extract n and d from public key file
	fscanf(key_fp, "%ld %ld", &mod, &exp);
	fclose(key_fp);
	// read plaintext from input file
	plaintext_len = readFromFile(input_file, &plaintext);
	ciphertext_len = sizeof(size_t) * (plaintext_len + 1);
	ciphertext = (unsigned char *)malloc(sizeof(int) * ciphertext_len);

	// compute ciphertext c using modular exponentiation
	for (size_t i = 0; i < ciphertext_len; i++)
	{
		ciphertext[i] = modular_exp(plaintext[i], exp, mod);
	}

	printf("plaintext to decrypt:\n");
	print_string(plaintext, plaintext_len);
	printf("ciphertext encrypted:\n");
	print_hex(ciphertext, ciphertext_len);

	writeToFile(output_file, ciphertext);

	free(plaintext);
	free(ciphertext);
}

/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void rsa_decrypt(char *input_file, char *output_file, char *key_file)
{
	unsigned char *plaintext;
	unsigned char *extraBplaintext;
	unsigned char *ciphertext = NULL;
	int plaintext_len = -1;
	int ciphertext_len = -1;
	size_t mod, exp;
	FILE *key_fp;
	key_fp = fopen(key_file, "rb");
	// extract n and e from private key file
	fscanf(key_fp, "%ld %ld", &mod, &exp);
	fclose(key_fp);
	// read ciphertext from input file
	ciphertext_len = readFromFile(input_file, &ciphertext);
	plaintext_len = ciphertext_len / sizeof(size_t);
	plaintext = (unsigned char *) malloc(sizeof(int) * plaintext_len);
	extraBplaintext = (unsigned char *)malloc(sizeof(int) * ciphertext_len);
	// recover m from c by using the private key exponent d using modular exponentiation
	for (size_t i = 0; i < ciphertext_len; i++)
	{
		extraBplaintext[i] = modular_exp(ciphertext[i], exp, mod);
	}
	for (size_t i = 0; i < ciphertext_len/8; i++)
	{
		plaintext[i] = extraBplaintext[i];
	}

	printf("ciphertext to decrypt:\n");
	print_hex(ciphertext, ciphertext_len);
	printf("plaintext decrypted:\n");
	print_string(plaintext, plaintext_len);

	writeToFile(output_file, plaintext);

	free(extraBplaintext);
	free(plaintext);
	free(ciphertext);
}
