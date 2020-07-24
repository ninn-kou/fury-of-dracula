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
#include <string.h>
void hunter_condition(char c,GameView gv);
void cycling(PlaceId array[TRAIL_SIZE]); 
void check_HP(GameView gv);
void Is_Hunter_rest(GameView gv);
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

    PlaceId traplist[TRAIL_SIZE]; // the 1d version of traplist
                    			  // traplist[6]
 
	Young_vampire vampire;  // point out the info of vampire

	Map map;
};
void Is_Hunter_rest(GameView gv) {
	if (gv->player[gv->Curr_Player_Number]->playerTrail[5] == gv->player[gv->Curr_Player_Number]->playerTrail[4]) {
		int limit_HP = 9;
		gv->player[gv->Curr_Player_Number]->HP += LIFE_GAIN_REST;
		if (gv->player[gv->Curr_Player_Number]->HP > limit_HP) {
			gv->player[gv->Curr_Player_Number]->HP = limit_HP;
		}
	}
}
void check_HP(GameView gv) {
	if (gv->player[gv->Curr_Player_Number]->HP <= 0) {
			gv->player[gv->Curr_Player_Number]->currlocation = HOSPITAL_PLACE;
			cycling(gv->player[gv->Curr_Player_Number]->playerTrail);
			gv->player[gv->Curr_Player_Number]->playerTrail[5] = HOSPITAL_PLACE;
			gv->player[gv->Curr_Player_Number]->HP = GAME_START_HUNTER_LIFE_POINTS;
			gv->score = (gv->score) - SCORE_LOSS_HUNTER_HOSPITAL;
		}
}
void hunter_condition(char c,GameView gv) {

	switch(c){
	case 'T':
		gv->player[gv->Curr_Player_Number]->HP -= LIFE_LOSS_TRAP_ENCOUNTER;
		check_HP(gv);
	
		for (int i = 0; i < 6; i++) {
			if(gv->traplist[i] == 
			gv->player[gv->Curr_Player_Number]->currlocation) {
				gv->traplist[i] = UNKNOWN;
			}
		}
		
		break;
	case 'V':
		gv->vampire->survive = 0;

		break;

	case 'D':
	
		gv->player[gv->Curr_Player_Number]->HP -= LIFE_LOSS_DRACULA_ENCOUNTER;
		gv->player[4]->HP -= LIFE_LOSS_HUNTER_ENCOUNTER;
		gv->player[4]->currlocation = gv->player[gv->Curr_Player_Number]->currlocation;
		check_HP(gv);
		break;
	
	case '.':
		break;
	}
}
void cycling(PlaceId array[TRAIL_SIZE]) {
	for (int i = 0; i < 5; i ++) {
		array[i] = array[i+1];
	}
}

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
	new->player = malloc(5 * sizeof(PlayerData *));
	new->traplist = malloc(sizeof(PlaceId)*6);
	

	
	PlayerData *Godalming = new->player[0];

	PlayerData *Seward = new->player[1];

	PlayerData *Helsing = new->player[2];

	PlayerData *Mina = new->player[3];

	PlayerData *Dracula = new->player[4];





	int pastPlays_length = strlen(pastPlays);
	int round = pastPlays_length/40;
	new->score = GAME_START_SCORE - round * SCORE_LOSS_DRACULA_TURN; 		// game start at score 366
	new->turn_Number = round;
	

	// initialize for the godalming
	new->Curr_Player_Number = PLAYER_LORD_GODALMING;
	Godalming->ID = PLAYER_LORD_GODALMING;							// the first player is Lord Godalming
	Godalming->HP = GAME_START_HUNTER_LIFE_POINTS;
	Godalming->currlocation = NOWHERE;
	Godalming->playerTrail = malloc(sizeof(PlaceId)*6);
	for(int i = 0; i < 6; i++) {
		Godalming->playerTrail[i] = NOWHERE;
	}
	
	// initialize for the seward
	Seward->ID = PLAYER_DR_SEWARD;									// the first player is Dr. Seward
	Seward->HP = GAME_START_HUNTER_LIFE_POINTS;
	Seward->playerTrail = malloc(sizeof(PlaceId)*6);
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Seward->playerTrail[i] = NOWHERE;
	}
	Seward->currlocation = NOWHERE;

	Helsing->ID = PLAYER_VAN_HELSING;								// the first player is Van Helsing
	Helsing->HP = GAME_START_HUNTER_LIFE_POINTS;
	Helsing->playerTrail = malloc(sizeof(PlaceId)*6);
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Helsing->playerTrail[i] = NOWHERE;
	}
	Helsing->currlocation = NOWHERE;
	
	Mina->ID = PLAYER_MINA_HARKER;								// the first player is Mina Harker
	Mina->HP = GAME_START_HUNTER_LIFE_POINTS;
	Mina->playerTrail = malloc(sizeof(PlaceId)*6);
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Mina->playerTrail[i] = NOWHERE;
	}
	Mina->currlocation = NOWHERE;
	
	Dracula->ID = PLAYER_DRACULA;									// the first player is Mina Harker
	Dracula->HP = GAME_START_BLOOD_POINTS;
	Dracula->playerTrail = malloc(sizeof(PlaceId)*6);
	for(int i = 0; i < TRAIL_SIZE; i++) {
		Dracula->playerTrail[i] = NOWHERE;
	}
	Dracula->currlocation = NOWHERE;
	
	
	for(int i = 0; i < 6; i++) {
		new->traplist[i] = NOWHERE;
	}
	new->map = MapNew();

	int pastPlays_counter = 0;
	int pastPlaysID = 0;
	int whose_turn = 0;
	char place[2];
	int k = 0;
	Round round_number = 0;
	while (pastPlays_counter < pastPlays_length) {
		// find whose turn
		pastPlaysID = pastPlays_counter % 40;					
		whose_turn = pastPlaysID / 8;
		new->Curr_Player_Number = whose_turn;
		// set for the counter addition,
		if(pastPlaysID == 0) {
			new->turn_Number = round_number;
			round_number++;
		}

		// now is looking at Godalming
		if (pastPlaysID > 0 && pastPlaysID < 3) {
			place[k] = pastPlays[pastPlays_counter];
			k++;
		}
		
		if (k == 2) {
			Godalming->currlocation = placeAbbrevToId(place);
			cycling(Godalming->playerTrail);
			Godalming->playerTrail[5] = placeAbbrevToId(place);
		}
		if (pastPlaysID > 2 && pastPlaysID < 7) {
			hunter_condition(pastPlays[pastPlays_counter], new);
			k = 0;
		}
		// now is looking at Seward
		if (pastPlaysID > 8 && pastPlaysID < 11) {
			place[k] = pastPlays[pastPlays_counter];
			k++;
		}
		if (k == 2) {
			Seward->currlocation = placeAbbrevToId(place);
			cycling(Seward->playerTrail);
			Seward->playerTrail[5] = placeAbbrevToId(place);
		}
		if (pastPlaysID > 10 && pastPlaysID < 15) {
			hunter_condition(pastPlays[pastPlays_counter], new);
			k = 0;
		}
		// now is looking at Helsing
		if (pastPlaysID > 16 && pastPlaysID < 19) {
			place[k] = pastPlays[pastPlays_counter];
			k++;
		}
		if (k == 2) {
			Helsing->currlocation = placeAbbrevToId(place);
			cycling(Helsing->playerTrail);
			Helsing->playerTrail[5] = placeAbbrevToId(place);
		}
		if (pastPlaysID > 18 && pastPlaysID < 23) {
			hunter_condition(pastPlays[pastPlays_counter], new);
			k = 0;
		}
		// now is looking at Mina
		if (pastPlaysID > 24 && pastPlaysID < 27) {
			place[k] = pastPlays[pastPlays_counter];
			k++;
		}
		if (k == 2) {
			Mina->currlocation = placeAbbrevToId(place);
			cycling(Mina->playerTrail);
			Mina->playerTrail[5] = placeAbbrevToId(place);
		}
		if (pastPlaysID > 26 && pastPlaysID < 31) {
			hunter_condition(pastPlays[pastPlays_counter], new);
			k = 0;
		}
		// now is looking at Dracula
		if (pastPlaysID == 0){
			cycling(new->traplist);
		}
		if (pastPlaysID > 32 && pastPlaysID < 35) {
			place[k] = pastPlays[pastPlays_counter];
			k++;
		}
		if (k == 2) {
			if (placeAbbrevToId(place) < HIDE) {
			Dracula->currlocation = placeAbbrevToId(place);
			} else if (placeAbbrevToId(place) == TELEPORT){
				// TP
				Dracula->currlocation = CASTLE_DRACULA;
			} else {
				// double back && Hide
				Dracula->currlocation = Dracula->playerTrail[HIDE+5-placeAbbrevToId(place)];
			}
				// check wheather in sea
			if (placeIsSea(Dracula->currlocation) == true) {
				Dracula->HP -= LIFE_LOSS_SEA;
				check_HP(new);
			}
			if (Dracula->currlocation == CASTLE_DRACULA) {
				Dracula->HP += LIFE_GAIN_CASTLE_DRACULA;
			}
			// roll the trail
			cycling(Dracula->playerTrail);
			Dracula->playerTrail[5] = Dracula->currlocation;
			k = 0;
		}
		if (pastPlaysID == 35 && pastPlays[pastPlays_counter] == 'T'){

		
		}
		if (pastPlaysID == 36 && pastPlays[pastPlays_counter] == 'V') {
			new->vampire->survive = 1;
			new->vampire->born_round_number = round;
			new->vampire->born_location = Dracula->currlocation;
		}
		if (pastPlaysID == 37 && pastPlays[pastPlays_counter] == 'M') {

		}
		if (pastPlaysID == 38 && pastPlays[pastPlays_counter] == 'V') {
			new->vampire->survive = 0;
			new->vampire->born_round_number= 0;
			new->vampire->born_location = NOWHERE;
			new->score -= SCORE_LOSS_VAMPIRE_MATURES;
		}
		pastPlays_counter++;
	}
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

void bubble_sort(int a[],int n){
    for(int i = 0; i < n - 1; i++){
        bool isSorted = true;
        for(int j = 0; j < n - 1 - i; j++){
            if(a[j] > a[j + 1]){
                isSorted = false;
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1]=temp;
            }  
        }
        if(isSorted) break;
    }
}
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
    if(numsSize == 0) {
        return 0;
    }
    int *a1,*a2;
    a1 = nums; 
    a2 = a1 + 1;
    int j = 1;
    for(int i = 1; i < numsSize; i++)
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
	    numReturnedLocs = malloc((dracula) * sizeof(int));
	    while (curr != NULL && curr->type != RAIL) {
	        if (curr->p == ST_JOSEPH_AND_ST_MARY) {
	        } else {
	            numReturnedLocs[count] = curr->p;
	            count++;
	        }
	        curr = curr->next;
	    }
	} else {
	    numReturnedLocs = malloc((hunter)* sizeof(int));
	    while (curr != NULL && curr->type != RAIL) {
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
        bubble_sort (array, roadCounter);
        int num_array = removeDuplicates(array, roadCounter);
        numReturnedLocs = realloc(numReturnedLocs, (num_array + 1 + count) * sizeof(int));
        int num_counter = num_array - 1;
        while (num_counter > 0) {
            numReturnedLocs[count] = array[num_counter];
            count++;
            num_counter--;
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
        bubble_sort (array, roadCounter);
        int num_array = removeDuplicates(array, roadCounter);
        numReturnedLocs = realloc(numReturnedLocs, (num_array + 1 + count) * sizeof(int));
        int num_counter = num_array - 1;
        while (num_counter > 0) {
            numReturnedLocs[count] = array[num_counter];
            count++;
            num_counter--;
        }
    }        
	if (boat) {	    
	    ConnList curr_boat = MapGetConnections(gv->map, from);
	    int num_b = 0;
	    while (curr_boat != NULL && curr_boat->type == BOAT) {
	        num_b++;
	        curr_boat = curr_boat->next;
	    }
        numReturnedLocs = realloc(numReturnedLocs, (count + num_b + 1) * sizeof(int));
        while (curr_boat != NULL && curr_boat->type == BOAT) {
            numReturnedLocs[count] = curr_boat->p;
            count++;
            curr_boat = curr_boat->next;
        }
	}
	
	return numReturnedLocs;
}
////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
