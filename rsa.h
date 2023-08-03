#ifndef _RSA_H
#define _RSA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>

# define RSA_SIEVE_LIMIT 255

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
sieve_of_eratosthenes(int, int *);

/*
* If n is prime number returns 1
*/
int isprime(int);

/*
* Random integer between range
*/
int
random_int(int , int );

/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int
gcd(int , int);


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t);

/*
* Extended Euclidean Algorithm to compute Bezout's coefficients
* ax + by = gcd(a,b)
*/
int
gcdX(int, int , int *, int *) ;

/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t, size_t);

void
writeToKeyFile( char *, size_t, size_t);
/* 
* writes data from the given buffer[length] to the given output file 
*/
void 
writeToFile(char *, unsigned char *);
/*reads data from file and stores them to buffer*/
int 
readFromFile(char *, unsigned char **);

/*Modular exponentiation*/
size_t modular_exp(size_t, size_t, size_t);


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void);


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *, char *, char *);


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *, char *, char *);

#endif /* _RSA_H */
