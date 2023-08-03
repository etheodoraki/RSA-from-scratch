# ACE414 Security of Systems and Services

### Assignment 3

### Implementation of RSA key-pair generation, encryption and decryption.

TASK A:
Key Derivation Function (KDF)
Generates an RSA public-private key pair and stores each one to the 
corresponding file. The public key is the combination of n-d variables, while 
the private key is the n-e combination. The values of n, d, e are assigned by 
the appropriate calculations based on the theory, while the random prime numbers 
p and q are selected by 2 random positions of a table with prime numbers from 0 
to 255, using the sieve of Eratosthenes algorithm.   

TASK B:
Data Encryption
Given input file, key file, output file.
First, the variables modulus(n for both private and public key) and exponent(d 
for public key and e for private key) extracts from the key file.
Then the input file is read as a plaintext and sent for decryption 
using a Modular Exponentiation function, that computes the ciphertext c 
providing the appropriate exponent and modulus. 
The result is written to the output file name given at the command for encryption.
The length of the ciphertext is plaintext_legth*8. 

TASK C:
Data Decryption
Given input file, key file, output file.
In this case, the key file must be the one not used at encryption. 
For example, if public.key file is used at encryption of the ciphertext we are 
about to decrypt, now we need to provide the private.key file for decryption at 
the command.
Like the previous task, the modulus and exponent extract from the key file 
in order to be used in the Modular Exponentiation function which is now used in 
reverse order of result, as it computes the plaintext (message) m this time.
Since the ciphertext created 8 bytes per 1byte of data in plaintext, the 
decrypted message must be 1byte per 8bytes of ciphertext.
The resulted decrypted message is written to the desired output file.
 

TASK D:
Using the tool

The files provided for encryption were succesfully encrypted and stored to the 
required file format.

