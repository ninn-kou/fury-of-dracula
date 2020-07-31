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
// here is my own functions
void hunter_condition(char c,GameView gv);
void cycling(PlaceId array[TRAIL_SIZE]);
void check_HP(GameView gv);
void Is_Hunter_rest(GameView gv);
void Is_need_Recovery(GameView gv);
int moving(PlaceId array[TRAIL_SIZE], PlaceId new, int a);

// TODO: ADD YOUR OWN STRUCTS HERE
#define MAXMUM_TRAP 6
#define NOT_FIND -100

#define Godalming (new->player[0])
#define Seward (new->player[1])
#define Helsing (new->player[2])
#define Mina (new->player[3])
#define Dracula (new->player[4])

typedef struct playerData {
	Player ID;
	int HP;
	PlaceId currlocation;
	PlaceId playerTrail[TRAIL_SIZE];
	int num_move;
	PlaceId *movelist;
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

void Is_need_Recovery(GameView gv) {
	// if hunter die, recover him, and move to hospital later...
	if (gv->player[gv->Curr_Player_Number]->HP == 0) {
		gv->player[gv->Curr_Player_Number]->HP = GAME_START_HUNTER_LIFE_POINTS;
	}
}
void Is_Hunter_rest(GameView gv) {
	// check the hunter wheather stay at the same city, if yes ,get 1 HP
	if (gv->player[gv->Curr_Player_Number]->playerTrail[5] 
	== gv->player[gv->Curr_Player_Number]->playerTrail[4]) {
		// hunter's HP can't over 9
		int limit_HP = 9;
		gv->player[gv->Curr_Player_Number]->HP += LIFE_GAIN_REST;
		if (gv->player[gv->Curr_Player_Number]->HP > limit_HP) {
			gv->player[gv->Curr_Player_Number]->HP = limit_HP;
		}
	}
}

void check_HP(GameView gv) {
	// if the hunter's HP is lower than 0, move him to hospital and loss score
	if (gv->player[gv->Curr_Player_Number]->HP <= 0) {
			gv->player[gv->Curr_Player_Number]->currlocation = HOSPITAL_PLACE;
			cycling(gv->player[gv->Curr_Player_Number]->playerTrail);
			gv->player[gv->Curr_Player_Number]->playerTrail[5] = HOSPITAL_PLACE;
			gv->player[gv->Curr_Player_Number]->HP = 0;
			gv->score = (gv->score) - SCORE_LOSS_HUNTER_HOSPITAL;
		}
}


void hunter_condition(char c,GameView gv) {
	// here are 4 encounter for hunters
	// T for trap
	// V for vampire
	// D for encounter dracula
	switch (c){
	case 'T':
		// hunter loss HP and check HP whether is lower than 0
		gv->player[gv->Curr_Player_Number]->HP -= LIFE_LOSS_TRAP_ENCOUNTER;
		check_HP (gv);
		// distroy trap
		for (int i = 0; i < 6; i++) {
			if (gv->traplist[i] ==
			gv->player[gv->Curr_Player_Number]->currlocation) {
				gv->traplist[i] = NOWHERE;
			}
		}
		break;
	case 'V':
		// hunter kill vampire
		gv->vampire->survive = 0;
		
		gv->vampire->born_round_number= -1;
		gv->vampire->born_location = NOWHERE;

		break;
	case 'D':
		// hunter and dracula both loss HP and show dracula's location
		gv->player[gv->Curr_Player_Number]->HP -= LIFE_LOSS_DRACULA_ENCOUNTER;
		gv->player[4]->HP -= LIFE_LOSS_HUNTER_ENCOUNTER;
		gv->player[4]->currlocation = gv->player[gv->Curr_Player_Number]->currlocation;
		check_HP(gv);
		break;
	case '.':
		// nothing happen
		break;
	}
}
// move trail array forward
void cycling(PlaceId array[TRAIL_SIZE]) {
	for (int i = 0; i < 5; i ++) {
		array[i] = array[i+1];
	}
	array[5] = NOWHERE;
}

int moving(PlaceId array[TRAIL_SIZE], PlaceId new, int a) {

	array[a] = new;
	a++;
	return a;
}

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[]) {
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(struct gameView));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}

    // CREATE THE WHOLE STRUCT OF TRAPLIST;
    for (int i = 0; i < 6; i++) {
		new->traplist[i] = NOWHERE;
	}
	// initialize the info foe the young_vampire.
	new->vampire = malloc(sizeof(Young_vampire));
	new->vampire->survive = 0;
	new->vampire->born_round_number = -1;
	new->vampire->born_location = NOWHERE;
	

	int pastPlays_length = strlen(pastPlays);
	int round = pastPlays_length/40;

	new->turn_Number = round;
	new->score = GAME_START_SCORE;

	// initialize for the player[5]
	// aka playerdata,
	for (int i = 0; i < 5; i++) {
		new->player[i] = malloc(sizeof(PlayerData));
		new->player[i]->movelist = malloc(sizeof(PlaceId)*(round+1));
		new->player[i]->ID = i;
		if (i != PLAYER_DRACULA) {
			new->player[i]->HP= GAME_START_HUNTER_LIFE_POINTS;
		} else {
			new->player[i]->HP = GAME_START_BLOOD_POINTS;
		}
		new->player[i]->currlocation = NOWHERE;
		
		for(int j = 0; j < 6; j++) {
		new->player[i]->playerTrail[j] = NOWHERE;
		}
		new->player[i]->num_move = 0;
		for(int j = 0; j <= round; j++) {
		new->player[i]->movelist[j] = NOWHERE;
		}

	}

	new->map = MapNew();

	int pastPlays_counter = 0;
	int pastPlaysID = 0;
	
	char *place;
	place = malloc(sizeof(char*)*2);

	int k = 0;
	new->turn_Number = 0;
	while (pastPlays_counter <= pastPlays_length) {
		
		// find whose turn
		pastPlaysID = pastPlays_counter % 40;
		int whose_turn = (pastPlaysID + 1) / 8;
		whose_turn = whose_turn % 5;
		new->Curr_Player_Number = whose_turn;
		// set for the counter addition,
		if(pastPlaysID == 39) {
			new->score =  new->score - SCORE_LOSS_DRACULA_TURN;
			new->turn_Number++;
		}


		// now is looking at Godalming
		if(pastPlaysID > 0 && pastPlaysID < 7) {
			// check which city
			if (pastPlaysID > 0 && pastPlaysID < 3) {
				place[k] = pastPlays[pastPlays_counter];
				k++;
				// check the life point of hunter, and recovery if is 0
				Is_need_Recovery(new);
				// here we get the city abb
				if (k == 2) {
					Godalming->currlocation = placeAbbrevToId(place);	
					// move forward trail
					// and put now location into trail
					cycling(Godalming->playerTrail);
					Godalming->playerTrail[5] = Godalming->currlocation;

					Godalming->num_move = moving(Godalming->movelist,
					Godalming->currlocation,Godalming->num_move);

				}
			}

			// check condition and reset k = 0 
			if (pastPlaysID > 2 && pastPlaysID < 7) {
				hunter_condition(pastPlays[pastPlays_counter], new);
				k = 0;
				
			}

		// now is looking at Seward
		} else if(pastPlaysID > 8 && pastPlaysID < 15) {
			// check which city
			if (pastPlaysID > 8 && pastPlaysID < 11) {
				place[k] = pastPlays[pastPlays_counter];
				k++;
				// check the life point of hunter, and recovery if is 0
				Is_need_Recovery(new);
				// here we get the city abb
				if (k == 2) {
					char placenew[2];
					strcpy(placenew, place);
					Seward->currlocation = placeAbbrevToId(placenew);
					// move forward trail
					// and put now location into trail
					cycling(Seward->playerTrail);
					Seward->playerTrail[5] = Seward->currlocation;

					Seward->num_move = moving(Seward->movelist,
					Seward->currlocation,Seward->num_move);
				}
			}
			// check condition and reset k = 0 
			if (pastPlaysID > 10 && pastPlaysID < 15) {
				hunter_condition(pastPlays[pastPlays_counter], new);
				k = 0;
			}
			// now is looking at Helsing
		} else if(pastPlaysID > 16 && pastPlaysID < 23) {
			// check which city
			if (pastPlaysID > 16 && pastPlaysID < 19) {
				place[k] = pastPlays[pastPlays_counter];
				k++;
				// check the life point of hunter, and recovery if is 0
				Is_need_Recovery(new);
				// here we get the city abb
				if (k == 2 ) {
					Helsing->currlocation = placeAbbrevToId(place);
					// move forward trail
					// and put now location into trail
					cycling(Helsing->playerTrail);
					Helsing->playerTrail[5] = placeAbbrevToId(place);
					Helsing->num_move = moving(Helsing->movelist,
					Helsing->currlocation,Helsing->num_move);

				}
			}
			// check condition and reset k = 0
			if (pastPlaysID > 18 && pastPlaysID < 23) {
				hunter_condition(pastPlays[pastPlays_counter], new);
				k = 0;
			}
		}
		// now is looking at Mina
		else if (pastPlaysID > 24 && pastPlaysID < 31) {
			// check which city
			if (pastPlaysID > 24 && pastPlaysID < 27) {
				place[k] = pastPlays[pastPlays_counter];
				k++;
				// check the life point of hunter, and recovery if is 0
				Is_need_Recovery(new);
				// here we get the city abb
				if (k == 2) {
					Mina->currlocation = placeAbbrevToId(place);
					// move forward trail
					// and put now location into trail
					cycling(Mina->playerTrail);
					Mina->playerTrail[5] = placeAbbrevToId(place);
					Mina->num_move = moving(Mina->movelist,
					Mina->currlocation,Mina->num_move);

				}
			}
			// check condition and reset k = 0
			if (pastPlaysID > 26 && pastPlaysID < 31) {
				hunter_condition(pastPlays[pastPlays_counter], new);
				k = 0;
			}
		} else {
			// now is looking at Dracula
			// check which city
			if (pastPlaysID > 32 && pastPlaysID < 35) {
				place[k] = pastPlays[pastPlays_counter];
				k++;
				// here we get the city abb and encounter condition
				if (k == 2) {
					// nomal city
					if (placeAbbrevToId(place) < HIDE) {
						Dracula->currlocation = placeAbbrevToId(place);
					} else if (placeAbbrevToId(place) == TELEPORT){
						// TP
						Dracula->currlocation = CASTLE_DRACULA;
					} else {
						// double back 1 && Hide
						if(placeAbbrevToId(place) == HIDE) {
							Dracula->currlocation = Dracula->playerTrail[5];
						} else{
							Dracula->currlocation = 
							Dracula->playerTrail[5 + DOUBLE_BACK_1 
							- placeAbbrevToId(place)];
						}
					}
					
					// check wheather in sea
					if (placeIsSea(Dracula->currlocation)) {
						Dracula->HP -= LIFE_LOSS_SEA;
						
					}
					// if in CASTLE add HP for dracula
					if (Dracula->currlocation == CASTLE_DRACULA) {
						Dracula->HP += LIFE_GAIN_CASTLE_DRACULA;
					}
					
					// roll the trail
					cycling(Dracula->playerTrail);
					Dracula->playerTrail[5] = Dracula->currlocation;
					Dracula->num_move = moving(Dracula->movelist,
					placeAbbrevToId(place),Dracula->num_move);

					
					k = 0;
				}
			}
			// roll the traplist before check if place trap
			if(pastPlaysID == 34) {
				cycling(new->traplist);
			}
			// place trap
			if (pastPlaysID == 35 && pastPlays[pastPlays_counter] == 'T'){
				new->traplist[5] = Dracula->currlocation;

			}
			// place vampire
			if (pastPlaysID == 36 && pastPlays[pastPlays_counter] == 'V') {
				
				new->vampire->survive = 1;
				new->vampire->born_round_number = new->turn_Number;
				new->vampire->born_location = Dracula->currlocation;
			}
			// here is destory trap because of old
			// however we write cycling function, so traps distroy by itself
			if (pastPlaysID == 37 && pastPlays[pastPlays_counter] == 'M') {

			}
			// vampire has mature, destory vampire and loss score
			if ((pastPlaysID == 37 && pastPlays[pastPlays_counter] == 'V')
			|| (pastPlaysID == 37 && new->vampire->survive == 1
			&& new->vampire->born_round_number + 6 == new->turn_Number)) {
				new->vampire->survive = 0;
				new->vampire->born_round_number= -1;
				new->vampire->born_location = NOWHERE;
				new->score -= SCORE_LOSS_VAMPIRE_MATURES;
			}
		}

		pastPlays_counter++;

	
	}
	free(place);
	return new;
}

void GvFree(GameView gv)
{
	for (int i = 0; i < 5; i++) {
		free (gv->player[i]);
	}
	
	free(gv->map);
	free(gv->vampire);
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

// gets the current round number
Round GvGetRound(GameView gv)
{
	return (gv->turn_Number);
}

// Gets the current player;
Player GvGetPlayer(GameView gv)
{
	return (gv->Curr_Player_Number);
}

//Gets the current game score;
int GvGetScore(GameView gv)
{
	return (gv->score);
}
//gets the PH VALUE for the PLAYER
int GvGetHealth(GameView gv, Player player) {

	return (gv->player[player]->HP);
}

// gets the player location for the Player
PlaceId GvGetPlayerLocation(GameView gv, Player player) {

	return gv->player[player]->currlocation;
	
}
// gets the player location for the Player if it exist.
PlaceId GvGetVampireLocation(GameView gv) {

	return (gv->vampire->born_location);
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps) {
	//create a dynamic memory, and record the placeid.
	int *trap = malloc(sizeof(PlaceId)*MAXMUM_TRAP);
	for (int i = 0; i < 6; i++) {
		trap[i] = '\0';
	}
	// excluding all the traps which have been founded
	int j = 0;
	for (int i = 0; i < 6; i++) {
		if (gv->traplist[i] != NOWHERE){
			trap[j] = gv->traplist[i];
			j++;
		}
	}
    *numTraps = j;

    return trap;
}

////////////////////////////////////////////////////////////////////////
// Game History
// 
// garb the correct moving history.
PlaceId *GvGetMoveHistory(GameView gv, Player player, 
int *numReturnedMoves, bool *canFree) {
	
	*numReturnedMoves = gv->player[player]->num_move;
	*canFree = false;
	if(*numReturnedMoves == 0) {
		return NULL;
	}
	return gv->player[player]->movelist;

}
// grab the last numMoves times move
// if the total number of moves is less than nummoves
// just return total number of moves numbers array
PlaceId *GvGetLastMoves(GameView gv, Player player, 
int numMoves, int *numReturnedMoves, bool *canFree) {
	*numReturnedMoves = 0;
	// get the total move history first,
	int numMovesTemp = 0; 
	bool canFree_temp = false;
	PlaceId *moves = GvGetMoveHistory(gv, player, &numMovesTemp, &canFree_temp);
	// using array to record the lastmove array
	PlaceId *array = malloc(sizeof(PlaceId)*numMoves);
	// if the total number of moves is less than nummoves
	// just return total number of moves numbers array
	int number = numMoves;
	if(number > numMovesTemp) {
		number = numMovesTemp;
	}
	*numReturnedMoves = number;
	number--;
	numMovesTemp--;
	// the copy the placeid onebyone
	while (number >= 0) {
		array[number] = moves[numMovesTemp];
		numMovesTemp--;
		number--;
	}
	// if there is noting inside the move history
	// just return null.
	if(*numReturnedMoves == 0) {
		array = NULL;
	}
	*canFree = true;
	if (canFree_temp) free(moves);
	return array;
}

// in  this fuction 
// we show the real postion for the DOUBLE BACK AND HIDE
PlaceId *GvGetLocationHistory(GameView gv, Player player, 
int *numReturnedLocs, bool *canFree) {
	*numReturnedLocs = gv->player[player]->num_move;
	*canFree = false;
	// if there is noting inside the move history
	// just return null.
	if(*numReturnedLocs == 0) {
		return NULL;
	}
	// 
	// showing the real postion for the DOUBLE BACK AND HIDE
	PlaceId *array = malloc(sizeof(PlaceId)*gv->player[player]->num_move);
	for (int i = 0; i < gv->player[player]->num_move; i++) {
		// copy for the normal postion 
		if(gv->player[player]->movelist[i] < HIDE){
			array[i] = gv->player[player]->movelist[i];
		}

		// showing the position for double back
		if(gv->player[player]->movelist[i] >= DOUBLE_BACK_1 
		&& gv->player[player]->movelist[i] <= DOUBLE_BACK_5) {
			int temp = i+ HIDE - gv->player[player]->movelist[i];
			array[i] = array[temp];	
		} 

		// showing the position for hide
		if(gv->player[player]->movelist[i] == HIDE){
			array[i] = array[i-1];
		} 
	}
	*canFree = true;
	return array;
}

// in  this fuction 
// we show the real postion for the DOUBLE BACK AND HIDE 
// for the last number of moves
PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs, 
int *numReturnedLocs, bool *canFree) {
	*numReturnedLocs = 0;
	int numlocTemp = 0; 
	bool canFree_temp = true;
	// get the total local history first,
	PlaceId *moves = GvGetLocationHistory(gv, player, &numlocTemp, &canFree_temp);
	
	// using array to record the lastmove array
	PlaceId *array = malloc(sizeof(PlaceId)*numLocs);
	// if the total number of moves is less than nummoves
	// just return total number of moves numbers array
	int number = numLocs;
	if(number > numlocTemp) {
		number = numlocTemp;
	}
	*numReturnedLocs = number;
	number--;
	numlocTemp--;

	// the copy the placeid onebyone
	while (number >= 0) {
		array[number] = moves[numlocTemp];
		numlocTemp--;
		number--;
	}
	// if there is noting inside the local history
	// just return null.
	if(*numReturnedLocs == 0) {
		array = NULL;
	}
	*canFree = true;
	if (canFree_temp) free(moves);
	return array;
}

////////////////////////////////////////////////////////////////////////
// Making a Move
// Making a Move
// bubble sort function to make the input array in ascending order
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
// loop through the whole link list to find the cities connected by rails, then
// put them in the input array
int Firstround(ConnList city_near, int *array, int i) {
       
    while (city_near != NULL) {        
        if (city_near->type == RAIL) {
            array[i] = city_near->p;             
            i++;
        }
        city_near = city_near->next;
    } 
    return i;    
}
// remove the same elements in the input array
int removeDuplicates(int* nums, int numsSize)
{
    if (numsSize == 0) {
        return 0;
    }
    int *a1,*a2;
    a1 = nums; 
    a2 = a1 + 1;
    int j = 1;
    for (int i = 1; i < numsSize; i++) {
        if (*a1 != *a2) {
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
	// use curr_r to get all the adjacent city of "from"
	ConnList adjacent = MapGetConnections(gv->map, from);
	PlaceId array_all[1000];	
	int count = 0;
	ConnList curr_r = MapGetConnections(gv->map, from);
    // loop through the whole link list to get the cities wanted
    while (curr_r != NULL) {
        // if player is dracula, and the place is hospital then it doesn't count
        if (player == PLAYER_DRACULA && curr_r->p == ST_JOSEPH_AND_ST_MARY) {
            curr_r = curr_r->next;
            continue;
        }
        // get all the cities that are reachable by boat or road
        if (curr_r->type == ROAD || curr_r->type == BOAT) {
            array_all[count] = curr_r->p;
            count++;
        }
        curr_r = curr_r->next;
    }
    // if player is a hunter, then get the places that are reachable by rail
    if (player == PLAYER_LORD_GODALMING || player == PLAYER_DR_SEWARD 
	|| player == PLAYER_VAN_HELSING || player == PLAYER_MINA_HARKER) {    
        ConnList curr = adjacent;
        int roadCounter = 0;
        int array_local[1000];                       
        // consider the moves through rail, split to four parts
        if ((player + round) % 4 == 0) {        
        // places reachable if can move one distance by rail
        } else if ((player + round) % 4 == 1) {
            if (player == PLAYER_DRACULA) {                
            } else {
                roadCounter = Firstround(curr, array_local, roadCounter);
            }
        // places reachable if can move two distances by rail
        } else if ((player + round) % 4 == 2) {            
            if (player == PLAYER_DRACULA) {                
            } else {
                int counterArray[1000];                            
                int k = Firstround(curr, counterArray, 0);
                ConnList *Array1 = malloc(k * sizeof(ConnList));            
                int l = k - 1;
                int z = k - 1;
                // add the cities adjacent to the current place to an array
                while (z >= 0) {
                    Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                    z--;
                }
                // get the cities adjacent to the cities in the array we get 
                // above
                while (l >= 0) {
                    roadCounter = Firstround(Array1[l], array_local, roadCounter);
                    l--;
                }
            roadCounter = Firstround(curr, array_local, roadCounter);
            }                     
        // places reachable if can move three distances by rail
        } else if ((player + round) % 4 == 2) {            
        } else { 
            if (player == PLAYER_DRACULA) {
                
            } else {
                // create an array contain cities around the current place
                int superArray[1000];                                
                int o = Firstround(curr, superArray, 0);
                ConnList *listArray1 = malloc((o) * sizeof(ConnList));
                int p = o - 1;
                int y = o - 1;
                // add the cities adjacent to the current place to an array
                while (y >= 0) {
                    listArray1[y] = MapGetConnections(gv->map, superArray[y]);
                    y--;
                }                
                int secondaryArray[1000];                                
                int q = 0;
                while (p >= 0) {
                    q = Firstround(listArray1[p], secondaryArray, q);
                    p--;
                }
                // create an array of linklist to contain the cities adjacent to
                // the cities around the current place
                ConnList *listArray2 = malloc((q) * sizeof(ConnList));
                int counter2 = q - 1;
                int counter3 = q - 1;
                while (counter3 >= 0) {
                    listArray2[counter3] = MapGetConnections(gv->map, secondaryArray[counter3]);
                    counter3--;
                }
                while (counter2 >= 0) {
                    roadCounter = Firstround(listArray2[counter2], array_local, roadCounter);
                    counter2--;
                }
                int counterArray[1000];                            
                int k = Firstround(curr, counterArray, 0);
                ConnList *Array1 = malloc(k * sizeof(ConnList));            
                int l = k - 1;
                int z = k - 1;
                // get all the cities adjacent to the cities in the array
                // we get above
                while (z >= 0) {
                    Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                    z--;
                }
                while (l >= 0) {
                    roadCounter = Firstround(Array1[l], array_local, roadCounter);
                    l--;
                }
                roadCounter = Firstround(curr, array_local, roadCounter);
            }            
        }
        // use bubble sort to sort the array that contains cities reachable by 
        // rail
        bubble_sort (array_local, roadCounter);
        int num_array = removeDuplicates(array_local, roadCounter);
        // add the cities to the array that contains all reachable cities
        int num_counter = num_array - 1;
        while (num_counter >= 0) {
            if (array_local[num_counter] != from) {
                array_all[count] = array_local[num_counter];
                count++;
            }
            num_counter--;
        } 
    }
    // sort and remove the same cities in the array
    bubble_sort (array_all, count);
	count = removeDuplicates (array_all, count);
    PlaceId *numReturn = malloc((count + 1)*sizeof(PlaceId));	
	numReturn[count] = from;
	*numReturnedLocs = count + 1;
	// add the cities after processed to the array wanted
	for (int final = 0; final <= count - 1; final++) {
	    numReturn[final] = array_all[final];
	}
	return numReturn;
}




PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	ConnList adjacent = MapGetConnections(gv->map, from);
	PlaceId array_all[1000];	
	int count = 0;
	// first get the all the cities reachable by road
	if (road) {
	    ConnList curr_r = MapGetConnections(gv->map, from);
	    while (curr_r != NULL) {
	        if (player == PLAYER_DRACULA && curr_r->p == ST_JOSEPH_AND_ST_MARY) {
	            curr_r = curr_r->next;
	            continue;
	        }
	        if (curr_r->type == ROAD) {
	            array_all[count] = curr_r->p;
	            count++;
	        }
	        curr_r = curr_r->next;
	    }
	 }
	// get the all the cities reachable by rail
	if (rail) {
        ConnList curr = adjacent;
        int roadCounter = 0;
        int array_local[1000];                       
        // consider the moves through rail, split to four parts
        if ((player + round) % 4 == 0) {                    
        // places reachable if can move one distance by rail
        } else if ((player + round) % 4 == 1) {
            if (player == PLAYER_DRACULA) {                
            } else {
                roadCounter = Firstround(curr, array_local, roadCounter);
            }
        } else if ((player + round) % 4 == 2) {            
            if (player == PLAYER_DRACULA) {                
            } else {
                int counterArray[1000];                            
                int k = Firstround(curr, counterArray, 0);
                ConnList *Array1 = malloc(k * sizeof(ConnList));            
                int l = k - 1;
                int z = k - 1;
                // add the cities adjacent to the current place to an array
                while (z >= 0) {
                    Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                    z--;
                }
                // get the cities adjacent to the cities in the array we get 
                // above
                while (l >= 0) {
                    roadCounter = Firstround(Array1[l], array_local, roadCounter);
                    l--;
                }
            roadCounter = Firstround(curr, array_local, roadCounter);
            }                     
        } else { 
            if (player == PLAYER_DRACULA) {
                
            } else {
                int superArray[1000];                                
                int o = Firstround(curr, superArray, 0);
                ConnList *listArray1 = malloc((o) * sizeof(ConnList));
                int p = o - 1;
                int y = o - 1;
                // add the cities adjacent to the current place to an array
                while (y >= 0) {
                    listArray1[y] = MapGetConnections(gv->map, superArray[y]);
                    y--;
                }                
                int secondaryArray[1000];                                
                int q = 0;
                // create an array of linklist to contain the cities adjacent to
                // the cities around the current place
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
                    roadCounter = Firstround(listArray2[counter2], array_local, roadCounter);
                    counter2--;
                }
                int counterArray[1000];                            
                int k = Firstround(curr, counterArray, 0);
                ConnList *Array1 = malloc(k * sizeof(ConnList));            
                int l = k - 1;
                int z = k - 1;
                // get all the cities adjacent to the cities in the array
                // we get above
                while (z >= 0) {
                    Array1[z] = MapGetConnections(gv->map, counterArray[z]);
                    z--;
                }
                while (l >= 0) {
                    roadCounter = Firstround(Array1[l], array_local, roadCounter);
                    l--;
                }
                roadCounter = Firstround(curr, array_local, roadCounter);
            }            
        }
        // sort and remove the same cities in the array
        bubble_sort (array_local, roadCounter);
        int num_array = removeDuplicates(array_local, roadCounter);
        // add the cities to the array that contains all reachable cities
        int num_counter = num_array - 1;
        while (num_counter >= 0) {
            if (array_local[num_counter] != from) {
                array_all[count] = array_local[num_counter];
                count++;
            }
            num_counter--;
        }
    }        
	// get the all the cities reachable by boat
	if (boat) {	    
	    ConnList curr_boat = MapGetConnections(gv->map, from);        
        while (curr_boat != NULL) {
            if (curr_boat->type == BOAT) {
                array_all[count] = curr_boat->p;
                count++;
            }
            curr_boat = curr_boat->next;
        }
	}
	// sort and remove the same cities in the array
	bubble_sort (array_all, count);
	count = removeDuplicates (array_all, count);
	PlaceId *numReturn = malloc((count + 1)*sizeof(PlaceId));	
	numReturn[count] = from;
	*numReturnedLocs = count + 1;
	// add the cities after processed to the array wanted
	for (int final = 0; final <= count - 1; final++) {
	    numReturn[final] = array_all[final];
	}
	
	
	return numReturn;
}
////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
