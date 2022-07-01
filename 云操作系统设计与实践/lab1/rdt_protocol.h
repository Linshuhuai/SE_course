#include "rdt_struct.h"

#define SEQ_SIZE 2147483648  /* seq number has 4 byte(32 bits), 2^(31) = 128 */
#define HEADER_SIZE 7   // header size of a packet
#define WINDOW_SIZE 9  // sender's send window size

static short CheckSum(struct packet *pkt)
{
    unsigned long checksum = 0;
    int start = 2; // skip the checksum byte
    while(start < RDT_PKTSIZE) {
        checksum += *(short *)(&(pkt->data[start]));
        start += 2;
    }
    while(checksum >> 16) {  // 高16位非零
        checksum = (checksum >> 16) + (checksum & 0xFFFF);
    }
    return ~checksum;
}

/* return true if a <= b < c circularly; false otherwise */
static bool between(int a, int b, int c)
{
    if(((a <= b) && (b < c))||((c < a) && (a <= b))||((b < c)&&(c < a)))
        return true;
    else 
        return false;
}

static int Inc(int num) {
    return (num + 1) % SEQ_SIZE;
}