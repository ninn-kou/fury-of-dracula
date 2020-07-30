////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testDraculaView.c: test the DraculaView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-02	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DraculaView.h"
#include "Game.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for basic functions, "
			   "just before Dracula's first move\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "..."
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 0);
		assert(DvGetScore(dv) == GAME_START_SCORE);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(DvGetPlayerLocation(dv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(DvGetPlayerLocation(dv, PLAYER_VAN_HELSING) == ZURICH);
		assert(DvGetPlayerLocation(dv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == NOWHERE);
		assert(DvGetVampireLocation(dv) == NOWHERE);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 0);
		free(traps);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD.. SAO.... HCD.... MAO....";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahah",
			"Aha!", "", "", ""
		};
		
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 1);
		assert(DvGetScore(dv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(DvGetHealth(dv, PLAYER_LORD_GODALMING) == 5);
		assert(DvGetHealth(dv, PLAYER_DRACULA) == 30);
		assert(DvGetPlayerLocation(dv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(DvGetPlayerLocation(dv, PLAYER_DRACULA) == GENEVA);
		assert(DvGetVampireLocation(dv) == NOWHERE);

		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula leaving minions 1\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DED.V.. "
			"GST.... SST.... HST.... MST.... DMNT... "
			"GST.... SST.... HST.... MST.... DLOT... "
			"GST.... SST.... HST.... MST.... DHIT... "
			"GST.... SST.... HST.... MST....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);

		assert(DvGetRound(dv) == 4);
		assert(DvGetVampireLocation(dv) == EDINBURGH);
		int numTraps = -1;
		PlaceId *traps = DvGetTrapLocations(dv, &numTraps);
		assert(numTraps == 3);
		sortPlaces(traps, numTraps);
		assert(traps[0] == LONDON);
		assert(traps[1] == LONDON);
		assert(traps[2] == MANCHESTER);
		free(traps);
		
		printf("Test passed!\n");
		DvFree(dv);
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[9] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 4);
		sortPlaces(moves, numMoves);
		assert(moves[0] == GALATZ);
		assert(moves[1] == KLAUSENBURG);
		assert(moves[2] == HIDE);
		assert(moves[3] == DOUBLE_BACK_1);
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 2\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 8);
		sortPlaces(moves, numMoves);
		assert(moves[0] == CLERMONT_FERRAND);
		assert(moves[1] == GENEVA);
		assert(moves[2] == LE_HAVRE);
		assert(moves[3] == NANTES);		
		assert(moves[4] == HIDE);
		assert(moves[5] == DOUBLE_BACK_1);
		assert(moves[6] == DOUBLE_BACK_2);
		assert(moves[7] == DOUBLE_BACK_3);		
		free(moves);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 3\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 5);
		sortPlaces(moves, numMoves);
		assert(moves[0] == CLERMONT_FERRAND);
		assert(moves[1] == GENEVA);
		assert(moves[2] == LE_HAVRE);
		assert(moves[3] == NANTES);		
		assert(moves[4] == HIDE);				
		free(moves);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula's valid moves 4\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DHIT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);
		
		int numMoves = -1;
		PlaceId *moves = DvGetValidMoves(dv, &numMoves);
		assert(numMoves == 4);
		sortPlaces(moves, numMoves);
		assert(moves[0] == CLERMONT_FERRAND);
		assert(moves[1] == GENEVA);
		assert(moves[2] == LE_HAVRE);
		assert(moves[3] == NANTES);							
		free(moves);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		
		assert(numLocs == 7);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == CLERMONT_FERRAND);
		assert(locs[2] == GENEVA);
		assert(locs[3] == LE_HAVRE);
		assert(locs[4] == NANTES);		
		assert(locs[5] == PARIS);
		assert(locs[6] == STRASBOURG);				
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 2\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DKL.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GGE.... SGE.... HGE.... MGE.... DBCT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);
		assert(locs[0] == BELGRADE);
		assert(locs[1] == CONSTANTA);
		assert(locs[2] == GALATZ);
		assert(locs[3] == SOFIA);
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
    {///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 3\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);		
		assert(locs[0] == CLERMONT_FERRAND);
		assert(locs[1] == GENEVA);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);		
		assert(locs[4] == PARIS);						
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGo 4\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DHIT... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGo(dv, &numLocs);
		
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);		
		assert(locs[0] == CLERMONT_FERRAND);
		assert(locs[1] == GENEVA);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);									
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 1\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, true, &numLocs);                             
		
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);		
		assert(locs[0] == CLERMONT_FERRAND);
		assert(locs[1] == GENEVA);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);		
		assert(locs[4] == PARIS);						
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 2\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DGR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DMAT... "
			"GGE.... SGE.... HGE.... MGE.... DSNT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, true, &numLocs);                             
		
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);		
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == LISBON);
		assert(locs[2] == MADRID);
		assert(locs[3] == SANTANDER);		
		assert(locs[4] == SARAGOSSA);						
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 3\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DGR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DMAT... "
			"GGE.... SGE.... HGE.... MGE.... DSNT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, false, &numLocs);                             
		
		assert(numLocs == 4);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == LISBON);
		assert(locs[1] == MADRID);
		assert(locs[2] == SANTANDER);		
		assert(locs[3] == SARAGOSSA);						
		free(locs);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 4\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DGR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DMAT... "
			"GGE.... SGE.... HGE.... MGE.... DSNT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, false, true, &numLocs);
                             
		assert(numLocs == 3);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == BAY_OF_BISCAY);
		assert(locs[1] == MADRID);
		assert(locs[2] == SANTANDER);						
		free(locs);
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 5\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DGR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DMAT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GGE.... SGE.... HGE.... MGE.... DSNT... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, false, true, &numLocs);
                             
		assert(numLocs == 2);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == BAY_OF_BISCAY);		
		assert(locs[1] == SANTANDER);						
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 6\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
		    "GGE.... SGE.... HGE.... MGE.... DD1T... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE.... DLET... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, true, &numLocs);                             										
		assert(numLocs == 3);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == ENGLISH_CHANNEL);
		assert(locs[1] == LE_HAVRE);
		assert(locs[2] == NANTES);									
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 7\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "		    		    
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE.... DLET... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, true, &numLocs);                             										
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);									
		assert(locs[4] == PARIS);
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	{///////////////////////////////////////////////////////////////////
	
		printf("Test for DvWhereCanIGoByType 8\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DFR.V.. "
			"GGE.... SGE.... HGE.... MGE.... DCOT... "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "		    
		    "GGE.... SGE.... HGE.... MGE.... DHIT... "
		    "GGE.... SGE.... HGE.... MGE.... DBUT... "
		    "GGE.... SGE.... HGE.... MGE.... DPAT... "
		    "GGE.... SGE.... HGE.... MGE.... DLET... "
		    "GGE.... SGE.... HGE.... MGE....";
		    
		Message messages[29] = {};
		DraculaView dv = DvNew(trail, messages);		
		int numLocs = -1;
		PlaceId *locs = DvWhereCanIGoByType(dv, true, true, &numLocs);                             										
		assert(numLocs == 5);
		sortPlaces(locs, numLocs);				
		assert(locs[0] == BRUSSELS);
		assert(locs[1] == ENGLISH_CHANNEL);
		assert(locs[2] == LE_HAVRE);
		assert(locs[3] == NANTES);									
		assert(locs[4] == PARIS);
		free(locs);		
		
		printf("Test passed!\n");
		DvFree(dv);
	}
	return EXIT_SUCCESS;
}
