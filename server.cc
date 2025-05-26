#include <string.h>
#include <omnetpp.h>
#include <stdio.h>
#include <error.h>

#include "api.h"

#include "printhex.h"
#include "common.h"
#include "randombytes.h"

#include "DataMsg_m.h"

using namespace omnetpp;

class Server : public cSimpleModule
{ private:
    unsigned char       *pk;
    unsigned char       sk[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       pk1[pqcrystals_kyber512_PUBLICKEYBYTES];
    unsigned char       sk1[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       pk2[pqcrystals_kyber512_PUBLICKEYBYTES];
    unsigned char       sk2[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       k[pqcrystals_kyber512_ref_BYTES];
    unsigned char       c[pqcrystals_kyber512_CIPHERTEXTBYTES];

    unsigned char       k1[pqcrystals_kyber512_ref_BYTES];
    unsigned char       c1[pqcrystals_kyber512_CIPHERTEXTBYTES];

    unsigned char       k2[pqcrystals_kyber512_ref_BYTES];
    unsigned char       *c2;

    unsigned char       aes_key[16], aes_iv[16];

    int                 ret_val;
    bool                is_handshake_finish = false;
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
  public:
    void printhex(unsigned char *hex, size_t len);
    void sendRandomData(DataMsg* datamsg);
    void receiveRandomData(DataMsg* datamsg);
    ~Server(){};
};

// The module class needs to be registered with OMNeT++
Define_Module(Server);

void Server::printhex(unsigned char *hex, size_t len)
{
    size_t index = 0;

    while (1)
    {
        if (len - index >= 32) {
            EVHEX(hex +  index, 32);
            index += 32;
        }
        else {
            EVHEX(hex +  index, len - index);
            break;
        }
    }
}

void Server::sendRandomData(DataMsg* datamsg) {
    int random_data_len; //true random data len
    int random_data_len_to16; // 16-256 , 16 multi

    char *random_data_plain;
    char *random_data_encryped_lenprefix;

    random_len_data_encrypt_aes128(&random_data_len, &random_data_len_to16, (const char*)aes_key, (char*)aes_iv,
                                     &random_data_plain, &random_data_encryped_lenprefix);

    EVN5("Sent Random data len(", random_data_len, " to ", random_data_len_to16, ")");
    printhex((unsigned char*)random_data_plain, random_data_len);

    char randomdatamsg[1024] = {0};
    char *randomdatastring = stringhex((unsigned char*)random_data_plain, random_data_len > 16 ? 16 : random_data_len);


    if (random_data_len>16) {
        sprintf(randomdatamsg, "\nRandom Message (%d bytes)\n %s%s\n", random_data_len, randomdatastring, "..........");
    }
    else
    {
        sprintf(randomdatamsg, "\nRandom Message (%d bytes)\n %s\n", random_data_len, randomdatastring);
    }

    free(randomdatastring);

    datamsg->setName(randomdatamsg);
    set_msg_data(datamsg, (unsigned char*)random_data_encryped_lenprefix);

    datamsg->setTruelen(random_data_len);
    datamsg->setDatalen(random_data_len_to16);

    send(datamsg, "out");

    free(random_data_plain);
}

void Server::receiveRandomData(DataMsg* datamsg) {
    char *out = new char[datamsg->getDatalen()];

    aes_128_decrypt((const char*)aes_key, (char*)aes_iv, ((char*)get_msg_data(datamsg)) + 8, datamsg->getDatalen(), out);

    EVN5("Received random data len(", datamsg->getTruelen(), " to ", datamsg->getDatalen(), ")");

    printhex((unsigned char*)out, datamsg->getTruelen());

    delete[] out;
    free((unsigned char*)(get_msg_data(datamsg)));
}

void Server::initialize()
{
    FILE *fp = fopen(SERVER_SK_PATH, "rb");
    fread(sk2, pqcrystals_kyber512_ref_SECRETKEYBYTES, 1, fp);
    fclose(fp);

    EVN3("Read server secret key from ", SERVER_SK_PATH," as sk2 successfully!");
    printhex(sk2, pqcrystals_kyber512_ref_SECRETKEYBYTES);

    fp = fopen(CLIENT_PK_PATH, "rb");
    fread(pk1, pqcrystals_kyber512_ref_PUBLICKEYBYTES, 1, fp);
    fclose(fp);

    EVN3("Read client public key from ", CLIENT_PK_PATH," as pk1 successfully!");
    printhex(pk1,pqcrystals_kyber512_ref_PUBLICKEYBYTES);
}

void Server::handleMessage(cMessage *msg)
{
    DataMsg* datamsg = check_and_cast<DataMsg*>(msg);

    if (!is_handshake_finish){
        pk = (unsigned char*)get_msg_data(datamsg);
        c2 = (unsigned char*)get_msg_data2(datamsg);

        EVN1("Received pk, c2 from client");

        if ( (ret_val = pqcrystals_kyber512_ref_dec(k2, c2, sk2)) != 0) {
            EVN2("Error key c2: ret value is ", ret_val);
            VFIN("Error key c2");
        }

        EVN1("Decaps k2 from sk2 c2:");
        EVN1("k2 is:");
        EVHEX(k2, 32);
        EVN1("\n");

        if ( (ret_val = pqcrystals_kyber512_ref_enc(c, k, pk)) != 0) {
            EVN2("crypto_kem_enc failed ", ret_val);
            VFIN("crypto_kem_enc failed");
        }

        EVN1("Encaps pk ---> c k successully!");
        EVN1("K is:");
        EVHEX(k, pqcrystals_kyber512_ref_BYTES);
        EVN1("\n");

        if ( (ret_val = pqcrystals_kyber512_ref_enc(c1, k1, pk1)) != 0) {
            EVN2("crypto_kem_enc failed ", ret_val);
            VFIN("crypto_kem_enc failed");
        }

        EVN1("Encaps pk1 ---> c1 k1 successully!");
        EVN1("K1 is:");
        EVHEX(k1, pqcrystals_kyber512_ref_BYTES);
        EVN1("\n");

        datamsg->setName("c c1");
        set_msg_data(datamsg, c);
        set_msg_data2(datamsg, c1);

        EVN1("Send c c1 to client successfully!");

        unsigned char aeskeyiv[32];

        sha256(k, k1, k2, aeskeyiv);

        memcpy(aes_key, aeskeyiv, 16);
        memcpy(aes_iv, aeskeyiv + 16, 16);

        EVN1("Hash k k1 k2 by sha3-256, get aes key and iv:");
        EVN1("AES KEY:");
        EVHEX(aes_key, 16);
        EVN;
        EVN1("AES IV:");
        EVHEX(aes_iv, 16);
        EVN1("\n");

        send(datamsg, "out");

        is_handshake_finish = true;
    } else {
        receiveRandomData(datamsg);
        sendRandomData(datamsg);
    }
}

