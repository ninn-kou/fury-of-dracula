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
/*
#include "GameView.h"
#include "Map.h"
#include "Places.h"
#include <assert.h>
#include "Queue.h"\
*/
int check_valid (DraculaView dv,PlaceId mach, Player hunter) {
	int numReturnedLocs = -1;
	PlaceId *path = DvWhereCanTheyGo(dv, hunter, &numReturnedLocs);
	for (int i = (numReturnedLocs - 1); i >= 0; i --) {
		
		if(path[i] == mach) {
			free(path);

			return 0;
		}
	}
	free(path);
	return 1;
}
void decideDraculaMove(DraculaView dv) {
	
	Round round = DvGetRound(dv);
	PlaceId me_loc = DvGetPlayerLocation(dv, 4);

	/*
	PlaceId loc01 = DvGetPlayerLocation(dv, 0);
	PlaceId loc02 = DvGetPlayerLocation(dv, 1);
	PlaceId loc03 = DvGetPlayerLocation(dv, 2);
	PlaceId loc04 = DvGetPlayerLocation(dv, 3);



	PlaceId decide_loc;
	PlaceId next_loc;
	Round temp;
	*/
	if(round == 0 && me_loc == NOWHERE) {
		registerBestPlay("RO", "I am comming.");
		return;
	}
	int num_adjacent = 0;

	PlaceId *adjacent = DvGetValidMoves(dv, &num_adjacent);
	int number = 0;
	int save = -1;
	int savei = 0;
	int i = num_adjacent - 1;
	PlaceId next = NOWHERE;
	
	while( i >= 0) {
		
		if(check_valid (dv, adjacent[i], 0)
		&& check_valid (dv, adjacent[i], 1)
		&& check_valid (dv, adjacent[i], 2)
		&& check_valid (dv, adjacent[i], 3)){
		next = adjacent[i];
		break;

		
		} else {
		number = check_valid (dv, adjacent[i], 0)
		+ check_valid (dv, adjacent[i], 1)
		+ check_valid (dv, adjacent[i], 2)
		+ check_valid (dv, adjacent[i], 3);
			if(number > save) {
				save = number;
				savei = i;
			}
		}
		i--;
	}
	if (next != NOWHERE) {
		next = adjacent[savei];
	}
	free(adjacent);
	registerBestPlay(placeIdToAbbrev(next), "AHHHHAHHHHAHHHHA.");
	return;

}





/*
void bubble_sort(int a[],int n);
void move_forward(int *array, int size);
Player Playerjudger(int *array, int length, int q, int w, int e, int r);
PlaceId *DvGetShortestPathTo(DraculaView dv, Player hunter, PlaceId dest,
                             int *pathLength);
PlaceId *hunterBfs(DraculaView dv, Player hunter, PlaceId src, Round r);
static Round playerNextRound(DraculaView dv, Player player);


static Round playerNextRound(DraculaView dv, Player player) {
	return (DvGetRound(dv) + 1);
}

PlaceId *hunterBfs(DraculaView dv, Player hunter, PlaceId src, Round r) {
	PlaceId *pred = malloc(NUM_REAL_PLACES * sizeof(PlaceId));
	for (int i = 0; i < NUM_REAL_PLACES; i++) pred[i] = NOWHERE;
	pred[src] = src;
	
	Queue q1 = QueueNew(); // current round locations
	Queue q2 = QueueNew(); // next round locations
	
	QueueEnqueue(q1, src);
	while (!(QueueIsEmpty(q1) && QueueIsEmpty(q2))) {
		PlaceId curr = QueueDequeue(q1);
		int numReachable = 0;
		PlaceId *reachable = DvWhereCanTheyGo(dv, hunter,&numReachable);
		
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
PlaceId *DvGetShortestPathTo(DraculaView dv, Player hunter, PlaceId dest,
                             int *pathLength)
{
	Round r = playerNextRound(dv, hunter);
	PlaceId src = DvGetPlayerLocation(dv, hunter);
	PlaceId *pred = hunterBfs(dv, hunter, src, r);
	
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
	if (DvGetRound(dv) == 0) {
		registerBestPlay("CD", "Mwahahahaha");
		return;
	}
	char *best_city;
	// TODO: Replace this with something better!
	int count = -1;
	PlaceId *adjacent = DvWhereCanIGo(dv, &count);	
	PlaceId curr = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	int curr_hunter[4];	
	PlaceId *path = malloc(NUM_REAL_PLACES * sizeof(PlaceId));;
	path = DvGetShortestPathTo(dv, PLAYER_LORD_GODALMING, curr, &curr_hunter[0]);	
	path = DvGetShortestPathTo(dv, PLAYER_DR_SEWARD, curr, &curr_hunter[1]);	
	path = DvGetShortestPathTo(dv, PLAYER_VAN_HELSING, curr, &curr_hunter[2]);	
	path = DvGetShortestPathTo(dv, PLAYER_MINA_HARKER, curr, &curr_hunter[3]);
    // find hunter from close to far
	// save curr_hunter before move forward
	int curr_hunter_0 = curr_hunter[0];
	int curr_hunter_1 = curr_hunter[1];
	int curr_hunter_2 = curr_hunter[2];
	int curr_hunter_3 = curr_hunter[3];
	int size = 4;
	bubble_sort(curr_hunter, size);
	Player Close_1 = Playerjudger(curr_hunter, 4, curr_hunter_0, curr_hunter_1, curr_hunter_2, curr_hunter_3);
	move_forward(curr_hunter, size);
	Player Close_2 = Playerjudger(curr_hunter, 4, curr_hunter_0, curr_hunter_1, curr_hunter_2, curr_hunter_3);
	move_forward(curr_hunter, size);
	Player Close_3 = Playerjudger(curr_hunter, 4, curr_hunter_0, curr_hunter_1, curr_hunter_2, curr_hunter_3);
	move_forward(curr_hunter, size);
	Player Close_4 = Playerjudger(curr_hunter, 4, curr_hunter_0, curr_hunter_1, curr_hunter_2, curr_hunter_3);
	move_forward(curr_hunter, size);
	
	int array1[1000] = {-1};
	int array1_count = 0;
	int i = count - 1;
	int length = 0;	
	
	while (i >= 0) {
		path = DvGetShortestPathTo(dv, Close_1, adjacent[i], &length);
	    if (curr_hunter[0] < length) {
	        array1[array1_count] = adjacent[i];
	        array1_count++;
	    }
		i--;
    }
    
	if (array1_count == 0) {
		best_city = placeIdToAbbrev(adjacent[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	////////////////////////////
	} else {
		best_city = placeIdToAbbrev(array1[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	}


	
	// check the second close 
	int array2[1000] = {-1};
	int array2_count = 0;
	i = array1_count - 1;
	
	while (i >= 0) {
		path = DvGetShortestPathTo(dv, Close_2, array1[i], &length);
		if (curr_hunter_1 < length) {
			array2[array2_count] = array1[i];
			array2_count++;
		}
		i--;
	}
	if (array2_count == 0) {
		best_city = placeIdToAbbrev(array1[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	}
    // check the third close
	int array3[1000] = {-1};
	int array3_count = 0;
	i = array2_count - 1;
	
	while (i >= 0) {
		path = DvGetShortestPathTo(dv, Close_3, array2[i], &length);
		if (curr_hunter_2 < length) {
			array3[array3_count] = array2[i];
			array3_count++;
		}
		i--;
	}
	if (array3_count == 0) {
		best_city = placeIdToAbbrev(array2[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	}

	// check the forth close
	int array4[1000] = {-1};
	int array4_count = 0;
	i = array3_count - 1;
	// our teammate's name is Ren
	while (i >= 0) {
		path = DvGetShortestPathTo(dv, Close_4, array3[i], &length);
		if (curr_hunter_3 < length) {
				array4[array4_count] = array3[i];
				array4_count++;
		}
		i--;
	}
	free(path);
	if (array4_count == 0) {
		best_city = placeIdToAbbrev(array3[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	} else {
		best_city = placeIdToAbbrev(array4[0]);
		registerBestPlay(best_city, "Mwahahahaha");
		return;
	}
}

*/
