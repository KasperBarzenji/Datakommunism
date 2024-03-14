#ifndef NODE_H
#define NODE_H

#include "sim_engine.h"
#include <stdio.h>
#include <limits.h>
#define MAX_NODES 4
#define INF 999
extern int TRACE;

void rtinit(struct distance_table *table, int nodeno);
void rtupdate(struct distance_table *table, int nodeno, struct rtpkt *pkt);
void printdt(struct distance_table *table, int nodeno);

#endif
