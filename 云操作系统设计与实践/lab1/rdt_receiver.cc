/*
 * FILE: rdt_receiver.cc
 * DESCRIPTION: Reliable data transfer receiver.
 * NOTE: This implementation assumes there is no packet loss, corruption, or 
 *       reordering.  You will need to enhance it to deal with all these 
 *       situations.  In this implementation, the packet format is laid out as 
 *       the following:
 *       
 *       |<-  1 byte  ->|<-             the rest            ->|
 *       | payload size |<-             payload             ->|
 *
 *       The first byte of each packet indicates the size of the payload
 *       (excluding this single-byte header)
 * 
 * NEW_NOTE: New packet format
 *       |<-  2 byte  ->|<- 4 byte ->|<-  1 byte  ->|<- the rest ->| 
 *       |<- checksum ->| seq number | payload size |<- payload  ->|
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "rdt_struct.h"
#include "rdt_receiver.h"
#include "rdt_protocol.h"

int seq_expected;   // oldest packet as yet unacknowledged

/* receiver initialization, called once at the very beginning */
void Receiver_Init()
{
    seq_expected = 0;
    fprintf(stdout, "At %.2fs: receiver initializing ...\n", GetSimulationTime());
}

/* receiver finalization, called once at the very end.
   you may find that you don't need it, in which case you can leave it blank.
   in certain cases, you might want to use this opportunity to release some 
   memory you allocated in Receiver_init(). */
void Receiver_Final()
{
    fprintf(stdout, "At %.2fs: receiver finalizing ...\n", GetSimulationTime());
}

/* event handler, called when a packet is passed from the lower layer at the 
   receiver */
void Receiver_FromLowerLayer(struct packet *pkt)
{
    int receive_seq;
    memcpy(&receive_seq, pkt->data + sizeof(short), sizeof(int));
    short receive_checksum;
    memcpy(&receive_checksum, pkt->data, sizeof(short));

    short expect_checksum = CheckSum(pkt);

    int low = (seq_expected - 2*WINDOW_SIZE + SEQ_SIZE) % SEQ_SIZE;
    // int high = (seq_expected + 2*WINDOW_SIZE) % SEQ_SIZE;
    if(receive_seq < 0 || receive_seq >= SEQ_SIZE) return;

    if(receive_seq == seq_expected && receive_checksum == expect_checksum){
        fprintf(stdout, "At %.2fs: receiver right packet %d\n", GetSimulationTime(), receive_seq);
        /* 7-byte header indicating the size of the payload */
        int header_size = 7;

        /* construct a message and deliver to the upper layer */
        struct message *msg = (struct message*) malloc(sizeof(struct message));
        ASSERT(msg!=NULL);

        int payload_size = pkt->data[6];

        /* sanity check in case the packet is corrupted */
        if (payload_size < 0 || payload_size > RDT_PKTSIZE - header_size) return;

        msg->size = payload_size;
        msg->data = (char*) malloc(msg->size);
        ASSERT(msg->data!=NULL);
        memcpy(msg->data, pkt->data+header_size, msg->size);
        Receiver_ToUpperLayer(msg);
        Receiver_ToLowerLayer(pkt);

        seq_expected = Inc(seq_expected);

        /* don't forget to free the space */
        if (msg->data!=NULL) free(msg->data);
        if (msg!=NULL) free(msg);
    } else {
        if(between(low, receive_seq, seq_expected) && receive_checksum == expect_checksum) {
            Receiver_ToLowerLayer(pkt);
            fprintf(stdout, "At %.2fs: receiver resend ack %d, expected %d\n", GetSimulationTime(), receive_seq, seq_expected);
        } else {
            fprintf(stdout, "At %.2fs: receiver drop %d, expected %d\n", GetSimulationTime(), receive_seq, seq_expected);
        }

    }

}
