#include "Sim_Engine.h"
#include "Host_A.h"


// toggle = !toggle switches 0 to 1 and 1 to 0: timer = 0 false 1 true
int toggle = 0, timer = 0, count = 0;

struct pkt *tempPacket;

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message) {
  /* TODO */
  toggle = !toggle;
  struct pkt packet;
  packet.seqnum = toggle;
  packet.acknum = toggle;
  strcpy(packet.payload, message.data);
  packet.checksum = checksum(&packet);
  tempPacket = (struct pkt*)realloc(tempPacket, (count + 1) * sizeof(struct pkt));
  tempPacket[count] = packet;
  count++;
  // if timer is false
  if (!timer) {
    timer = 1;
    tolayer3(A, packet);
    starttimer(A, TIMEOUT);
  }
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet) {
  /* TODO */
  stoptimer(A);
  timer = 0;
  if (verify_checksum(&packet)) {

    for (int i = 0; i < count; i++){
      tempPacket[i] = tempPacket[i+1];
    }

    count--;
    if(count > 0){
      tempPacket = (struct pkt*)realloc(tempPacket, count * sizeof(struct pkt));

      timer = 1;
      tolayer3(A, tempPacket[0]);
      starttimer(A, TIMEOUT);
    }

  } else if(count > 0){
    timer = 1;
    tolayer3(A, tempPacket[0]);
    starttimer(A, TIMEOUT);
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt() {
  if(count > 0){
    timer = 1;
    tolayer3(A, tempPacket[0]);
    starttimer(A, TIMEOUT);
  }
}  

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init() {
  /* TODO */
  tempPacket = (struct pkt*)malloc(count * sizeof(struct pkt));
}
