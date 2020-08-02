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

// NOTE: Please be mindful that this is just one solution - there are
//       many other (potentially better) ways to implement this

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include "utils.h"

////////////////////////////////////////////////////////////////////////
// Turn struct

#define MAX_TURN_ACTIONS 4

typedef enum action {
	ENCOUNTERED_TRAP,
	ENCOUNTERED_VAMPIRE,
	ENCOUNTERED_DRACULA,
	PLACED_TRAP,
	PLACED_VAMPIRE,
	TRAP_MALFUNCTIONED,
	VAMPIRE_MATURED
} Action;

typedef struct turn {
	Player  player;
	PlaceId move;
	Action  actions[MAX_TURN_ACTIONS];
	int     numActions;
} Turn;

////////////////////////////////////////////////////////////////////////

// I lump everything into the same struct, because I'd rather have all
// the state information in one place

struct gameView {
	// State
	Round    round;                        // current round number
	Player   currentPlayer;                // current player
	int      score;                        // game score
	int      playerHealth[NUM_PLAYERS];    // health of each player
	PlaceId  playerLocations[NUM_PLAYERS]; // location of each player
	PlaceId  trapLocations[TRAIL_SIZE];    // locations of traps
	int      numActiveTraps;               // number of active traps
	PlaceId  vampireLocation;              // location of the immature vampire
	bool     restAttempted;                // if the hunter attempted to rest
	
	// History
	PlaceId *moveHistory[NUM_PLAYERS];     // each player's move history
	PlaceId *draculaLocationHistory;       // Dracula's location history
	
	Map      map;
};

// Helper functions
static int     max(int num1, int num2);
static int     min(int num1, int num2);

////////////////////////////////////////////////////////////////////////
// Constructor

static Turn   *pastPlaysToTurns(char *pastPlays, int *numTurns);
static int     pastPlaysToNumTurns(char *pastPlays);
static Turn    playToTurn(char *play);

static int     numTurnsToNumRounds(int numTurns);
static void    initGameView(GameView gv, int numRounds);
static void    processTurns(GameView gv, Turn *turn, int numTurns);
static void    processTurn(GameView gv, Turn turn);

static void    processDraculaTurn(GameView gv, Turn turn);
static void    processDraculaMove(GameView gv, Turn turn);
static PlaceId resolveDraculaMove(GameView gv, PlaceId move);
static void    processDraculaActions(GameView gv, Turn turn);
static void    processTrapPlaced(GameView gv);
static void    processVampirePlaced(GameView gv);
static void    processMalfunctionedTrap(GameView gv);
static void    processMaturedVampire(GameView gv);
static void    processDraculaEndOfTurn(GameView gv);
static void    processDraculaLifeGain(GameView gv, int amount);

static void    processHunterTurn(GameView gv, Turn turn);
static void    processHunterMove(GameView gv, Turn turn);
static void    processHunterActions(GameView gv, Turn turn);
static void    processTrapEncountered(GameView gv);
static void    processVampireEncountered(GameView gv);
static void    processDraculaEncountered(GameView gv);
static void    processHunterEndOfTurn(GameView gv);
static void    processHunterLifeGain(GameView gv, Player hunter, int amount);

static void    processRemoveTrap(GameView gv, PlaceId location);
static void    processPlayerDamage(GameView gv, Player player, int damage);

GameView GvNew(char *pastPlays, Message messages[])
{
	GameView gv = malloc(sizeof(*gv));
	if (gv == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	
	int numTurns = 0;
	Turn *turns = pastPlaysToTurns(pastPlays, &numTurns);
	
	int numRounds = numTurnsToNumRounds(numTurns);
	initGameView(gv, numRounds);
	processTurns(gv, turns, numTurns);
	free(turns);
	
	gv->map = MapNew();
	return gv;
}

/**
 * Converts a pastPlays string to an array of Turn structs, and sets
 * *numTurns to the number of turns
 */
static Turn *pastPlaysToTurns(char *pastPlays, int *numTurns) {
	*numTurns = pastPlaysToNumTurns(pastPlays);
	Turn *turns = malloc(*numTurns * sizeof(Turn));
	if (turns == NULL) {
		fprintf(stderr, "Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < *numTurns; i++) {
		// Hack to avoid string splitting
		turns[i] = playToTurn(&pastPlays[i * 8]);
	}
	return turns;
}

/**
 * Counts the number of turns/plays in a pastPlays string
 */
static int pastPlaysToNumTurns(char *pastPlays) {
	if (pastPlays[0] == '\0') {
		return 0;
	}
	
	int numTurns = 1;
	int i = 0;
	// Just counts the number of spaces
	while (pastPlays[i] != '\0') {
		if (pastPlays[i] == ' ') {
			numTurns++;
		}
		i++;
	}
	return numTurns;
}

/**
 * Converts a play string to a Turn struct
 */
static Turn playToTurn(char *play) {
	Turn turn = {};
	
	switch (play[0]) {
		case 'G': turn.player = PLAYER_LORD_GODALMING; break;
		case 'S': turn.player = PLAYER_DR_SEWARD;      break;
		case 'H': turn.player = PLAYER_VAN_HELSING;    break;
		case 'M': turn.player = PLAYER_MINA_HARKER;    break;
		case 'D': turn.player = PLAYER_DRACULA;        break;
		default:  assert(0); /* impossible */          break;
	}
	
	turn.move = placeAbbrevToId((char[3]){ play[1], play[2], '\0' });
	
	if (turn.player == PLAYER_DRACULA) {
		if (play[3] == 'T') turn.actions[turn.numActions++] = PLACED_TRAP;
		if (play[4] == 'V') turn.actions[turn.numActions++] = PLACED_VAMPIRE;
		if (play[5] == 'M') turn.actions[turn.numActions++] = TRAP_MALFUNCTIONED;
		if (play[5] == 'V') turn.actions[turn.numActions++] = VAMPIRE_MATURED;
	} else {
		for (int i = 3; i < 7; i++) {
			switch (play[i]) {
				case 'T': turn.actions[turn.numActions++] = ENCOUNTERED_TRAP;    break;
				case 'V': turn.actions[turn.numActions++] = ENCOUNTERED_VAMPIRE; break;
				case 'D': turn.actions[turn.numActions++] = ENCOUNTERED_DRACULA; break;
				case '.':                                                        break;
				default:  assert(0); /* impossible */                            break;
			}
		}
	}
	
	return turn;
}

////////////////////////////////////////////////////////////////////////

/**
 * Given the number of turns in the game, returns the number of rounds
 */
static int numTurnsToNumRounds(int numTurns) {
	return numTurns / NUM_PLAYERS + (numTurns % NUM_PLAYERS > 0 ? 1 : 0);
}

/**
 * Initialises the given GameView struct with the given number of rounds
 */
static void initGameView(GameView gv, int numRounds) {
	// State
	gv->round = 0;
	gv->currentPlayer = PLAYER_LORD_GODALMING;
	gv->score = GAME_START_SCORE;
	
	for (int i = 0; i < NUM_PLAYERS; i++) {
		gv->playerHealth[i] = (i == PLAYER_DRACULA) ?
			GAME_START_BLOOD_POINTS : GAME_START_HUNTER_LIFE_POINTS;
		gv->playerLocations[i] = NOWHERE;
	}
	
	gv->numActiveTraps = 0;
	gv->vampireLocation = NOWHERE;
	gv->restAttempted = false;
	
	// History
	for (int i = 0; i < NUM_PLAYERS; i++) {
		gv->moveHistory[i] = malloc(numRounds * sizeof(PlaceId));
	}
	gv->draculaLocationHistory = malloc(numRounds * sizeof(PlaceId));
}

static void processTurns(GameView gv, Turn *turns, int numTurns) {
	for (int i = 0; i < numTurns; i++) {
		processTurn(gv, turns[i]);
	}
}

/**
 * Processes a turn
 */
static void processTurn(GameView gv, Turn turn) {
	assert(gv->currentPlayer == turn.player);
	
	if (gv->currentPlayer == PLAYER_DRACULA) {
		processDraculaTurn(gv, turn);
		gv->round++; // Advance round after Dracula's turn
	} else {
		processHunterTurn(gv, turn);
	}
	
	// Passes the turn to the next player
	gv->currentPlayer = (gv->currentPlayer + 1) % NUM_PLAYERS;
	
	// Revives the next player
	if (gv->playerHealth[gv->currentPlayer] == 0) {
		gv->playerHealth[gv->currentPlayer] = GAME_START_HUNTER_LIFE_POINTS;
	}
}

///////////////////
// Dracula's turn

/**
 * Processes Dracula's turn
 */
static void processDraculaTurn(GameView gv, Turn turn) {
	processDraculaMove(gv, turn);
	processDraculaActions(gv, turn);
	processDraculaEndOfTurn(gv);
}

static void processDraculaMove(GameView gv, Turn turn) {
	// Store the move in the move history
	gv->moveHistory[PLAYER_DRACULA][gv->round] = turn.move;
	
	// Resolve the move to a location, in case it was a special move
	// and update Dracula's location
	gv->playerLocations[PLAYER_DRACULA] = resolveDraculaMove(gv, turn.move);
	
	// Store the location in the location history
	gv->draculaLocationHistory[gv->round] = gv->playerLocations[PLAYER_DRACULA];
}

static PlaceId resolveDraculaMove(GameView gv, PlaceId move) {
	if (placeIsReal(move) || move == CITY_UNKNOWN || move == SEA_UNKNOWN) {
		return move;
	}
	
	switch (move) {
		case TELEPORT:      return CASTLE_DRACULA;
		case HIDE:          return gv->draculaLocationHistory[gv->round - 1];
		case DOUBLE_BACK_1: return gv->draculaLocationHistory[gv->round - 1];
		case DOUBLE_BACK_2: return gv->draculaLocationHistory[gv->round - 2];
		case DOUBLE_BACK_3: return gv->draculaLocationHistory[gv->round - 3];
		case DOUBLE_BACK_4: return gv->draculaLocationHistory[gv->round - 4];
		case DOUBLE_BACK_5: return gv->draculaLocationHistory[gv->round - 5];
		default:            assert(0); // impossible
	}
}

static void processDraculaActions(GameView gv, Turn turn) {
	for (int i = 0; i < turn.numActions; i++) {
		switch (turn.actions[i]) {
			case PLACED_TRAP:        processTrapPlaced(gv);        break;
			case PLACED_VAMPIRE:     processVampirePlaced(gv);     break;
			case TRAP_MALFUNCTIONED: processMalfunctionedTrap(gv); break;
			case VAMPIRE_MATURED:    processMaturedVampire(gv);    break;
			default:                                               break;
		}
	}
}

/**
 * Dracula placed a trap
 */
static void processTrapPlaced(GameView gv) {
	gv->trapLocations[gv->numActiveTraps++] = gv->playerLocations[PLAYER_DRACULA];
}

/**
 * Dracula placed a vampire
 */
static void processVampirePlaced(GameView gv) {
	gv->vampireLocation = gv->playerLocations[PLAYER_DRACULA];
}

/**
 * A trap (from 6 rounds ago) malfunctioned
 */
static void processMalfunctionedTrap(GameView gv) {
	processRemoveTrap(gv, gv->draculaLocationHistory[gv->round - TRAIL_SIZE]);
}

/**
 * A vampire matured
 */
static void processMaturedVampire(GameView gv) {
	gv->vampireLocation = NOWHERE;
	gv->score -= SCORE_LOSS_VAMPIRE_MATURES;
}

static void processDraculaEndOfTurn(GameView gv) {
	if (placeIsSea(gv->playerLocations[PLAYER_DRACULA])) {
		processPlayerDamage(gv, PLAYER_DRACULA, LIFE_LOSS_SEA);
	}
	if (gv->playerLocations[PLAYER_DRACULA] == CASTLE_DRACULA) {
		processDraculaLifeGain(gv, LIFE_GAIN_CASTLE_DRACULA);
	}
	gv->score -= SCORE_LOSS_DRACULA_TURN;
}

static void processDraculaLifeGain(GameView gv, int amount) {
	gv->playerHealth[PLAYER_DRACULA] += amount;
}

///////////////////
// Hunters' turns

/**
 * Processes a hunter's turn
 */
static void processHunterTurn(GameView gv, Turn turn) {
	processHunterMove(gv, turn);
	processHunterActions(gv, turn);
	processHunterEndOfTurn(gv);
}

/**
 * Processes a hunter's move
 */
static void processHunterMove(GameView gv, Turn turn) {
	// Store the move in the move history
	gv->moveHistory[gv->currentPlayer][gv->round] = turn.move;
	
	// Check if the hunter is attempting to rest
	gv->restAttempted = (turn.move == gv->playerLocations[gv->currentPlayer]);
	
	// Update the hunter's location
	gv->playerLocations[gv->currentPlayer] = turn.move;
}

/**
 * Processes a hunter's actions
 */
static void processHunterActions(GameView gv, Turn turn) {
	for (int i = 0; i < turn.numActions; i++) {
		switch (turn.actions[i]) {
			case ENCOUNTERED_TRAP:    processTrapEncountered(gv);    break;
			case ENCOUNTERED_VAMPIRE: processVampireEncountered(gv); break;
			case ENCOUNTERED_DRACULA: processDraculaEncountered(gv); break;
			default:                                                 break;
		}
	}
	
	// If the hunter died :(
	if (gv->playerHealth[gv->currentPlayer] == 0) {
		gv->playerLocations[gv->currentPlayer] = ST_JOSEPH_AND_ST_MARY;
		gv->score -= SCORE_LOSS_HUNTER_HOSPITAL;
	}
}

/**
 * The current player encounters a trap
 */
static void processTrapEncountered(GameView gv) {
	processPlayerDamage(gv, gv->currentPlayer, LIFE_LOSS_TRAP_ENCOUNTER);
	processRemoveTrap(gv, gv->playerLocations[gv->currentPlayer]);
}

/**
 * The current player encounters an immature vampire
 */
static void processVampireEncountered(GameView gv) {
	gv->vampireLocation = NOWHERE;
}

/**
 * The current player encounters Dracula
 */
static void processDraculaEncountered(GameView gv) {
	processPlayerDamage(gv, gv->currentPlayer, LIFE_LOSS_DRACULA_ENCOUNTER);
	processPlayerDamage(gv, PLAYER_DRACULA, LIFE_LOSS_HUNTER_ENCOUNTER);
}

static void processHunterEndOfTurn(GameView gv) {
	// If the player attempted to rest and is not dead, heal them
	if (gv->playerHealth[gv->currentPlayer] > 0 && gv->restAttempted) {
		processHunterLifeGain(gv, gv->currentPlayer, LIFE_GAIN_REST);
	}
}

/**
 * The given hunter gains some life points
 */
static void processHunterLifeGain(GameView gv, Player hunter, int amount) {
	gv->playerHealth[hunter] = min(gv->playerHealth[hunter] + amount,
	                               GAME_START_HUNTER_LIFE_POINTS);
}

/////////////////////
// Everyone's turns

/**
 * Removes a trap from the given location
 */
static void processRemoveTrap(GameView gv, PlaceId location) {
	placesDelete(gv->trapLocations, gv->numActiveTraps, location);
	gv->numActiveTraps--;
}

/**
 * Deals a given amount of damage to the given player
 */
static void processPlayerDamage(GameView gv, Player player, int amount) {
	gv->playerHealth[player] = max(0, gv->playerHealth[player] - amount);
}

////////////////////////////////////////////////////////////////////////

void GvFree(GameView gv)
{
	for (int i = 0; i < NUM_PLAYERS; i++) {
		free(gv->moveHistory[i]);
	}
	free(gv->draculaLocationHistory);
	MapFree(gv->map);
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	return gv->round;
}

Player GvGetPlayer(GameView gv)
{
	return gv->currentPlayer;
}

int GvGetScore(GameView gv)
{
	return gv->score;
}

int GvGetHealth(GameView gv, Player player)
{
	return gv->playerHealth[player];
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
	return gv->playerLocations[player];
}

PlaceId GvGetVampireLocation(GameView gv)
{
	return gv->vampireLocation;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// This just copies gv->trapLocations
	*numTraps = gv->numActiveTraps;
	PlaceId *trapLocations = malloc(*numTraps * sizeof(PlaceId));
	placesCopy(trapLocations, gv->trapLocations, *numTraps);
	return trapLocations;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// 1 move for each previous round
	// plus 1 move if they have made a move this round
	*numReturnedMoves = gv->round + (player < gv->currentPlayer ? 1 : 0);
	*canFree = false;
	return gv->moveHistory[player];
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// Get the number of moves the player has made
	int numMovesMade =  gv->round + (player < gv->currentPlayer ? 1 : 0);
	
	// If the number of moves requested is more than the number of
	// moves the player has made, return only that many moves
	*numReturnedMoves = min(numMoves, numMovesMade);
	
	// Return a pointer to the section of the array containing the
	// player's last *numReturnedMoves moves
	*canFree = false;
	return &gv->moveHistory[player][numMovesMade - *numReturnedMoves];
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	if (player != PLAYER_DRACULA) {
		return GvGetMoveHistory(gv, player, numReturnedLocs, canFree);
		
	} else {
		// 1 location for each previous round
		*numReturnedLocs = gv->round;
		*canFree = false;
		return gv->draculaLocationHistory;
	}
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	if (player != PLAYER_DRACULA) {
		// If the given player is a hunter, simply call GvGetLastMoves
		// since all hunter moves are real locations
		return GvGetLastMoves(gv, player, numLocs, numReturnedLocs, canFree);
		
	} else {
		// Get the number of moves Dracula has made
		int numMovesMade =  gv->round;
		
		// If the number of moves requested is more than the number of
		// moves Dracula has made, return only that many locations
		*numReturnedLocs = min(numLocs, numMovesMade);
		
		// Return a pointer to the section of the array containing the
		// player's last *numReturnedLocs locations
		*canFree = false;
		return &gv->moveHistory[player][numMovesMade - *numReturnedLocs];
	}
}

////////////////////////////////////////////////////////////////////////
// Making a Move

static bool isForbiddenMove(Player player, PlaceId place);
static void addRoadConnections(Map map, Player player, PlaceId from,
                               bool *locations);
static void addRailConnections(Map map, Player player, PlaceId from,
                               bool *locations, int maxHops);
static void addBoatConnections(Map map, Player player, PlaceId from,
                               bool *locations);

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	return GvGetReachableByType(gv, player, round, from, true, true,
	                            true, numReturnedLocs);
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	bool locations[NUM_REAL_PLACES] = {};
	locations[from] = true;
	
	if (road) {
		addRoadConnections(gv->map, player, from, locations);
	}
	if (rail) {
		int maxHops = (player + round) % 4;
		addRailConnections(gv->map, player, from, locations, maxHops);
	}
	if (boat) {
		addBoatConnections(gv->map, player, from, locations);
	}
	
	return boolsToPlaces(locations, numReturnedLocs);
}

static bool isForbiddenMove(Player player, PlaceId place) {
	return (player == PLAYER_DRACULA && place == ST_JOSEPH_AND_ST_MARY);
}

static void addRoadConnections(Map map, Player player, PlaceId from,
                               bool *locations) {
	ConnList curr = MapGetConnections(map, from);
	for (; curr != NULL; curr = curr->next) {
		if (curr->type == ROAD) {
			if (!isForbiddenMove(player, curr->p)) {
				locations[curr->p] = true;
			}
		}
	}
}

static void addRailConnections(Map map, Player player, PlaceId from,
                               bool *locations, int maxHops) {
    locations[from] = true;
    if (maxHops <= 0 || player == PLAYER_DRACULA) {
        return;
    }
    
    ConnList curr = MapGetConnections(map, from);
    for (; curr != NULL; curr = curr->next) {
        if (curr->type == RAIL) {
            if (!isForbiddenMove(player, from)) {
	            addRailConnections(map, player, curr->p, locations,
	                               maxHops - 1);
            }
        }
    }
}

static void addBoatConnections(Map map, Player player, PlaceId from,
                               bool *locations) {
    ConnList curr = MapGetConnections(map, from);
    for (; curr != NULL; curr = curr->next) {
        if (curr->type == BOAT) {
			if (!isForbiddenMove(player, from)) {
				locations[curr->p] = true;
			}
		}
    }
}

////////////////////////////////////////////////////////////////////////
// Helper functions

static int max(int num1, int num2) {
	return (num1 > num2 ? num1 : num2);
}

static int min(int num1, int num2) {
	return (num1 < num2 ? num1 : num2);
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

void test(int a, int b) {
	printf("%d %d\n", a, b);
}

// TODO
