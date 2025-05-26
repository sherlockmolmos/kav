#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "DataMsg_m.h"

#ifdef _WIN32


#define SERVER_PK_PATH "d:/server.pk"
#define SERVER_SK_PATH "d:/server.sk"

#define CLIENT_PK_PATH "d:/client.pk"
#define CLIENT_SK_PATH "d:/client.sk"

#else
typedef int SOCKET;

#define closesocket close

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <unistd.h>
#include <errno.h>

#define SERVER_PK_PATH "/home/veins/.key/server.pk"
#define SERVER_SK_PATH "/home/veins/.key/server.sk"

#define CLIENT_PK_PATH "/home/veins/.key/client.pk"
#define CLIENT_SK_PATH "/home/veins/.key/client.sk"
#endif

#define ERRORMSG strerror(errno)

#define EVN EV << "\n"
#define EVN1(s1) EV << (s1) << "\n"
#define EVN2(s1, s2) EV << (s1) << (s2) << "\n"
#define EVN3(s1, s2, s3) EV << (s1) << (s2) << (s3) << "\n"
#define EVN4(s1, s2, s3, s4) EV << (s1) << (s2) << (s3) << (s4) << "\n"
#define EVN5(s1, s2, s3, s4, s5) EV << (s1) << (s2) << (s3) << (s4) << (s5) << "\n"

//char *stringhex(unsigned char *data, size_t len);
#define EVHEX(data, len) {char *__str = stringhex((unsigned char *)(data), (len)); EVN1(__str); free(__str);}

#define VFIN(errmsg) throw cRuntimeError((errmsg))
#define VERR(mainmsg) EVN3(mainmsg, " ", ERRORMSG); VFIN((mainmsg))


unsigned char *get_msg_data(DataMsg *msg);

unsigned char *get_msg_data2(DataMsg *msg);

void set_msg_data(DataMsg *msg, unsigned char *data);

void set_msg_data2(DataMsg *msg, unsigned char *data);


void aes_128_encrypt(const char *aes_key, char *aes_iv, char *data, size_t data_len, char *out);


void aes_128_decrypt(const char *aes_key, char *aes_iv, char *data, size_t data_len, char *out);

void read_nbytes_from_socket(int sockfd, char *buffer, size_t n);

char *read_from_socket_with_bytespefix_then_decrept(int sockfd, char *key, char *iv, int *outlen);

void random_len_data_encrypt_aes128(int *random_data_len, //true random data len
                                    int *random_data_len_to16, // 16-256 , 16 multi
                                    const char *aes_key,
                                    char *aes_iv,
                                    char **data_plain,
                                    char **data_encrypted);

void sha256(const unsigned char *k, const unsigned char *k1, const unsigned char *k2, unsigned char *out);

#endif //COMMON_H
