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
#include "DraculaView.c"
#include "HunterView.h"
#include "HunterView.c"
#include "Game.h"
#include "Places.h"
void bubble_sort(int a[],int n);
PlaceId *HvGetShortestPathTo(HunterView hv, Player hunter, PlaceId dest,
                             int *pathLength);
void move_forward(int *array, int size);
Player Playerjudger(int *array, int length, int q, int w, int e, int r);


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
void move_forward(int *array, int size){
	int i = 0;
	while (i < size - 1) {
		array[i] = array[i+1];
		i++;
	}
	array[i] = -1;
}
Player Playerjudger(int *array, int length, int q, int w, int e, int r) {
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
	char *best_city;
	// TODO: Replace this with something better!
	int count = -1;
	PlaceId *adjacent = DvWhereCanIGo(dv, &count);
	PlaceId hunter0 = DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING);
	PlaceId hunter1 = DvGetPlayerLocation(dv, PLAYER_DR_SEWARD);
	PlaceId hunter2 = DvGetPlayerLocation(dv, PLAYER_VAN_HELSING);
	PlaceId hunter3 = DvGetPlayerLocation(dv, PLAYER_MINA_HARKER);
	PlaceId curr = DvGetPlayerLocation(dv, PLAYER_DRACULA);
	int *curr_hunter;
	PlaceId *path0;
	PlaceId *path1;
	PlaceId *path2;
	PlaceId *path3;	
	path0 = HvGetShortestPathTo(hv, PLAYER_LORD_GODALMING, curr, &curr_hunter[0]);	
	path1 = HvGetShortestPathTo(hv, PLAYER_DR_SEWARD, curr, &curr_hunter[1]);	
	path2 = HvGetShortestPathTo(hv, PLAYER_VAN_HELSING, curr, &curr_hunter[2]);	
	path3 = HvGetShortestPathTo(hv, PLAYER_MINA_HARKER, curr, &curr_hunter[3]);
    // find hunter from close to far
	int length = 4;
	bubble_sort(curr_hunter, length);
	// save curr_hunter before move forward
	int curr_hunter_0 = curr_hunter[0];
	int curr_hunter_1 = curr_hunter[1];
	int curr_hunter_2 = curr_hunter[2];
	int curr_hunter_3 = curr_hunter[3];
	Player Close_1 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, length);
	Player Close_2 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, length);
	Player Close_3 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, length);
	Player Close_4 = Playerjudger(curr_hunter, 4, curr_hunter[0], curr_hunter[1], curr_hunter[2], curr_hunter[3]);
	move_forward(curr_hunter, length);
	int *array1;
	int array1_count = 0;
	int i = count - 1;
	int length = 0;	
	// our teammate's name is Eason
	PlaceId *Eason;
	while (i >= 0) {
		Eason = HvGetShortestPathTo(hv, Close_1, adjacent[i], &length);
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
	int *array2;
	int array2_count = 0;
	i = array1_count - 1;
	// our teammate's name is Charles
	PlaceId *Charles;
	while (i >= 0) {
		Charles = HvGetShortestPathTo(hv, Close_2, array1[i], &length);
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
	int *array3;
	int array3_count = 0;
	i = array2_count - 1;
	// our teammate's name is Steve
	PlaceId *Steve;
	while (i >= 0) {
		Steve = HvGetShortestPathTo(hv, Close_3, array2[i], &length);
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
	int *array4;
	int array4_count = 0;
	i = array3_count - 1;
	// our teammate's name is Ren
	PlaceId *Ren;
	Ren = HvGetShortestPathTo(hv, Close_4, array3[i], &length);
	if (curr_hunter_3 < length) {
			array4[array4_count] = array3[i];
			array4_count++;
		}
		i--;
	}
	if (array4_count == 0) {
		best_city = placeIdToAbbrev(array3[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	} else {
		best_city = placeIdToAbbrev(array4[0]);
		registerBestPlay(best_city, "Mwahahahaha");
	}
}
