/*
 * FILE: rdt_sender.cc
 * DESCRIPTION: Reliable data transfer sender.
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
#include "rdt_sender.h"
#include "rdt_protocol.h"

#include <queue>
#include <list>

#define TIME_OUT 0.28    // timeout 

int next_seq_num;        // seq number of next new packet
int ack_expected;        // oldest packet as yet unacknowledged

std::queue<packet*> buffer = std::queue<packet*>();  // packets waiting for window available
std::list<packet*> window = std::list<packet*>();  // packets that are sent but not acknowledged


/* sender initialization, called once at the very beginning */
void Sender_Init()
{
    fprintf(stdout, "At %.2fs: sender initializing ...\n", GetSimulationTime());
    next_seq_num = 0;
    ack_expected = 0;
}

/* sender finalization, called once at the very end.
   you may find that you don't need it, in which case you can leave it blank.
   in certain cases, you might want to take this opportunity to release some 
   memory you allocated in Sender_init(). */
void Sender_Final()
{
    fprintf(stdout, "At %.2fs: sender finalizing ...\n", GetSimulationTime());
}

void send()
{
    /* finish*/
    if(window.empty() && buffer.empty()) {
        if(Sender_isTimerSet()) Sender_StopTimer();
        return;
    }
    
    if(window.size() < WINDOW_SIZE) {
        for(int i = 0; i < (int)(WINDOW_SIZE - window.size()) && !buffer.empty(); i++){
            packet *pkt = buffer.front();
            buffer.pop();
            window.push_back(pkt);
            packet *pktCopy = new packet();
            memcpy(pktCopy->data, pkt->data, RDT_PKTSIZE);
            
            /* send it out through the lower layer */
            Sender_ToLowerLayer(pktCopy);
            Sender_StartTimer(TIME_OUT);
        }
    }
}

void CreatePacket(int payload_size, struct message *msg, int cursor)
{
    packet *pkt = new packet();
    memcpy(pkt->data + sizeof(short), &next_seq_num, sizeof(int));
    memcpy(pkt->data + sizeof(short) + sizeof(int), &payload_size, sizeof(char));
    memcpy(pkt->data + HEADER_SIZE, msg->data + cursor, payload_size);
    short checksum = CheckSum(pkt);
    memcpy(pkt->data, &checksum, sizeof(short));
    buffer.push(pkt);
    fprintf(stdout, "At %.2fs: sender create packet %d\n", GetSimulationTime(), next_seq_num);
    next_seq_num = Inc(next_seq_num);
}

/* event handler, called when a message is passed from the upper layer at the 
   sender */
void Sender_FromUpperLayer(struct message *msg)
{
    /* maximum payload size */
    int maxpayload_size = RDT_PKTSIZE - HEADER_SIZE;

    /* split the message if it is too big */
    /* the cursor always points to the first unsent byte in the message */
    int cursor = 0;

    while (msg->size-cursor > maxpayload_size) {
	/* fill in the packet */
    CreatePacket(maxpayload_size, msg, cursor);
	/* move the cursor */
	cursor += maxpayload_size;
    }

    /* send out the last packet */
    if (msg->size > cursor) {
	/* fill in the packet */
    CreatePacket(msg->size-cursor, msg, cursor);
    }

	/* send it out through the lower layer */
    // if(Sender_isTimerSet()) return;
	send();
}

/* event handler, called when a packet is passed from the lower layer at the 
   sender */
void Sender_FromLowerLayer(struct packet *pkt)
{
    int ack;
    memcpy(&ack, pkt->data + sizeof(short), sizeof(int));
    short expect_checksum = CheckSum(pkt);
    short receive_checksum;
    memcpy(&receive_checksum, pkt->data, sizeof(short));

    fprintf(stdout, "At %.2fs: sender receive ack %d, expected %d\n", GetSimulationTime(), ack, ack_expected);

    if(ack < 0 || ack >= SEQ_SIZE || expect_checksum != receive_checksum) return;

    int gap = ack - ack_expected;
    if((ack >= ack_expected) && (gap <= WINDOW_SIZE)) {
        // ack disorder is not an error
        if(expect_checksum == receive_checksum) {
            for(int i = 0; i <= gap && !window.empty(); i++){
                window.pop_front();    
            }
            ack_expected = Inc(ack);
        }
        send();
    }
}

/* event handler, called when the timer expires */
void Sender_Timeout()
{
    fprintf(stdout, "timeout!\n");
    if(!window.empty()) {
        for (packet *pkt : window) {
            packet *pktCopy = new packet();
            memcpy(pktCopy->data, pkt->data, RDT_PKTSIZE);

            /* send it out through the lower layer */
	        Sender_ToLowerLayer(pktCopy);
            Sender_StartTimer(TIME_OUT);
        }
    }
}
