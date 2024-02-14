#include "Sim_Engine.h"

// sums up acknum, seqnum and payload store in sum
int checksum(struct pkt *pack) {
    // sum variable starts at 0
    int sum = 0;

    // loop through payload until end of char
    for (int i = 0; pack->payload[i] != '\0'; i++) {
        sum += pack->payload[i];
    }

    sum += pack->acknum;
    sum += pack->seqnum;

    return sum;
}

// verify checksum in packet, returns 1 true and -1 false
int verify_checksum(struct pkt *pack) {
    // ternerary operator
    return checksum(pack) == pack->checksum ? 1 : -1;
}