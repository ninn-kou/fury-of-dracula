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

// TODO: ADD YOUR OWN STRUCTS HERE
#define MAXMUM_TRAP 3
#define NOT_FIND -100
typedef struct playerData {
	Player ID;
	int HP;
	PlaceId currlocation;
	PlaceId playerTrail[TRAIL_SIZE];    
} PlayerData;

typedef struct young_vampire{
	// return 1 for true, 0 for died.
	int survive;
	int born_round_number;
	PlaceId born_location;
} *Young_vampire;

struct gameView {
	
	int score;
	Round turn_Number;

	Player Curr_Player_Number;
	PlayerData *player[NUM_PLAYERS];

	int num_traps;
	int *trap_list;

	// point out the info of vampire
	Young_vampire vampire;

	Map map;
};

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
	new->score = GAME_START_SCORE; 		// game start at score 366
	new->turn_Number = 0;          		// game start at turn 0
	new->PLayer_Number = NULL;
	new->players[0]->ID = "G";			// the first player is Lord Godalming
	new->players[0]->HP = GAME_START_HUNTER_LIFE_POINTS;
	new->players[0]->currlocation = NULL;
	new->players[0]->playerTrail = NULL;
	new->players[1]->ID = "S";			// the first player is Dr. Seward
	new->players[1]->HP = GAME_START_HUNTER_LIFE_POINTS;
	new->players[1]->currlocation = NULL;
	new->players[1]->playerTrail = NULL;
	new->players[2]->ID = "H";			// the first player is Van Helsing
	new->players[2]->HP = GAME_START_HUNTER_LIFE_POINTS;
	new->players[2]->currlocation = NULL;
	new->players[2]->playerTrail = NULL;
	new->players[3]->ID = "M";			// the first player is Mina Harker
	new->players[3]->HP = GAME_START_HUNTER_LIFE_POINTS;
	new->players[3]->currlocation = NULL;
	new->players[3]->playerTrail = NULL;
	new->players[4]->ID = "D";			// the first player is Mina Harker
	new->players[4]->HP = GAME_START_BLOOD_POINTS;
	new->players[4]->currlocation = NULL;
	new->players[4]->playerTrail = NULL;
	new->num_traps = 0;
	new->trap_list = NULL;
	new->map = MapNew();
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
	*numTraps = gv->num_traps;
	if (*numTraps == 0) {
		return NULL;
	}
	// return the list of traps
	return (gv->trap_list);
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

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
