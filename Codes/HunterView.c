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
typedef struct QueueRep *Queue;

Queue newQueue (void);			// create new empty queue
void dropQueue (Queue);			// free memory used by queue
void QueueJoin (Queue, PlaceId);	// add item on queue
PlaceId QueueLeave (Queue);		// remove item from queue
int QueueIsEmpty (Queue);		// check for no items
// TODO: ADD YOUR OWN STRUCTS HERE



typedef struct QueueNode {
	PlaceId value;
	struct QueueNode *next;
} QueueNode;

typedef struct QueueRep {
	QueueNode *head; // ptr to first node
	QueueNode *tail; // ptr to last node
} QueueRep;

struct hunterView {
	GameView gv;
	Map	map;
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

HunterView HvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	HunterView new = malloc(sizeof(HunterView));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate HunterView!\n");
		exit(EXIT_FAILURE);
	}
	new->gv = GvNew(pastPlays, messages);
	new->map = MapNew();

	return new;
}

void HvFree(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GvFree(hv->gv);
	MapFree(hv->map);
	free(hv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round HvGetRound(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetRound(hv->gv);
}

Player HvGetPlayer(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetPlayer(hv->gv);
}

int HvGetScore(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetScore(hv->gv);
}

int HvGetHealth(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetHealth(hv->gv,player);
}

PlaceId HvGetPlayerLocation(HunterView hv, Player player)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetPlayerLocation(hv->gv,player);
	
}

PlaceId HvGetVampireLocation(HunterView hv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	return GvGetVampireLocation(hv->gv);
}

////////////////////////////////////////////////////////////////////////
// Utility Functions

PlaceId HvGetLastKnownDraculaLocation(HunterView hv, Round *round)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION

	int numMoves = 0; bool canFree = false;
	PlaceId *moves = GvGetMoveHistory(hv->gv, PLAYER_DRACULA, &numMoves, &canFree);
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

PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	PlaceId src = HvGetPlayerLocation(hv, hunter);
	assert (hv->map != NULL);
	int num_place = MapNumPlaces(hv->map);

	int *visited = malloc(num_place * sizeof(PlaceId));
   	for (int i = 0; i < num_place; i++) visited[i] = -1;
	
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
			int turn = HvGetRound(hv);
			
			for(PlaceId i = cur; i != src; i = visited[i]) {
				turn ++;
			}
			if(HvGetPlayer(hv) != 0) {
				turn++;
			}
		
			int temp = 0;
		
			PlaceId *locs = GvGetReachable(hv->gv, hunter, turn, cur, &temp);
			for (int i = 0; i < temp; i++) {
				// make sure each side meet the condition.
                if (visited[locs[i]] == -1 ) {
					visited[locs[i]] = cur;
                    QueueJoin(q, locs[i]);
					
                }
            }	
		}
	}
	
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

PlaceId *HvWhereCanIGo(HunterView hv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	int temp = 0;
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != 0) {
		turn++;
	}
	PlaceId *locs = GvGetReachable(hv->gv, 
	HvGetPlayer(hv), turn,
	HvGetPlayerLocation(hv, HvGetPlayer(hv)), &temp);
	*numReturnedLocs = temp;
	return locs;
}

PlaceId *HvWhereCanIGoByType(HunterView hv, bool road, bool rail,
                             bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != 0) {
		turn++;
	}
	int temp = 0;
	PlaceId *locs = GvGetReachableByType(hv->gv, HvGetPlayer(hv), turn,
	HvGetPlayerLocation(hv, HvGetPlayer(hv)), road, rail, boat, &temp);
	*numReturnedLocs = temp;
	return locs;

}

PlaceId *HvWhereCanTheyGo(HunterView hv, Player player,
                          int *numReturnedLocs)
{
	*numReturnedLocs = 0;
	
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != 0) {
		turn++;
	}
	int temp = 0;
	PlaceId *locs = GvGetReachable(hv->gv, player, turn,
	HvGetPlayerLocation(hv, player), &temp);
	*numReturnedLocs = temp;
	return locs;
}

PlaceId *HvWhereCanTheyGoByType(HunterView hv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	
	Round turn = HvGetRound(hv);
	if(HvGetPlayer(hv) != 0) {
		turn++;
	}
	int temp = 0;
	PlaceId *locs = GvGetReachableByType(hv->gv, player, turn,
	HvGetPlayerLocation(hv, player), road, rail, boat, &temp);
	*numReturnedLocs = temp;
	return locs;
	
	
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO

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
