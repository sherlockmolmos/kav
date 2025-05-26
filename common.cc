#include "common.h"
#include "randombytes.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>

#include <errno.h>
#include <openssl/aes.h>
#include <openssl/sha.h>

#include <stdint.h>


#ifdef _WIN32

#include <Windows.h>

#else

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#endif

unsigned char *get_msg_data(DataMsg *msg){
    uintptr_t tempres;
    unsigned char *cres = (unsigned char*)(&tempres);

    for(int i=0; i < 8; i++) {
        cres[i] = msg->getData(i);
    }

    return (unsigned char*)tempres;
}

unsigned char *get_msg_data2(DataMsg *msg){
   uintptr_t tempres;
   unsigned char *cres = (unsigned char*)(&tempres);

   for(int i=0; i < 8; i++) {
       cres[i] = msg->getData2(i);
   }

   return (unsigned char*)tempres;
}

void set_msg_data(DataMsg *msg, unsigned char *data){
    uintptr_t temp = (uintptr_t)data;
    unsigned char *tempc = (unsigned char*)(&temp);

    for(int i=0; i < 8; i++) {
        msg->setData(i, tempc[i]);
    }
}

void set_msg_data2(DataMsg *msg, unsigned char *data){
    uintptr_t temp = (uintptr_t)data;
    unsigned char *tempc = (unsigned char*)(&temp);

    for(int i=0; i < 8; i++) {
        msg->setData2(i, tempc[i]);
    }
}

void aes_128_encrypt(const char* aes_key, char* aes_iv, char* data, size_t data_len, char* out) {
    AES_KEY encryptkey;

    AES_set_encrypt_key((const unsigned char*)aes_key, 128, &encryptkey);

    AES_cbc_encrypt((const unsigned char*)data, (unsigned char*)out, data_len, &encryptkey, (unsigned char*)aes_iv, AES_ENCRYPT);

}

void aes_128_decrypt(const char* aes_key, char* aes_iv, char* data, size_t data_len, char* out) { //data length must multi for 16
    AES_KEY decryptkey;

    AES_set_decrypt_key((const unsigned char*)aes_key, 128, &decryptkey);

    AES_cbc_encrypt((const unsigned char*)data, (unsigned char*)out, data_len, &decryptkey, (unsigned char*)aes_iv, AES_DECRYPT);
}

void read_nbytes_from_socket(int sockfd, char* buffer, size_t n) {
    size_t readsize, allreadsize = 0;

    while (allreadsize < n && (readsize = recv(sockfd, buffer + allreadsize, n - allreadsize, 0)) > 0) {
        allreadsize += readsize;
    }

    printf("out\n");
}

char* read_from_socket_with_bytespefix_then_decrept(int sockfd, char* key, char* iv, int* outlen) {
    size_t readsize, allreadsize = 0;
    int datalen = 264;

    char buffer[264] = { 0 }; // 256 + 8

    while (allreadsize < datalen && (readsize = recv(sockfd, buffer + allreadsize, datalen - allreadsize, 0)) > 0) {
        //printf("Received: %s\n", buffer);
        allreadsize += readsize;

        if (allreadsize >= 4) {
            memcpy(&datalen, buffer, 4);

            if (datalen == 0) {
                return 0;
            }

            datalen += 8;
        }
        //printf("datelen is %d\n", datalen);
    }

    memcpy(outlen, buffer + 4, 4);

    char* out = (char*)malloc(datalen - 8);

    aes_128_decrypt(key, iv, buffer + 8, datalen - 8, out);

    return out;
}

void random_len_data_encrypt_aes128(int* random_data_len, //true random data len
    int* random_data_len_to16, // 16-256 , 16 multi
    const char* aes_key,
    char* aes_iv,
    char** data_plain,
    char** data_encrypted) {

    unsigned char templen;
    randombytes(&templen, 1);

    *random_data_len = (int)templen;
    *random_data_len_to16 = *random_data_len + 16 - *random_data_len % 16;

    char* random_data_plain_to16 = (char*)malloc(*random_data_len_to16);
    char* random_data_encryped_lenprefix = (char*)malloc(8 + *random_data_len_to16);

    memset(random_data_plain_to16, 0, *random_data_len_to16);
    randombytes((uint8_t*)random_data_plain_to16, *random_data_len);

    memcpy(random_data_encryped_lenprefix, random_data_len_to16, 4);
    memcpy(random_data_encryped_lenprefix + 4, random_data_len, 4);

    aes_128_encrypt(aes_key, aes_iv, random_data_plain_to16, *random_data_len_to16, random_data_encryped_lenprefix + 8);

    *data_plain = random_data_plain_to16;
    *data_encrypted = random_data_encryped_lenprefix;
}


void sha256(const unsigned char* k, const unsigned char* k1, const unsigned char* k2, unsigned char* out)
{
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, k, 32);
    SHA256_Update(&sha256, k1, 32);
    SHA256_Update(&sha256, k2, 32);

    SHA256_Final(out, &sha256);
}
