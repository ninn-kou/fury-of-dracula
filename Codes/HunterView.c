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

/////////////////////////////////////////////////////////////
//                 my own interface functions              //
/////////////////////////////////////////////////////////////
typedef struct QueueRep *Queue;
// using queue struct for BFS finding for shortes path
Queue newQueue (void);			// create new empty queue
void dropQueue (Queue);			// free memory used by queue
void QueueJoin (Queue, PlaceId);	// add item on queue
PlaceId QueueLeave (Queue);		// remove item from queue
int QueueIsEmpty (Queue);		// check for no items



// struct using by queue
typedef struct QueueNode {
	PlaceId value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head; // ptr to first node
	QueueNode *tail; // ptr to last node
} QueueRep;
// struct using by hunter view
struct hunterView {
	GameView gv;
	Map	map;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor
// create and malloc info we need 
HunterView HvNew(char *pastPlays, Message messages[])
{
	HunterView new = malloc(sizeof(HunterView));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}
	new->gv = GvNew(pastPlays, messages);
	new->map = MapNew();

	return new;
}
// free the info 
void HvFree(HunterView hv)
{
	GvFree(hv->gv);
	MapFree(hv->map);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information
//Get the current round number
Round HvGetRound(HunterView hv) 
{
	
	return GvGetRound(hv->gv);
}

// Get the current player
Player HvGetPlayer(HunterView hv) 
{
	
	return GvGetPlayer(hv->gv);
}

// Gets the current game score
int HvGetScore(HunterView hv)
{
	return GvGetScore(hv->gv);
}

// Gets the current health points for the given player; an value between
// 0 and 9 for a hunter, or >= 0 for Dracula.
int HvGetHealth(HunterView hv, Player player)
{
	
	return GvGetHealth(hv->gv,player);
}

// return the curr location for each "player"
PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	
	return GvGetPlayerLocation(hv->gv,player);
	
}

// return real placeID FOR the vampire location ID 
// if the vampire is not exist
// return nowhere
PlaceId HvGetVampireLocation(HunterView hv)
{
	
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

// return the real placeID if we konw the last dracula location
// if we don't know it 
// or the location cannot see 
// just return nowhere.
PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	int numMoves = 0; 
	bool canFree = false;
	PlaceId *moves = GvGetLocationHistory(hv->gv, PLAYER_DRACULA, &numMoves, &canFree);
	numMoves --;
	while (numMoves >= 0 ) {
		if(moves[numMoves] < NUM_REAL_PLACES) {
			*round = numMoves;
			return(moves[numMoves]);
		}
		numMoves--;
	};
	
	*round = 0;	
	return NOWHERE;
}

// get the shortest path to dest for the player "hunter"
// using the BFS way and Queue method to grab the placeid
PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	PlaceId src = HvGetPlayerLocation(hv, hunter);
	assert (hv->map != NULL);
	int num_place = MapNumPlaces(hv->map);
	// reate the visted array, to record the place, 
	// we have visted before
	int *visited = malloc(num_place * sizeof(PlaceId));
   	for (int i = 0; i < num_place; i++) visited[i] = -1;
	// reate the path array, to record the real place, 
	// we needed.
	int *path = malloc(num_place * sizeof(PlaceId));
   	for (int i = 0; i < num_place; i++) path[i] = NOWHERE;
	
	int found = 0;
	visited[src] = src;

    Queue q = newQueue();
    QueueJoin(q, src);
	
	// make sure we have not found the dest
	while(!found && !QueueIsEmpty(q)) {
		
		PlaceId cur = QueueLeave(q);
		// if we find, take found be true, and stop while.
		if(cur == dest) {
			found = 1;
		
		} else{

			// using round to record the round_number for each
			// city turn.
			int round = HvGetRound(hv);
			// check how many round for this city
			// check how many roads from present city to src.
			for(PlaceId i = cur; i != src; i = visited[i]) {
				round ++;
			}
			// if current player is not the play_godalming
			// we need adding the round 
			// as every player's move is in the next round
			if(HvGetPlayer(hv) != PLAYER_LORD_GODALMING) {
				round++;
			}
		
			int temp = 0;
			// using visited array, and connect each placeid and its parents id.
			PlaceId *locs = GvGetReachable(hv->gv, hunter, round, cur, &temp);
			for (int i = 0; i < temp; i++) {
				// make sure each side meet the condition.
                if (visited[locs[i]] == -1 ) {
					visited[locs[i]] = cur;
                    QueueJoin(q, locs[i]);
					
                }
            }	
		}
	}
	// if we have got the dest one,
	if(found) {

		// first time, record the pathLength in reverse order.
		PlaceId copy = dest;
		int number = 0;
		while(copy != src){
			path[number] = copy;		
			copy = visited[copy]; 
			number++;
		}
		path[number] = src;
		// get the correct number of sides.
		int fixed_number = number;
		
		// reverse the order and get the correct one
		for(int i = 0; i <= (number/2); i++){
			copy = path[number - i];
			path[number - i] = path[i];
			path[i] = copy;
		}
		// then move forward one to deleting the src city
		for (int i = 0; i < fixed_number; i++) {
			path[i] = path[i+1];
			path[i+1] = NOWHERE;
		}
		
		*pathLength = fixed_number;
		

	}
	 
	return path;
}

////////////////////////////////////////////////////////////////////////
// Making a Move
// grab the city which is reachable for the current player.
PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	// initial the info
	*numReturnedLocs = 0;
	PlaceId curr_place = HvGetPlayerLocation(hv, HvGetPlayer(hv));
	// if the current have not moved before
	// just return NULL.
	if(curr_place == NOWHERE) {
		return NULL;
	}
	// else we reusing GvGetReachable fuction to grab cities
	PlaceId *locs = GvGetReachable(hv->gv, 
	HvGetPlayer(hv), HvGetRound(hv), curr_place, numReturnedLocs);
	
	return locs;
}

// grab the city which is reachable for the current player 
// by diff transport method.
PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	// initial the info
	*numReturnedLocs = 0;
	PlaceId curr_place = HvGetPlayerLocation(hv, HvGetPlayer(hv));
	// if the current have not moved before
	// just return NULL.
	if(curr_place == NOWHERE) {
		return NULL;
	}

	// else we reusing GvGetReachable fuction to grab cities
	PlaceId *locs = GvGetReachableByType(hv->gv, HvGetPlayer(hv), 
	HvGetRound(hv), curr_place, road, rail, boat, numReturnedLocs);
	
	return locs;
}

// grab the city which is reachable for the player.
PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	PlaceId curr_place = HvGetPlayerLocation(hv, player);

	// for the cases :
	// 1. dracula start from unknow place
	// 2. hunter no move
	// we need return NULL
	if(player == PLAYER_DRACULA && (curr_place == CITY_UNKNOWN 
	||curr_place == NOWHERE|| curr_place == SEA_UNKNOWN)) {
		return NULL;
	}

	if(player != PLAYER_DRACULA && curr_place == NOWHERE) {
		return NULL;
	}
	// if current player is not the play_godalming
	// we need adding the round 
	// as every player's move is in the next round
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != PLAYER_LORD_GODALMING) {
		turn++;
	}
	// we using GvGetReachable fuction to grab cities
	PlaceId *locs = GvGetReachable(hv->gv, player, turn,
	curr_place, numReturnedLocs);
	return locs;
}

// grab the city which is reachable for the player 
// by diff transport method.
PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	PlaceId curr_place = HvGetPlayerLocation(hv, player);

	// for the cases :
	// 1. dracula unknow
	// 2. hunter no move
	if(player == PLAYER_DRACULA && (curr_place == CITY_UNKNOWN ||
	curr_place == NOWHERE|| curr_place == SEA_UNKNOWN)) {
		return NULL;
	}

	if(player != PLAYER_DRACULA && curr_place == NOWHERE) {
		return NULL;
	}
	// if current player is not the play_godalming
	// we need adding the round 
	// as every player's move is in the next round
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != PLAYER_LORD_GODALMING) {
		turn++;
	}
	// we using GvGetReachable fuction to grab cities by diff transport methods
	PlaceId *locs = GvGetReachableByType(hv->gv, player, turn,
	curr_place, road, rail, boat, numReturnedLocs);
	return locs;
}

/////////////////////////////////////////////////////////////
//                 my own interface functions              //
/////////////////////////////////////////////////////////////

// create new empty Queue
Queue newQueue (void)
{
	QueueRep *new = malloc (sizeof *new);
	*new = (QueueRep){ .head = NULL, .tail = NULL };
	return new;
}

// free memory used by Queue
void dropQueue (Queue Q)
{
	assert (Q != NULL);
	for (QueueNode *curr = Q->head, *next; curr != NULL; curr = next) {
		next = curr->next;
		free (curr);
	}
	free (Q);
}



// add item at end of Queue
void QueueJoin (Queue Q, PlaceId it)
{
	assert (Q != NULL);

	QueueNode *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (QueueNode){ .value = it, .next = NULL };

	if (Q->head == NULL)
		Q->head = new;
	if (Q->tail != NULL)
		Q->tail->next = new;
	Q->tail = new;
}

// remove item from front of Queue
PlaceId QueueLeave (Queue Q)
{
	assert (Q != NULL);
	assert (Q->head != NULL);
	PlaceId it = Q->head->value;
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL)
		Q->tail = NULL;
	free (old);
	return it;
}

// check for no items
int QueueIsEmpty (Queue Q)
{
	return (Q->head == NULL);
}
