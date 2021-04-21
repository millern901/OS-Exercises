#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <cstdlib>

using namespace std;

//helper function to convert public key in string format to RSA format
RSA* convertPublicKeyToRSA(FILE* fp, int length)
{
    RSA* r = NULL;
    RSA* x = PEM_read_RSA_PUBKEY(fp, &r, NULL, NULL);
    return r;
}

int main()
{
	// 1. Open the named pipe in read only mode
	int pipe = open("pipeEx9", O_RDONLY);

 	// 2. Read the public key of the receiver from the pipe
	char* pubKeyStr = (char*)malloc(512);
	int readSize = read(pipe, pubKeyStr, 512);

    	// 3. Close the pipe
	close(pipe);

    	// 4. Display the received public key
	printf("Public Key of Sender:\n%s\n\n", pubKeyStr);

	// 5. Input a message from the user using standard input
	char* de = (char*)malloc(214);
	printf("Enter the message:\n");
	cin.getline(de, 213);

	// 6. Convert the public key of the receiver from string in to RSA strucure format
	FILE* pubKeyF = fopen("publicKey.txt", "r");
	RSA* pubKey = convertPublicKeyToRSA(pubKeyF, 0);
	fclose(pubKeyF);

	// 7. Encrypt the message using the RSA public encryption API
	int pubRsaSize = RSA_size(pubKey);
	char* en = (char*)malloc(pubRsaSize);
	RSA_public_encrypt(214, (const unsigned char*)de, (unsigned char*)en, pubKey, RSA_PKCS1_OAEP_PADDING);

	// 8. Display the encrypted message
	printf("Encrypted Message:\n%s\n", en);

	// 9. Open the named pipe in write only mode
	pipe = open("pipeEx9", O_WRONLY);

    	// 10. Write the encrypted message to the pipe
	int writeSize = write(pipe, en, pubRsaSize);

	// 11. Close the pipe
     	close(pipe);

	// 12. Free memory used
	free(pubKeyStr);
	free(en);
	free(de);

	return 0;
}
