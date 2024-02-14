#ifndef CHECKSUM_H
#define CHECKSUM_H

int checksum(struct pkt *pack);
int verify_checksum(struct pkt *pack);

#endif