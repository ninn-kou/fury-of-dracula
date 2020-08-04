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
#include "Places.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include <assert.h>

/*void bubble_sort(int a[],int n);
void move_forward(int *array, int size);
Player Playerjudger(int *array, int length, int q, int w, int e, int r);
typedef struct QueueRep *Queue; 
typedef struct QueueNode {
	PlaceId value;
	struct QueueNode *next;
} QueueNode;
typedef struct QueueRep {
	QueueNode *head;                // Pointer to first node.
	QueueNode *tail;                // Pointer to last node.
} QueueRep;
// Create new empty Queue.
Queue newQueue (void) {
	QueueRep *new = malloc (sizeof *new);
	*new = (QueueRep){ .head = NULL, .tail = NULL };
	return new;
}

// Free memory used by Queue.
void dropQueue (Queue Q) {
	assert (Q != NULL);
	for (QueueNode *curr = Q->head, *next; curr != NULL; curr = next) {
		next = curr->next;
		free (curr);
	}
	free (Q);
}

// Add item at end of Queue.
void QueueJoin (Queue Q, PlaceId it) {
	assert (Q != NULL);

	QueueNode *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (QueueNode){ .value = it, .next = NULL };

	if (Q->head == NULL) Q->head = new;
	if (Q->tail != NULL) Q->tail->next = new;
	Q->tail = new;
}

// Remove item from start of Queue.
PlaceId QueueLeave (Queue Q) {
	assert (Q != NULL);
	assert (Q->head != NULL);
	PlaceId it = Q->head->value;
	QueueNode *old = Q->head;
	Q->head = old->next;
	if (Q->head == NULL) Q->tail = NULL;
	free (old);
	return it;
}

// Check for no items.
int QueueIsEmpty (Queue Q) {
	return (Q->head == NULL);
}
int DvGetShortestPathTo(DraculaView dv, Player hunter, PlaceId dest) {
	int pathLength = 0;
	PlaceId src = DvGetPlayerLocation(dv, hunter);
	//assert (dv->map != NULL);
	int num_place = NUM_REAL_PLACES;

	// Create the visited array, to record the place visited.
	int *visited = malloc(num_place * sizeof(PlaceId));
   	for (int i = 0; i < num_place; i++) visited[i] = -1;

	// Create the path array to record the real place we need.
	int *path = malloc(num_place * sizeof(PlaceId));
   	for (int i = 0; i < num_place; i++) path[i] = NOWHERE;

	int found = 0;
	visited[src] = src;

    Queue q = newQueue();
    QueueJoin(q, src);

	// Make sure we have not found the dest.
	while(!found && !QueueIsEmpty(q)) {
		PlaceId cur = QueueLeave(q);
		// If we find, take found be true, and stop while.
		if(cur == dest) {
			found = 1;
		} else{
			// Using round to record the round_number for each city turn.
			int round = DvGetRound(dv);
			// Check how many round for this city.
			// Check how many roads from present city to src.
			for(PlaceId i = cur; i != src; i = visited[i]) {
				round ++;
			}

			// If current player is not the play_Godalming;
			// We need adding the round as every player's move is in the next round.
			if(GvGetPlayer(dv->gv) != PLAYER_LORD_GODALMING) round++;

			int temp = 0;
			// Using visited array, and connect each placeID and its parents ID.
			PlaceId *locs = GvGetReachable(dv->gv, hunter, round, cur, &temp);
			for (int i = 0; i < temp; i++) {
				// Make sure each side meet the condition.
                if (visited[locs[i]] == -1) {
					visited[locs[i]] = cur;
                    QueueJoin(q, locs[i]);

                }
            }
		}
	}

	// If we have got the dest one.
	if (found) {

		// First time, record the pathLength in reverse order.
		PlaceId copy = dest;
		int number = 0;
		while(copy != src){
			path[number] = copy;
			copy = visited[copy];
			number++;
		}
		path[number] = src;

		// Get the correct number of sides.
		int fixed_number = number;

		// Reverse the order and get the correct one.
		for(int i = 0; i <= (number/2); i++){
			copy = path[number - i];
			path[number - i] = path[i];
			path[i] = copy;
		}

		// Then move forward one to deleting the src city.
		for (int i = 0; i < fixed_number; i++) {
			path[i] = path[i+1];
			path[i+1] = NOWHERE;
		}

		pathLength = fixed_number;
	}
	return pathLength;
}

void bubble_sort(int a[],int n) {
    for (int i = 0; i < n - 1; i++) {
        bool isSorted = true;
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                isSorted = false;
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }  
        }
        if (isSorted) {
            break;
        }
    }
}


void move_forward(int *array, int size){
	int i = 0;
	while (i < size - 1) {
		array[i] = array[i+1];
		i++;
	}
	array[i] = -1;
}
Player Playerjudger(int *array, int length, int q, int w, int e, int r) {
    if (array[0] == q) {
        return PLAYER_LORD_GODALMING;
    } else if (array[0] == w) {
        return PLAYER_DR_SEWARD;
    } else if (array[0] == e) {
        return PLAYER_VAN_HELSING;
    } else if (array[0] == r) {
        return PLAYER_MINA_HARKER;
    }
    return 0;
}
void decideDraculaMove(DraculaView dv)
{
	
	char *best_city;
	// TODO: Replace this with something better!
	int count = -1;
	PlaceId *adjacent = DvWhereCanIGo(dv, &count);	
	PlaceId curr = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	int curr_hunter[4];	
	curr_hunter[0] = DvGetShortestPathTo(dv, PLAYER_LORD_GODALMING, curr);	
	curr_hunter[0] = DvGetShortestPathTo(dv, PLAYER_DR_SEWARD, curr);	
	curr_hunter[0] = DvGetShortestPathTo(dv, PLAYER_VAN_HELSING, curr);	
	curr_hunter[0] = DvGetShortestPathTo(dv, PLAYER_MINA_HARKER, curr);
    // find hunter from close to far
	int size = 4;
	bubble_sort(curr_hunter, size);
	// save curr_hunter before move forward
	int curr_hunter_0 = curr_hunter[0];
	int curr_hunter_1 = curr_hunter[1];
	int curr_hunter_2 = curr_hunter[2];
	int curr_hunter_3 = curr_hunter[3];
	Player Close_1 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, size);
	Player Close_2 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, size);
	Player Close_3 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, size);
	Player Close_4 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, size);
	int array1[1000] = {-1};
	int array1_count = 0;
	int i = count - 1;
	int length = 0;	
	// our teammate's name is Eason
	
	while (i >= 0) {
		length = DvGetShortestPathTo(dv, Close_1, adjacent[i]);
	    if (curr_hunter_0 < length) {
	        array1[array1_count] = adjacent[i];
	        array1_count++;
	    }
		i--;
    }
    
	if (array1_count == 0) {
		best_city = placeIdToAbbrev(adjacent[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	}
	// check the second close 
	int array2[1000] = {-1};
	int array2_count = 0;
	i = array1_count - 1;
	// our teammate's name is Charles
	
	while (i >= 0) {
		length = DvGetShortestPathTo(dv, Close_2, array1[i]);
		if (curr_hunter_1 < length) {
			array2[array2_count] = array1[i];
			array2_count++;
		}
		i--;
	}
	if (array2_count == 0) {
		best_city = placeIdToAbbrev(array1[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	}
    // check the third close
	int array3[1000] = {-1};
	int array3_count = 0;
	i = array2_count - 1;
	// our teammate's name is Steve
	
	while (i >= 0) {
		length = DvGetShortestPathTo(dv, Close_3, array2[i]);
		if (curr_hunter_2 < length) {
			array3[array3_count] = array2[i];
			array3_count++;
		}
		i--;
	}
	if (array3_count == 0) {
		best_city = placeIdToAbbrev(array2[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	}

	// check the forth close
	int array4[1000] = {-1};
	int array4_count = 0;
	i = array3_count - 1;
	// our teammate's name is Ren
	
	length = DvGetShortestPathTo(dv, Close_4, array3[i]);
	if (curr_hunter_3 < length) {
			array4[array4_count] = array3[i];
			array4_count++;
		}
		i--;
	
	if (array4_count == 0) {
		best_city = placeIdToAbbrev(array3[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	} else {
		best_city = placeIdToAbbrev(array4[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	}
	*/
void decideDraculaMove(DraculaView dv)
{
	int numLocs = -1;
    PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
    registerBestPlay(placeIdToAbbrev(locs[0]), "Mwahahahaha");
}
