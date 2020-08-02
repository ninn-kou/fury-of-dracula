////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// HunterView.c: the HunterView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "HunterView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include "Queue.h"
#include "utils.h"

struct hunterView {
	GameView gv;
	Map map;
};

static PlaceId *hunterBfs(HunterView hv, Player hunter, PlaceId src,
                          Round r);
static Round playerNextRound(HunterView hv, Player player);

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView hv = malloc(sizeof(*hv));
	if (hv == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}
	
	hv->gv = GvNew(pastPlays, messages);
	hv->map = MapNew();
	return hv;
}

void HvFree(HunterView hv)
{
	GvFree(hv->gv);
	MapFree(hv->map);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv)
{
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player)
{
	return GvGetHealth(hv->gv, player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	return GvGetPlayerLocation(hv->gv, player);
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	int numLocs = 0;
	bool canFree = false;
	PlaceId *locs = GvGetLocationHistory(hv->gv, PLAYER_DRACULA,
	                                     &numLocs, &canFree);
	PlaceId location = NOWHERE;
	for (Round i = numLocs - 1; i >= 0; i--) {
		if (placeIsReal(locs[i])) {
			location = locs[i];
			*round = i;
			break;
		}
	}
	
	if (canFree) free(locs);
	return location;
}

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

/**
 * Performs a BFS for the given hunter starting at `src`, assuming the
 * round is `r`. Returns a predecessor array.
 */
PlaceId *hunterBfs(HunterView hv, Player hunter, PlaceId src, Round r) {
	PlaceId *pred = malloc(NUM_REAL_PLACES * sizeof(PlaceId));
	placesFill(pred, NUM_REAL_PLACES, -1);
	pred[src] = src;
	
	Queue q1 = QueueNew(); // current round locations
	Queue q2 = QueueNew(); // next round locations
	
	QueueEnqueue(q1, src);
	while (!(QueueIsEmpty(q1) && QueueIsEmpty(q2))) {
		PlaceId curr = QueueDequeue(q1);
		int numReachable = 0;
		PlaceId *reachable = GvGetReachable(hv->gv, hunter, r, curr,
		                                    &numReachable);
		
		for (int i = 0; i < numReachable; i++) {
			if (pred[reachable[i]] == -1) {
				pred[reachable[i]] = curr;
				QueueEnqueue(q2, reachable[i]);
			}
		}
		free(reachable);
		
		// When we've exhausted the current round's locations, advance
		// to the next round and swap the queues (so the next round's
		// locations becomes the current round's locations)
		if (QueueIsEmpty(q1)) {
			Queue tmp = q1; q1 = q2; q2 = tmp; // swap queues
			r++;
		}
	}
	
	QueueDrop(q1);
	QueueDrop(q2);
	return pred;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	return HvWhereCanIGoByType(hv, true, true, true, numReturnedLocs);
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	return HvWhereCanTheyGoByType(hv, HvGetPlayer(hv), road, rail,
	                              boat, numReturnedLocs);
}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	return HvWhereCanTheyGoByType(hv, player, true, true, true,
	                              numReturnedLocs);
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	Round round = playerNextRound(hv, player);
	
	PlaceId location = GvGetPlayerLocation(hv->gv, player);
	
	// If the given player hasn't made a move or the given player is
	// Dracula and his location hasn't been revealed, return NULL
	if (round == 0 || !placeIsReal(location)) {
		*numReturnedLocs = 0;
		return NULL;
	}
	
	return GvGetReachableByType(hv->gv, player, round, location, road,
	                            rail, boat, numReturnedLocs);
}

////////////////////////////////////////////////////////////////////////
// Helper functions

/**
 * Gets the round number of the player's next move
 */
static Round playerNextRound(HunterView hv, Player player) {
	return HvGetRound(hv) + (player < HvGetPlayer(hv) ? 1 : 0);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
