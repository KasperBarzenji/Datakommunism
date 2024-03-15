#include "node.h"

void distribute_packet(struct rtpkt pkt) {
	// gå igenom alla noder
	for (int i = 0; i < MAX_NODES; i++) {
		// om jag är noden skippa
		if (i == pkt.sourceid) continue;
		// noden inte är granne med mig skippa
		if (!is_neighbor(pkt.sourceid, i)) continue;
		// nod hittad skicka
		pkt.destid = i;
		tolayer2(pkt);
	}
}

void rtinit(struct distance_table *table, int node) {
	// initialisera ett packet
	struct rtpkt temp_pack;
	// sätt initial värden till 999 och källa till nod
	temp_pack.destid = INF;
	temp_pack.sourceid = node;
	// gå igenom alla noder
	for (int i = 0; i < MAX_NODES; i++) {
		// ändra mincost på noden
		temp_pack.mincost[i] = table->costs[node][i];
	}

	// skicka packet
	distribute_packet(temp_pack);

    if(TRACE >= 3)
        printdt(table, node);
}

void rtupdate(struct distance_table *table, int node, struct rtpkt *pkt) {
    int changed = 0;

	// bellman-ford
	for (int i = 0; i < MAX_NODES; i++) {
		if ((pkt->mincost[i] + table->costs[node][pkt->sourceid]) < table->costs[pkt->sourceid][i]) {
			table->costs[pkt->sourceid][i] = pkt->mincost[i] + table->costs[node][pkt->sourceid];
			changed = 1;
		}
	}

	// kollar om nod blivit ändrad
	if (changed) {
		struct rtpkt new_table;
		new_table.destid = INF;
		new_table.sourceid = node;
		for (int i = 0; i < MAX_NODES; i++) {
			int best_route = table->costs[node][i];
			for (int j = 0; j < MAX_NODES; j++) {
				if (table->costs[j][i] < best_route)
					best_route = table->costs[j][i];
			}
			new_table.mincost[i] = best_route;
		}
		distribute_packet(new_table);
	}
    if(TRACE >= 3)
        printdt(table, node);
}
