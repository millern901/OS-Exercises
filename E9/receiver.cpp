#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <streambuf>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>
#include <string>

using namespace std;

// reads the key from the file
char* readKey(string fileName)
{
	ifstream file(fileName.c_str());
	string contents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	char* key = (char*)malloc(strlen(contents.c_str()) * sizeof(char) + 1);
	strcpy(key, (char*)contents.c_str());
	file.close();
	return key;
}

// helper function to convert private key in string format to RSA format
RSA* convertPrivateKeyToRSA(FILE* fp, int length)
{
	RSA* p_key = NULL;
	RSA* temp = PEM_read_RSAPrivateKey(fp, &p_key, NULL, NULL);
	return p_key;
}

// helper function to convert public key in string format to RSA format
RSA* convertPublicKeyToRSA(FILE* fp, int length)
{
	RSA* publicKey = NULL;
	RSA* temp = PEM_read_RSA_PUBKEY(fp, &publicKey, NULL, NULL);
	return publicKey;
}

int main()
{
	// 1. Read the keys
	char* pubKeyStr = readKey("publicKey.txt");
	char* privKeyStr = readKey("privateKey.txt");

	//2. Display the keys
	printf("Public Key of Receiver:\n%s\n\n", pubKeyStr);
	printf("Private Key of Receiver:\n%s\n", privKeyStr);

	// 3. Open the named pipe in write only mode
	int pipe = open("pipeEx9", O_WRONLY);

	// 4. Write the public key to the pipe and close the pipe
	int pubKeyLength = strlen(pubKeyStr);
	int writeSize = write(pipe, pubKeyStr, pubKeyLength);
	close(pipe);

	// 5. Convert the public key in string format to RSA format
	FILE* pubKeyF = fopen("publicKey.txt", "r");
	RSA* pubKey = convertPublicKeyToRSA(pubKeyF, 0);
	fclose(pubKeyF);

	// 6. Open the pipe again in read only mode
	pipe = open("pipeEx9", O_RDONLY);

	// 7. Read the incoming message length and the incoming message
	int pubRsaSize = RSA_size(pubKey);
	char* en = (char*)malloc(pubRsaSize);
	int readSize = read(pipe, en, pubRsaSize);

	// 8. Close the pipe
	close(pipe);

	// 9. Display the received encrypted message
	printf("Encrypted Message:\n%s\n", en);

	// 10. Decrypt the message using RSA private decryption API
	FILE* privKeyF = fopen("privateKey.txt", "r");
	RSA* privKey = convertPrivateKeyToRSA(privKeyF, 0);
	fclose(privKeyF);

	int privRsaSize = RSA_size(privKey);
	char* de = (char*)malloc(privRsaSize);
	int decryptSize = RSA_private_decrypt(privRsaSize, (const unsigned char*)en, (unsigned char*)de, privKey, RSA_PKCS1_OAEP_PADDING);

	// 11. Display the decrypted message
	printf("Decrypted Message:\n%s\n", de);

	// 12. Free memory used
	free(pubKeyStr);
	free(privKeyStr);
	free(en);
	free(de);

	return 0;
}
