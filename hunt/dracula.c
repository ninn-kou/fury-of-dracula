////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// dracula.c: your "Fury of Dracula" Dracula AI
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include "dracula.h"
#include "DraculaView.h"
#include "Game.h"
PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	Round r = playerNextRound(hv, hunter);
	PlaceId src = HvGetPlayerLocation(hv, hunter);
	PlaceId *pred = hunterBfs(hv, hunter, src, r);
	
	// One pass to get the path length
	int dist = 0;
	PlaceId curr = dest;
	while (curr != src) {
		dist++;
		curr = pred[curr];
	}
	
	PlaceId *path = malloc(dist * sizeof(PlaceId));
	// Another pass to copy the path in
	int i = dist - 1;
	curr = dest;
	while (curr != src) {
		path[i] = curr;
		curr = pred[curr];
		i--;
	}
	
	free(pred);
	*pathLength = dist;
	return path;
}

Player Playerjudger(int *array, int length, int q, int w, int e, int r) {
    bubble_sort(array, length);
    if (array[0] = q) {
        return PLAYER_LORD_GODALMING;
    } else if (array[0] == w) {
        return PLAYER_DR_SEWARD;
    } else if (array[0] == e) {
        return PLAYER_VAN_HELSING;
    } else if (array[0] == r) {
        return PLAYER_MINA_HARKER;
    }
}
void decideDraculaMove(DraculaView dv)
{
	// TODO: Replace this with something better!
	int count = -1;
	PlaceId *adjacent = DvWhereCanIGo(DraculaView dv, &count);
	PlaceId hunter0 = DvGetPlayerLocation(DraculaView dv, PLAYER_LORD_GODALMING);
	PlaceId hunter1 = DvGetPlayerLocation(DraculaView dv, PLAYER_DR_SEWARD);
	PlaceId hunter2 = DvGetPlayerLocation(DraculaView dv, PLAYER_VAN_HELSING);
	PlaceId hunter3 = DvGetPlayerLocation(DraculaView dv, PLAYER_MINA_HARKER);
	PlaceId curr = DvGetPlayerLocation(DraculaView dv, PLAYER_DRACULA);
	int *curr_hunter;
	PlaceId *path0;
	PlaceId *path1;
	PlaceId *path2;
	PlaceId *path3;	
	path0 = HvGetShortestPathTo(HunterView hv, PLAYER_LORD_GODALMING, curr, &curr_hunter[0]);	
	path1 = HvGetShortestPathTo(HunterView hv, PLAYER_DR_SEWARD, curr, &curr_hunter[1]);	
	path2 = HvGetShortestPathTo(HunterView hv, PLAYER_VAN_HELSING, curr, &curr_hunter[2]);	
	path3 = HvGetShortestPathTo(HunterView hv, PLAYER_MINA_HARKER, curr, &curr_hunter[3]);
	int *array1;
	int array1_count;
	int i = count - 1;
	int length = 0;	
	while (i >= 0) {
	    if (curr_hunter[0] < HvGetShortestPathTo(HunterView hv, Playerjudger(int *curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]), adjacent[i], &length)) {
	        array1[array1_count] = adjacent[i];
	        array1_count++;
	    }
	i--;
    }
    
}
