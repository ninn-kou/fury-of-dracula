////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// hunter.c: your "Fury of Dracula" hunter AI.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////
#include "Game.h"
#include "hunter.h"
#include "HunterView.h"
#include "Places.h"
//////////////////////////////////////////////////////////////////////////////
///
///                      MY OWEN FUCTION
///
//////////////////////////////////////////////////////////////////////////////
PlaceId findnextCityToAiming (HunterView hv, PlaceId dest, Player name) {
	Round temp = -1;
	// if we have  arrive the dracula trail loc
	// tring to find the next place;
	if (HvGetPlayerLocation(hv, name) == HvGetLastKnownDraculaLocation(hv, &temp)) {
		//return dest;
		int pathLength = -1;
		PlaceId *path = HvWhereCanIGoByType(hv, true, false, true, &pathLength);

		for(int i = pathLength - 1; i >= 0; i--) {
			if(path[i] != HvGetPlayerLocation(hv, 0)
			&& path[i] != HvGetPlayerLocation(hv, 1)
			&& path[i] != HvGetPlayerLocation(hv, 2)
			&& path[i] != HvGetPlayerLocation(hv, 3)){
				PlaceId next = path[i];
				free(path);
				return next;
			}

		}
		PlaceId next = path[0];
		free(path);
		return next;

	}
	// if we have not find the trail of dracula
	// doing research.
	if (HvGetPlayerLocation(hv, name) == dest) {
		return dest;
	}

	// else, if none above here
	// just find the next city next to dest.
	int pathLength = -1;
	PlaceId *path = HvGetShortestPathTo(hv, name,
										dest, &pathLength);


	PlaceId next = path[0];
	free(path);
	return next;
}


void decideHunterMove(HunterView hv)
{
	// initial all the information we need.
	Round round = HvGetRound(hv);
	Player whose_turn = HvGetPlayer(hv);
	PlaceId me_loc = HvGetPlayerLocation(hv, whose_turn);

	PlaceId decide_loc;
	PlaceId next_loc;
	Round temp;
	PlaceId drac_loc = HvGetLastKnownDraculaLocation(hv, &temp);

	// we
	if(whose_turn == PLAYER_LORD_GODALMING) {
		if(round == 0 && me_loc == NOWHERE) {
			registerBestPlay(placeIdToAbbrev(STRASBOURG), "I am comming.");
			return;
		}
		if (drac_loc == NOWHERE
		&& round < 6) {
			decide_loc = BRUSSELS;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am waitting you.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round >= 6) {

			registerBestPlay(placeIdToAbbrev(me_loc), "I AM LOOKING FOREARD YOU!");
			return;
		}
		if(drac_loc != NOWHERE) {

			decide_loc = drac_loc;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am behind you");
			return;
		}

	}

	if(whose_turn == PLAYER_DR_SEWARD) {
		if(round == 0 && me_loc == NOWHERE) {
			registerBestPlay(placeIdToAbbrev(GRANADA), "I am comming.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round < 6) {
			decide_loc = STRASBOURG;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am waitting you.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round >= 6) {

			registerBestPlay(placeIdToAbbrev(me_loc), "I AM LOOKING FOREARD YOU!");
			return;
		}
		if(drac_loc != NOWHERE) {
			decide_loc = drac_loc;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am behind you");
			return;

		}

	}if(whose_turn == PLAYER_VAN_HELSING) {
		if(round == 0 && me_loc == NOWHERE) {
			registerBestPlay(placeIdToAbbrev(MANCHESTER), "I am comming.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round < 6) {
			decide_loc = FRANKFURT;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am waitting you.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round >= 6) {

			registerBestPlay(placeIdToAbbrev(me_loc), "I AM LOOKING FOREARD YOU!");
			return;
		}
		if(drac_loc != NOWHERE) {
			decide_loc = drac_loc;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am behind you");
			return;

		}


	}if(whose_turn == PLAYER_MINA_HARKER) {
		if(round == 0 && me_loc == NOWHERE) {
			registerBestPlay(placeIdToAbbrev(CONSTANTA), "I am comming.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round < 6) {
			decide_loc = NUREMBURG;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am waitting you.");
			return;

		}
		if (drac_loc == NOWHERE
		&& round >= 6) {

			registerBestPlay(placeIdToAbbrev(me_loc), "I AM LOOKING FOREARD YOU!");
			return;
		}
		if(drac_loc != NOWHERE) {
			decide_loc = drac_loc;
			next_loc = findnextCityToAiming (hv, decide_loc, whose_turn);
			registerBestPlay(placeIdToAbbrev(next_loc), "I am behind you");
			return;

		}
	}




}
