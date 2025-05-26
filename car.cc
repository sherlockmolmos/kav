#include <string.h>
#include <omnetpp.h>

#include <time.h>
#include <math.h>
#include <unistd.h>

#include "api.h"

#include "randombytes.h"
#include "printhex.h"
#include "common.h"

#include "DataMsg_m.h"

using namespace omnetpp;

class Car : public cSimpleModule
{
  private:
    unsigned char       pk[pqcrystals_kyber512_PUBLICKEYBYTES];
    unsigned char       sk[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       pk1[pqcrystals_kyber512_PUBLICKEYBYTES];
    unsigned char       sk1[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       pk2[pqcrystals_kyber512_PUBLICKEYBYTES];
    unsigned char       sk2[pqcrystals_kyber512_SECRETKEYBYTES];

    unsigned char       k[pqcrystals_kyber512_ref_BYTES];
    unsigned char       *c;

    unsigned char       k1[pqcrystals_kyber512_ref_BYTES];
    unsigned char       *c1;

    unsigned char       k2[pqcrystals_kyber512_ref_BYTES];
    unsigned char       c2[pqcrystals_kyber512_CIPHERTEXTBYTES];

    unsigned char       aes_key[16], aes_iv_encrypt[16], aes_iv_decrypt[16];

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
};

// The module class needs to be registered with OMNeT++
Define_Module(Car);

void Car::printhex(unsigned char *hex, size_t len)
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

void Car::initialize()
{
    FILE *fp = fopen(SERVER_PK_PATH, "rb");
    fread(pk2, pqcrystals_kyber512_PUBLICKEYBYTES, 1, fp);
    fclose(fp);

    EVN3("Read server public key from ", SERVER_PK_PATH, " as pk2 successfully!");
    printhex(pk2, pqcrystals_kyber512_PUBLICKEYBYTES);

    fp = fopen(CLIENT_SK_PATH, "rb");
    fread(sk1, pqcrystals_kyber512_SECRETKEYBYTES, 1, fp);
    fclose(fp);

    EVN3("Read client secret key from ", SERVER_PK_PATH," as sk1 successfully!");
    printhex(sk1, pqcrystals_kyber512_SECRETKEYBYTES);

    if ( (ret_val = pqcrystals_kyber512_ref_keypair(pk, sk)) != 0) {
        EVN2("Generate pk,sk failed! returned ", ret_val);
        VFIN("Generate pk,sk failed");
    }

    EVN1("Generate pk,sk key pair from Kyber KeyGen successfully!\n");

    if ( (ret_val = pqcrystals_kyber512_ref_enc(c2, k2, pk2)) != 0) {
        printf("crypto_kem_enc failed <%d>\n", ret_val);
        VFIN("crypto_kem_enc failed");
    }

    EVN1("Encaps pk2 ---> c2 k2 successully!");
    EVN1("K2 is:");
    EVHEX(k2, pqcrystals_kyber512_ref_BYTES);
    EVN1("\n");

    char msgname[256] = {0};
    char *k2str = stringhex(k2, pqcrystals_kyber512_ref_BYTES);

    sprintf(msgname, "k2:\n%s\n", k2str);
    free(k2str);

    DataMsg* datamsg = new DataMsg(msgname);
    set_msg_data(datamsg, (unsigned char*)pk);
    set_msg_data2(datamsg, (unsigned char*)c2);

    send(datamsg, "out");

    EVN1("Send pk c2 to server successfully!\n");
}

void Car::sendRandomData(DataMsg* datamsg) {
    int random_data_len; //true random data len
    int random_data_len_to16; // 16-256 , 16 multi

    char *random_data_plain;
    char *random_data_encryped_lenprefix;

    random_len_data_encrypt_aes128(&random_data_len, &random_data_len_to16, (const char*)aes_key, (char*)aes_iv_encrypt,
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

void Car::receiveRandomData(DataMsg* datamsg) {
    char *out = new char[datamsg->getDatalen()];

    aes_128_decrypt((const char*)aes_key, (char*)aes_iv_decrypt, ((char*)get_msg_data(datamsg)) + 8, datamsg->getDatalen(), out);

    EVN5("Received random data len(", datamsg->getTruelen(), " to ", datamsg->getDatalen(), ")");

    printhex((unsigned char*)out, datamsg->getTruelen());

    delete[] out;
    free((unsigned char*)(get_msg_data(datamsg)));
}

void Car::handleMessage(cMessage *msg)
{
    DataMsg* datamsg = check_and_cast<DataMsg*>(msg);

    if (!is_handshake_finish) {
        c = get_msg_data(datamsg);
        c1 = get_msg_data2(datamsg);

        EVN1("Received c, c1 from server successfully!");

        if ( (ret_val = pqcrystals_kyber512_ref_dec(k, c, sk)) != 0) {
            EVN2("Error key c: ret value is ", ret_val);
            VFIN("Error key c");
        }

        EVN1("Decaps k from sk c:");
        EVN1("k is:");
        EVHEX(k, 32);
        EVN1("\n");

        if ( (ret_val = pqcrystals_kyber512_ref_dec(k1, c1, sk1)) != 0) {
            EVN2("Error key c1: ret value is ", ret_val);
            VFIN("Error key c1");
        }

        EVN1("Decaps k1 from sk1 c1:");
        EVN1("k1 is:");
        EVHEX(k1, 32);
        EVN1("\n");

        unsigned char aeskeyiv[32];

        sha256(k, k1, k2, aeskeyiv);

        memcpy(aes_key, aeskeyiv, 16);
        memcpy(aes_iv_encrypt, aeskeyiv + 16, 16);
        memcpy(aes_iv_decrypt, aeskeyiv + 16, 16);

        EVN1("Hash k k1 k2 by sha3-256, get aes key and iv:");
        EVN1("AES KEY:");
        EVHEX(aes_key, 16);
        EVN;
        EVN1("AES IV:");
        EVHEX(aeskeyiv + 16, 16);
        EVN1("\n");

        is_handshake_finish = true;

        sendRandomData(datamsg);
    }else{
        receiveRandomData(datamsg);
        sendRandomData(datamsg);
    }
}

