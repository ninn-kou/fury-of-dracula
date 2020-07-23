////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
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
#include "Map.h"
#include "Places.h"
// add your own #includes here
//void hunter_condition(char c, PlayerData hunter, PlayerData dracula, Young_vampire vampire, int list[NUM_REAL_PLACES][MAXIMUM_TRAP]);
// TODO: ADD YOUR OWN STRUCTS HERE
#define MAXMUM_TRAP 6
#define NOT_FIND -100
typedef struct playerData {
	Player ID;
	int HP;
	PlaceId currlocation;
	PlaceId playerTrail[TRAIL_SIZE];    
} PlayerData;

typedef struct young_vampire{
	
	int survive;    // return 1 for true, 0 for died.
	int born_round_number; // record which turn the vampire born.
	PlaceId born_location;  // // record which city the vampire born.
} *Young_vampire;

struct gameView {
	
	int score;
	Round turn_Number;

	Player Curr_Player_Number;
	PlayerData *player[NUM_PLAYERS];

	int num_traps;  // return the sum of traps
	
    PlaceId traplist[TRAIL_SIZE]; // the 1d version of traplist
                    			  // traplist[6]
    
    
	
	Young_vampire vampire;  // point out the info of vampire

	Map map;
};
/*
void hunter_condition(char c, PlayerData hunter, PlayerData dracula, Young_vampire vampire, int list[NUM_REAL_PLACES][MAXIMUM_TRAP]) {
	int j = 0;
	switch(c){
	case T:
		hunter->HP = player->HP - LIFE_LOSS_TRAP_ENCOUNTER;
		while (trap_list[place_id][j] != 1) {
			j++;
		}
		trap_list[place_id][j] = 0;
		break;
	case V:
		vampire->survive = 0;
		while (trap_list[place_id][j] != 2) {
			j++;
		}
		trap_list[place_id][j] = 0;
		break;
	case D:
		hunter->HP = player->HP - LIFE_LOSS_DRACULA_ENCOUNTER;
		dracula->HP = dracula->HP - LIFE_LOSS_HUNTER_ENCOUNTER;
		dracula->currlocation = strcpy(place);
		break;
	case .:
		break;
	}
}
*/
////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
    // CREATE THE WHOLE STRUCT OF TRAPLIST;
    // malloc sizeof(placeid)*MAX_REAL_PLACE+1)*3
	/*
    new->trap_list = malloc(sizeof(PlaceId)*(MAX_REAL_PLACE+1)*MAXMUM_TRAP);
    for(PlaceId city = MIN_REAL_PLACE; city < (MAX_REAL_PLACE + 1); city++) {
        for(int i = 0; i < MAXMUM_TRAP; i++) {
            // makesure the trap for all the city is zero;
            new->trap_list[city][i] = 0;
        }
    }; 
	*/


	/*
	PlayerData Godalming = new->players[0]->ID
	PlayerData Seward = new->players[1]->ID
	PlayerData Helsing = new->players[2]->ID
	PlayerData Mina = new->players[3]->ID
	PlayerData Dracula = new->players[4]->ID





	int pastPlays_length = strlen(pastPlays)
	int round = pastPlays_length/40
	new->score = GAME_START_SCORE - round * SCORE_LOSS_DRACULA_TURN; 		// game start at score 366
	new->turn_Number = round;        										// in pastplays, 40 characters are one round
	new->Curr_PLayer_Number = PLAYER_LORD_GODALMING;
	Godalming->ID = PLAYER_LORD_GODALMING;							// the first player is Lord Godalming
	Godalming->HP = GAME_START_HUNTER_LIFE_POINTS;
	Godalming->currlocation = NOWHERE;
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Godalming->playerTrail[i] = NOWHERE;
	}
		Godalming->currlocation = NOWHERE;
	Seward->ID = PLAYER_DR_SEWARD;									// the first player is Dr. Seward
	Seward->HP = GAME_START_HUNTER_LIFE_POINTS;
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Seward->playerTrail[i] = NOWHERE;
	}
		Seward->currlocation = NOWHERE;
	Helsing->ID = PLAYER_VAN_HELSING;								// the first player is Van Helsing
	Helsing->HP = GAME_START_HUNTER_LIFE_POINTS;
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Helsing->playerTrail[i] = NOWHERE;
	}
		Helsing->currlocation = NOWHERE;
	Mina->ID = PLAYER_MINA_HARKER;								// the first player is Mina Harker
	Mina->HP = GAME_START_HUNTER_LIFE_POINTS;
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Mina->playerTrail[i] = NOWHERE;
	}
		Mina->currlocation = NOWHERE;
	Dracula->ID = PLAYER_DRACULA;									// the first player is Mina Harker
	Dracula->HP = GAME_START_BLOOD_POINTS;
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Dracula->playerTrail[i] = NOWHERE;
	}
		Dracula->currlocation = NOWHERE;
	new->num_traps = 0;
	new->trap_list = NULL;
	new->map = MapNew();

	int pastPlays_counter = 0;
	int pastPlaysID = 0;
	char place[2];
	int k = 0;
	while (pastPlays_counter < length) {
		pastPlaysID = pastPlays_counter % 40					// find whose turn
		// now is looking at Godalming
		if (pastPlaysID > 0 && pastPlaysID < 3) {
			place[k] = pastplays[pastPlays_counter]
			K++;
		}
		if (k == 2) {
			Godalming->currlocation = strcpy(place);
		}
		if (pastPlaysID > 2 && pastPlaysID < 7) {
			hunter_condition(pastPlays[pastPlays_counter], Godalming, Dracula, new->vampire, new->trap_list);
			k = 0;
		}
		// now is looking at Seward
		if (pastPlaysID > 8 && pastPlaysID < 11) {
			place[k] = pastplays[pastPlays_counter]
			K++;
		}
		if (k == 2) {
			Seward->currlocation = strcpy(place);
		}
		if (pastPlaysID > 10 && pastPlaysID < 15) {
			hunter_condition(pastPlays[pastPlays_counter], Seward, Dracula, new->vampire, new->trap_list);
			k = 0;
		}
		// now is looking at Helsing
		if (pastPlaysID > 16 && pastPlaysID < 19) {
			place[k] = pastplays[pastPlays_counter]
			K++;
		}
		if (k == 2) {
			Helsing->currlocation = strcpy(place);
		}
		if (pastPlaysID > 18 && pastPlaysID < 23) {
			hunter_condition(pastPlays[pastPlays_counter], Helsing, Dracula, new->vampire, new->trap_list);
			k = 0;
		}
		// now is looking at Mina
		if (pastPlaysID > 24 && pastPlaysID < 27) {
			place[k] = pastplays[pastPlays_counter]
			K++;
		}
		if (k == 2) {
			Mina->currlocation = strcpy(place);
		}
		if (pastPlaysID > 26 && pastPlaysID < 31) {
			hunter_condition(pastPlays[pastPlays_counter], Mina, Dracula, new->vampire, new->trap_list);
			k = 0;
		}
		// now is looking at Dracula
		if (pastPlaysID > 32 && pastPlaysID < 35) {
			place[k] = pastplays[pastPlays_counter]
			K++;
		}
		if (k == 2) {
			Dracula->currlocation = strcpy(place);
			k = 0;
		}
		if (pastPlaysID == 35 && pastPlays[pastPlays_counter] == "T"){
			
			}
		}
		pastPlays_counter++;
		*/
	
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	// gets the current round number
	return (gv->turn_Number);
}

Player GvGetPlayer(GameView gv)
{
	// Gets the current player;
	return (gv->Curr_Player_Number);
}

int GvGetScore(GameView gv)
{
	//Gets the current game score;
	return (gv->score);
}

int GvGetHealth(GameView gv, Player player)
{
	// find the correct player, then  return the info
	int temp = 0;
	int blood = 0;
	while (temp < NUM_PLAYERS) {
		if(gv->player[temp]->ID == player){
			blood = gv->player[temp]->HP;
		}
	}
	
	return (blood);
	
}
//ssh test

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	int temp = 0;
	// find the player form the playerlist,
	// then return the location,
	while (temp < NUM_PLAYERS) {
		if(gv->player[temp]->ID == player){
			return (gv->player[temp]->currlocation);
		}
	}
	
	return NOWHERE;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	// if the young vampire died, return nowhere;
	if (!gv->vampire->survive) {
		return NOWHERE;	
	}
	
	// else just return the info
	return (gv->vampire->born_location);
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	int *trap = malloc(sizeof(PlaceId)*MAXMUM_TRAP);
	for(int i = 0; i < 6; i++) {
		trap[i] = '\0';
	}
	
	int j = 0;
	for(int i = 0; i < 6; i++) {
		if(gv->traplist[i] != NOWHERE){
			trap[j] = gv->traplist[i];
			j++;
		}
	}
    *numTraps = j;

    return trap;
    
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

int Firstround(ConnList whole, int *array, int i) {
       
    while (whole != NULL && whole->type == RAIL) {        
        array[i] = whole->p;             
        i++;
        whole = whole->next;
    } 
    return i;    
}
int removeDuplicates(int* nums, int numsSize)
{
    if(numsSize==0) {
        return 0;
    }
    int *a1,*a2;
    a1 = nums; 
    a2 = a1 + 1;
    int j = 1;
    for(int i = 1 ;i < numsSize; i++)
    {
        if(*a1 != *a2) {
            a1 = a1+1;
            j++;
            *a1 = *a2;
        }
        a2 = a2 + 1;
    }
    return j; 
}
PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	
	ConnList curr = MapGetConnections(gv->map, from);
	int count = 0;
	int dracula = 0;
	int hunter = 0;
	while (curr != NULL && curr->type != RAIL) {
	    if (curr->p == ST_JOSEPH_AND_ST_MARY) {
	            hunter++;
	        } else {
	            dracula++;
	            hunter++;
	        }
	        curr = curr->next;
	}
	if (player == PLAYER_DRACULA) {
	    numReturnedLocs = malloc((dracula + 1) * sizeof(int));
	    while (curr != NULL && curr->type != RAIL) {
	        if (curr->p == ST_JOSEPH_AND_ST_MARY) {
	        } else {
	            numReturnedLocs[count] = curr->p;
	            count++;
	        }
	        curr = curr->next;
	    }
	} else {
	    numReturnedLocs = malloc((hunter + 1)* sizeof(int));
	    while (curr != NULL && curr->type == ROAD) {
	        numReturnedLocs[count] = curr->p;
	        count++;
	        curr = curr->next;
	    }
	}
    int hunter2 = 0;
    while (curr != NULL && curr->type == RAIL) {
        hunter2++;
    }
    if (player == PLAYER_LORD_GODALMING || player == PLAYER_DR_SEWARD || player == PLAYER_VAN_HELSING || player == PLAYER_MINA_HARKER) {    
        int array[1000];       
        int n = 0;
        while (n < 1000) {
            array[n] = -1;
            n++;
        }
        int roadCounter = 0;
        if ((player + round) % 4 == 0) { 
        }
        if ((player + round) % 4 == 1) {  
            roadCounter = Firstround(curr, array, roadCounter);
        }
        if ((player + round) % 4 == 2) {
            int counterArray[1000];            
            int Secondarycounter = 0;
            while (Secondarycounter < 1000) {
                counterArray[Secondarycounter] = -1;
                Secondarycounter++;
            }
            int k = Firstround(curr, counterArray, 0);
            ConnList *Array1 = malloc(k * sizeof(ConnList));            
            int l = k - 1;
            int z = k - 1;
            while (z >= 0) {
                Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                z--;
            }
            while (l >= 0) {
                roadCounter = Firstround(Array1[l], array, roadCounter);
                l--;
            } 
        }
        if ((player + round) % 4 == 3) {
            int superArray[1000];
            int map = 0;
            while (map < 1000) {
                superArray[map] = -1;
                map++;
            }
            
            int o = Firstround(curr, superArray, 0);
            ConnList *listArray1 = malloc((o) * sizeof(ConnList));
            int p = o - 1;
            int y = o - 1;
            while (y >= 0) {
                listArray1[y] = MapGetConnections(gv->map, superArray[y]);
                y--;
            }
            
            int secondaryArray[1000];
            int c = 0;
            while (c < 1000) {
                secondaryArray[1000] = -1;
                c++;
            }
            
            int q = 0;
            while (p >= 0) {
                q = Firstround(listArray1[p], secondaryArray, q);
                p--;
            }
            ConnList *listArray2 = malloc((q) * sizeof(ConnList));
            int counter2 = q - 1;
            int counter3 = q - 1;
            while (counter3 >= 0) {
                listArray2[counter3] = MapGetConnections(gv->map, secondaryArray[counter3]);
                counter3--;
            }
            while (counter2 >= 0) {
                roadCounter = Firstround(listArray2[counter2], array, roadCounter);
                counter2--;
            }
        }
        
        int num_array = removeDuplicates(array, roadCounter - 1);
        numReturnedLocs = realloc(numReturnedLocs, (num_array + 1 + count) * sizeof(int));
        while (num_array > 0) {
            numReturnedLocs[count] = array[num_array];
            count++;
            num_array--;
        } 
    }
    return numReturnedLocs;
}




PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	ConnList adjacent = MapGetConnections(gv->map, from);
	int count = 0;
	if (road) {
	    ConnList curr_1 = MapGetConnections(gv->map, from);
	    int num_1 = 0;
	    int num_2 = 0;
	    while (curr_1 != NULL && curr_1->type == ROAD) {
	        if (curr_1->p == ST_JOSEPH_AND_ST_MARY) {
	            num_1++;
	        } else {
	            num_1++;
	            num_2++;
	        }
	        curr_1 = curr_1->next;
	    }
	    if (player == PLAYER_DRACULA) {
	        numReturnedLocs = malloc((num_2 + 1) * sizeof(int));
	        while (curr_1 != NULL && curr_1->type == ROAD) {
	            if (curr_1->p == ST_JOSEPH_AND_ST_MARY) {
	            } else {
	                numReturnedLocs[count] = curr_1->p;
	                count++;
	            }
	            curr_1 = curr_1->next;
	        }
	     } else {
	        numReturnedLocs = malloc((num_1 + 1)* sizeof(int));
	        while (curr_1 != NULL && curr_1->type == ROAD) {
	            numReturnedLocs[count] = curr_1->p;
	            count++;
	            curr_1 = curr_1->next;
	        }
	    }
	}
	if (rail) {
        ConnList curr = adjacent;
        int roadCounter = 0;
        int array[1000];       
        int n = 0;
        while (n < 1000) {
            array[n] = -1;
            n++;
        }        
        if ((player + round) % 4 == 0) {        
        } else if ((player + round) % 4 == 1) {
            if (player == PLAYER_DRACULA) {
                
            } else {
                roadCounter = Firstround(curr, array, roadCounter);
            }
        } else if ((player + round) % 4 == 2) {            
            if (player == PLAYER_DRACULA) {
                
            } else {
                int counterArray[1000];            
                int Secondarycounter = 0;
                while (Secondarycounter < 1000) {
                    counterArray[Secondarycounter] = -1;
                    Secondarycounter++;
                }
                int k = Firstround(curr, counterArray, 0);
                ConnList *Array1 = malloc(k * sizeof(ConnList));            
                int l = k - 1;
                int z = k - 1;
                while (z >= 0) {
                    Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                    z--;
                }
                while (l >= 0) {
                    roadCounter = Firstround(Array1[l], array, roadCounter);
                    l--;
                }
            }         
        } else { 
            if (player == PLAYER_DRACULA) {
                
            } else {
                int superArray[1000];
                int map = 0;
                while (map < 1000) {
                    superArray[map] = -1;
                    map++;
                }
                
                int o = Firstround(curr, superArray, 0);
                ConnList *listArray1 = malloc((o) * sizeof(ConnList));
                int p = o - 1;
                int y = o - 1;
                while (y >= 0) {
                    listArray1[y] = MapGetConnections(gv->map, superArray[y]);
                    y--;
                }
                
                int secondaryArray[1000];
                int c = 0;
                while (c < 1000) {
                    secondaryArray[1000] = -1;
                    c++;
                }
                
                int q = 0;
                while (p >= 0) {
                    q = Firstround(listArray1[p], secondaryArray, q);
                    p--;
                }
                ConnList *listArray2 = malloc((q) * sizeof(ConnList));
                int counter2 = q - 1;
                int counter3 = q - 1;
                while (counter3 >= 0) {
                    listArray2[counter3] = MapGetConnections(gv->map, secondaryArray[counter3]);
                    counter3--;
                }
                while (counter2 >= 0) {
                    roadCounter = Firstround(listArray2[counter2], array, roadCounter);
                    counter2--;
                }
            }
            
        }
        int num_array = removeDuplicates(array, roadCounter - 1);
        numReturnedLocs = realloc(numReturnedLocs, (count + num_array + 1) * sizeof(int));
        while (num_array > 0) {
            numReturnedLocs[count] = array[num_array];
            count++;
            num_array--;
        }
    }        
	if (boat) {
	    
	    ConnList curr_b_2 = MapGetConnections(gv->map, from);
	    int num_b = 0;
	    while (curr_b_2 != NULL && curr_b_2->type == BOAT) {
	        num_b++;
	        curr_b_2 = curr_b_2->next;
	    }
	    if (player == PLAYER_DRACULA) {
	        numReturnedLocs = realloc(numReturnedLocs, (count + num_b + 1) * sizeof(int));
	        while (curr_b_2 != NULL && curr_b_2->type == BOAT) {
	            numReturnedLocs[count] = curr_b_2->p;
	            count++;
	            curr_b_2 = curr_b_2->next;
	        }
	    } else {
	        numReturnedLocs = realloc(numReturnedLocs, (count + num_b + 1) * sizeof(int));
	        while (curr_b_2 != NULL && curr_b_2->type == BOAT) {
	            numReturnedLocs[count] = curr_b_2->p;
	            count++;
	            curr_b_2 = curr_b_2->next;
	        }	
	}
	}
	
	return numReturnedLocs;
}
////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
