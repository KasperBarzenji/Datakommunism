#include "Sim_Engine.h"
#include "Checksum.h"
#include "Host_A.h"

// toggle = !toggle switches 0 to 1 and 1 to 0
int packID = 1;

/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
  /* TODO */
 if(verify_checksum(packet) != 1) {
		packet.seqnum = packID;
		packet.acknum = !packID;
		packet.checksum = checksum(packet);
		tolayer3(B, packet);
		return;
	} else if (packID == packet.seqnum) {
		tolayer5(B, packet.payload);
		packID = !packID;
	}
	tolayer3(B, packet);
}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
  /* TODO */
}
