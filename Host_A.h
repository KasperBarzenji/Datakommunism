#ifndef HOST_A_H
#define HOST_A_H

#include <string.h>
#include "Checksum.h"

void A_output(struct msg message);
void A_input(struct pkt packet);
void A_timerinterrupt();
void A_init();

#define A 0
#define B 1
// 1 second, 1000 ms
#define TIMEOUT 1000

#endif
