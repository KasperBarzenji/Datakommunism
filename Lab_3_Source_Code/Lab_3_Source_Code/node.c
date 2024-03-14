#include "node.h"

void distribute_packet(struct rtpkt pkt) {
	for (int i = 0; i < MAX_NODES; i++) {
		if (i == pkt.sourceid) continue;
        if ((pkt.sourceid % 2 == 1) && (i % 2 == 1)) continue;
		pkt.destid = i;
		tolayer2(pkt);
	}
}

void rtinit(struct distance_table *table, int node) {
    struct rtpkt initial_broadcast = { .destid = INF, .sourceid = node };
	for (int i = 0; i < MAX_NODES; i++) {
		initial_broadcast.mincost[i] = table->costs[node][i];
	}

	distribute_packet(initial_broadcast);

    if(TRACE >= 3)
        printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int changed = 0;

	for (int i = 0; i < MAX_NODES; i++) {
		if ((pkt->mincost[i] + table->costs[node][pkt->sourceid]) < table->costs[pkt->sourceid][i]) {
			table->costs[pkt->sourceid][i] = pkt->mincost[i] + table->costs[node][pkt->sourceid];
			changed = 1;
		}
	}

	if (changed) {
		struct rtpkt upt_table = { .destid = INF, .sourceid = node };
		for (int i = 0; i < MAX_NODES; i++) {
			int best_route = table->costs[node][i];
			for (int j = 0; j < MAX_NODES; j++) {
				if (table->costs[j][i] < best_route)
					best_route = table->costs[j][i];
			}
			upt_table.mincost[i] = best_route;
		}
		distribute_packet(upt_table);
	}
    if(TRACE >= 3)
        printdt(table, node);
}
