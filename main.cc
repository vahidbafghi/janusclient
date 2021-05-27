/*
 * main.cc
 *
 *  Created on: May 27, 2021
 *      Author: vahid
 */
#include <iostream>
#include "openssl/rand.h"
#include "openssl/sha.h"
#include "openssl/"
#include <cstring>
#include <fstream>

#pragma pack(1)

using namespace std;
const unsigned int randLength = 468;
const unsigned int tokenLength = 32;

// TODO Create corresponding class
typedef struct{
	char token[tokenLength];
	time_t timestamp;
	int option;
	unsigned char rand[randLength];
} OptionMessage;


void sha256(unsigned char *buffer, int size, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, buffer, size);
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

bool writeToFile(char* filename, OptionMessage& om){
	ofstream finalvote(filename, ios::out | ios::binary);
	if(!finalvote) {
	  cout << "Cannot open file!" << endl;
	  return false;
	}
	finalvote.write((char *) &om, sizeof(OptionMessage));
	finalvote.close();
	if(!finalvote.good()) {
	  cout << "Error occurred at writing time!" << endl;
	  return false;
	}
	return true;
}

int main(int argc, char* argv[]){
	if (argc < 3){
		std::cout << "Usage: " << argv[0] << " [token] [option]" << std::endl;
		return -1;
	}
	// TODO sanity check the token
	OptionMessage om;
	om.timestamp = time(NULL);
	strncpy(om.token, argv[1], tokenLength);
	om.option = atoi(argv[2]);
	RAND_bytes(om.rand, randLength);

	writeToFile("finalvote", om);

	char hash[65];
	sha256((unsigned char*)&om, sizeof(OptionMessage), hash);
	std::cout << hash << std::endl;

	return 0;

}


