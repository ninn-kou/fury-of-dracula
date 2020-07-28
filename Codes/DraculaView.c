////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
// add your own #includes here

// TODO: ADD YOUR OWN STRUCTS HERE

struct draculaView {
	// TODO: ADD FIELDS HERE
	GameView gv;
    Map map;
};
//---------------------------------------------------------------------------
//
// GameView function call
//
//---------------------------------------------------------------------------

#define MAXIMUM_CITY 128	

//Count the number of lists
extern const char* GvPlayerName(Player id);


////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
	DraculaView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate DraculaView\n");
		exit(EXIT_FAILURE);
	}
    new->map = MapNew();
	new->gv = GvNew(pastPlays, messages);

	return new;
}

void DvFree(DraculaView dv)
{
	GvFree(dv->gv);
    MapFree(dv->map);
	free(dv);
}
////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return GvGetRound(dv->gv);
}

int DvGetScore(DraculaView dv)
{
	return GvGetScore(dv->gv);
}

int DvGetHealth(DraculaView dv, Player player)
{
	return GvGetHealth(dv->gv, player);
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
	return GvGetPlayerLocation(dv->gv, player);
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	return GvGetVampireLocation(dv->gv);
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	return GvGetTrapLocations(dv->gv, numTraps);
}

////////////////////////////////////////////////////////////////////////
// Making a Move
void del_element(int *arr,int n, int delete) //n????????
{
    for(int i=0;i<=n-1;i++)
    {
        if(delete==arr[i])
        {
            for(int j=i;j<=n-2;j++)
            {
                arr[j]=arr[j+1];//??
            }
        }
    }
}

int judger(DraculaView dv, int numMoves, PlaceId *moves) {
    bool back = false;
    bool hide = false;
    if (numMoves < 5) {
        int counter_back = 0;                        
        while (counter_back < numMoves) {
            if (moves[counter_back] == DOUBLE_BACK_1 || moves[counter_back] == DOUBLE_BACK_2 || moves[counter_back] == DOUBLE_BACK_3 || moves[counter_back] == DOUBLE_BACK_4 || moves[counter_back] == DOUBLE_BACK_5) {
	            back = true;	            
	        }
	        counter_back++;
	    }
	    int counter_hide = 0;
	    while (counter_hide < numMoves) {
            if (moves[counter_hide] == HIDE) {
	            hide = true;	           
	        }
	        counter_hide++;
	    }
    } else {
        int counter_five = numMoves - 5;
        int counter = numMoves - 1;
        while (counter >= counter_five) {          
            if (moves[counter] == DOUBLE_BACK_1 || moves[counter] == DOUBLE_BACK_2 || moves[counter] == DOUBLE_BACK_3 || moves[counter] == DOUBLE_BACK_4 || moves[counter] == DOUBLE_BACK_5) {                
                back = true;               
            }
            counter--;
        }
        int counter2 = numMoves;
        while (counter2 >= counter_five) {
            if (moves[counter2] == HIDE) {
                hide = true;               
            }
            counter2--;
        }
    }
    if (back == true && hide == true) {
        return 3;
    } else if (hide == true) {
        return 2;
    } else if (back == true) {
        return 1;
    } else { 
        return 0;
    }
}
PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{	
	int array[1000];	
	int count = 0;
	if (GvGetRound(dv->gv) == 0) {	    
	} else if (GvGetRound(dv->gv) < 5 && GvGetRound(dv->gv) > 0) {	    
	    int accept = 0;
	    PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	    PlaceId *array1 = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), from, &accept);
        int i = 0;
        int temp = accept - 1;
        while (i <= temp) {
            array[count] = array1[i];
            i++;
            count++;
        }
        int numMoves = 0; bool canFree = false;
        PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree);
        int judge = judger(dv, numMoves, moves);	    
	    if (judge == 1) {
	        array[count] = HIDE;
	        count = count + 1;
	    }
	    if (judge == 2) {	               	        
	        ConnList curr = MapGetConnections(dv->map, from);
	        int adjacent[1000];
	        int adjacent_c = 0;
	        int q = numMoves - 1;
	        while (curr != NULL) {
	            q = numMoves - 1;
	            while (q >= 0) {
	                if (curr->p == moves[q]) {
	                    adjacent[adjacent_c] = numMoves - q;
	                    adjacent_c++;
	                }
	                q--;
	            }
	            curr = curr->next;
	        }
	        adjacent[adjacent_c] = 1;
	        adjacent_c++;
	        bubble_sort(adjacent, adjacent_c);
	        adjacent_c = removeDuplicates(adjacent, adjacent_c);
	        int final = adjacent_c - 1;
	        while (final >= 0) {
	            array[count] = DOUBLE_BACK_1 + adjacent[final] - 1;
	            final--;
	            count++;
	        }
	    }
	    if (judge == 0) {
	        array[count] = HIDE;	        
	        count = count + 1;	        	        
	        ConnList curr_0 = MapGetConnections(dv->map, from);
	        int adjacent_0[1000];
	        int adjacent_c_0 = 0;
	        int w = numMoves - 1;
	        while (curr_0 != NULL) {
	            w = numMoves - 1;
	            while (w >= 0) {
	                if (curr_0->p == moves[w]) {
	                    adjacent_0[adjacent_c_0] = numMoves - w;
	                    adjacent_c_0++;
	                }
	                w--;
	            }
	            curr_0 = curr_0->next;
	        }
	        adjacent_0[adjacent_c_0] = 1;
	        adjacent_c_0++;
	        bubble_sort(adjacent_0, adjacent_c_0);
	        adjacent_c_0 = removeDuplicates(adjacent_0, adjacent_c_0);
	        int final0 = adjacent_c_0 - 1;
	        while (final0 >= 0) {
	            array[count] = DOUBLE_BACK_1 + adjacent_0[final0] - 1;
	            final0--;
	            count++;
	        }
	    }	       
	} else if (GvGetRound(dv->gv) >= 5) {	    
	    int accept = 0;
	    PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);	    
	    PlaceId *array1 = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), from, &accept);        
        int i = 0;
        int temp = accept - 1;
        while (i <= temp) {
            array[count] = array1[i];
            i++;
            count++;
        }
        int numMoves = 0; bool canFree = false;
        PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree);       
        int judge = judger(dv, numMoves, moves);	    	    
	    if (judge == 2) {	        
	        ConnList curr_2 = MapGetConnections(dv->map, from);
	        int adjacent_2[1000];
	        int adjacent_c_2 = 0;
	        int r = numMoves - 1;
	        int z = numMoves - 5;
	        while (curr_2 != NULL) {
	            z = numMoves - 5;
	            while (z <= r) {
	                if (curr_2->p == moves[z]) {
	                    adjacent_2[adjacent_c_2] = numMoves - z;
	                    adjacent_c_2++;
	                }
	                z++;
	            }
	            curr_2 = curr_2->next;
	        }
	        adjacent_2[adjacent_c_2] = 1;
	        adjacent_c_2++;
	        bubble_sort(adjacent_2, adjacent_c_2);
	        adjacent_c_2 = removeDuplicates(adjacent_2, adjacent_c_2);
	        int final3 = adjacent_c_2 - 1;
	        while (final3 >= 0) {
	            array[count] = DOUBLE_BACK_1 + adjacent_2[final3] - 1;
	            final3--;
	            count++;
	        }
	    } else if (judge == 1) {	        
	        array[count] = HIDE;
	        count = count + 1;
	    } else if (judge == 0) {	        
	        array[count] = HIDE;	        
	        count = count + 1;	        	        
	        ConnList curr_1 = MapGetConnections(dv->map, from);	        
	        int adjacent_1[1000];
	        int adjacent_c_1 = 0;
	        int w = numMoves - 1;	        
	        while (curr_1 != NULL) {
	            w = numMoves - 1;
	            while (w >= 0) {	                
	                if (curr_1->p == moves[w]) {	                    
	                    adjacent_1[adjacent_c_1] = numMoves - w;
	                    adjacent_c_1++;
	                }
	                w--;
	            }
	            curr_1 = curr_1->next;
	        }	        
	        adjacent_1[adjacent_c_1] = 1;
	        adjacent_c_1++;
	        bubble_sort (adjacent_1, adjacent_c_1);
	        adjacent_c_1 = removeDuplicates(adjacent_1, adjacent_c_1);	       	        
	        int final1 = adjacent_c_1 - 1;
	        while (final1 >= 0) {
	            array[count] = DOUBLE_BACK_1 + adjacent_1[final1] - 1;
	            final1--;
	            count++;
	        }
	    }		
	}	 
	if (GvGetRound(dv->gv) < 5 && GvGetRound(dv->gv) > 0) {
	    int numMoves_judge = 0; bool canFree = false;
        PlaceId *moves_judge = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge, &canFree);
	    int p = 0;
	    int count_judge = count - 1;
	    while (p < numMoves_judge) {
	        count_judge = count - 1;
	        while (count_judge >= 0) {
	            if (moves_judge[p] == array[count_judge]) {
	                del_element(array, count, moves_judge[p]);
	                count--;	                
	            }
	        count_judge--;
	        }
	    p++;
	    }
	}
	if (GvGetRound(dv->gv) >= 5) {
	    int numMoves_judge1 = 0; bool canFree = false;
        PlaceId *moves_judge1 = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge1, &canFree);
	    int a = numMoves_judge1 - 1;
	    int b = numMoves_judge1 - 5;
	    int count_judge = count - 1;
	    while (a >= b) {
	        count_judge = count - 1;
	        while (count_judge >= 0) {
	            if (moves_judge1[a] == array[count_judge]) {
	                del_element(array, count, moves_judge1[a]);
	                count--;
	               
	            }
	        count_judge--;
	        }
	    a--;
	    }
	}	  		
	PlaceId *array_final = malloc(1000 * sizeof(PlaceId));
	*numReturnedMoves = count;
	int final_count = count - 1;
	while (final_count >= 0) {
	    array_final[final_count] = array[final_count];
	    final_count--;
	}	
	if (*numReturnedMoves == 0) {
	    return NULL;
	} else {
	    return array_final;
    }    
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	
	int array[1000];	
	int count = 0;
	if (GvGetRound(dv->gv) == 0) {	    
	} else if (GvGetRound(dv->gv) < 5 && GvGetRound(dv->gv) > 0) {	    
	    int accept = 0;
	    PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);
	    PlaceId *array1 = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), from, &accept);
        int i = 0;
        int temp = accept - 1;
        while (i <= temp) {
            array[count] = array1[i];
            i++;
            count++;
        }
        int numMoves_judge = 0; bool canFree = false;
        PlaceId *moves_judge = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge, &canFree);
	    int p = 0;
	    int count_judge = count - 1;
	    while (p < numMoves_judge) {
	        count_judge = count - 1;
	        while (count_judge >= 0) {
	            if (moves_judge[p] == array[count_judge]) {
	                del_element(array, count, moves_judge[p]);
	                count--;	                
	            }
	        count_judge--;
	        }
	    p++;
	    }
        int numMoves = 0; bool canFree1 = false;
        PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree1);       
        int judge = judger(dv, numMoves, moves);	    
	    if (judge == 1) {
	        array[count] = from;
	        count = count + 1;
	    }
	    if (judge == 2) {	               	        
	        ConnList curr = MapGetConnections(dv->map, from);
	        int adjacent[1000];
	        int adjacent_c = 0;
	        int q = numMoves - 1;
	        while (curr != NULL) {
	            q = numMoves - 1;
	            while (q >= 0) {
	                if (curr->p == moves[q]) {
	                    adjacent[adjacent_c] = numMoves - q;
	                    adjacent_c++;
	                }
	                q--;
	            }
	            curr = curr->next;
	        }
	        adjacent[adjacent_c] = 1;
	        adjacent_c++;
	        bubble_sort(adjacent, adjacent_c);
	        adjacent_c = removeDuplicates(adjacent, adjacent_c);
	        int final = adjacent_c - 1;
	        while (final >= 0) {
	            array[count] = moves[numMoves - adjacent[final]];
	            final--;
	            count++;
	        }
	    }
	    if (judge == 0) {	               	        
	        ConnList curr_0 = MapGetConnections(dv->map, from);
	        int adjacent_0[1000];
	        int adjacent_c_0 = 0;
	        int w = numMoves - 1;
	        while (curr_0 != NULL) {
	            w = numMoves - 1;
	            while (w >= 0) {
	                if (curr_0->p == moves[w]) {
	                    adjacent_0[adjacent_c_0] = numMoves - w;
	                    adjacent_c_0++;
	                }
	                w--;
	            }
	            curr_0 = curr_0->next;
	        }
	        adjacent_0[adjacent_c_0] = 1;
	        adjacent_c_0++;
	        bubble_sort(adjacent_0, adjacent_c_0);
	        adjacent_c_0 = removeDuplicates(adjacent_0, adjacent_c_0);
	        int final0 = adjacent_c_0 - 1;
	        while (final0 >= 0) {
	            array[count] = moves[numMoves - adjacent_0[final0]];
	            final0--;
	            count++;
	        }
	    }	       
	} else if (GvGetRound(dv->gv) >= 5) {	    
	    int accept = 0;
	    PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);	    
	    PlaceId *array1 = GvGetReachable(dv->gv, PLAYER_DRACULA, GvGetRound(dv->gv), from, &accept);        
        int i = 0;
        int temp = accept - 1;
        while (i <= temp) {
            array[count] = array1[i];
            i++;
            count++;
        }
        int numMoves_judge1 = 0; bool canFree = false;
        PlaceId *moves_judge1 = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge1, &canFree);
	    int a = numMoves_judge1 - 1;
	    int b = numMoves_judge1 - 5;
	    int count_judge = count - 1;
	    while (a >= b) {
	        count_judge = count - 1;
	        while (count_judge >= 0) {
	            if (moves_judge1[a] == array[count_judge]) {
	                del_element(array, count, moves_judge1[a]);
	                count--;
	               
	            }
	        count_judge--;
	        }
	        a--;
	    }
	    
        int numMoves = 0; bool canFree2 = false;
        PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree2);       
        int judge = judger(dv, numMoves, moves);	    	    
	    if (judge == 2) {	        
	        ConnList curr = MapGetConnections(dv->map, from);
	        int adjacent[1000];
	        int adjacent_c = 0;
	        int q = numMoves - 1;
	        while (curr != NULL) {
	            q = numMoves - 1;
	            while (q >= 0) {
	                if (curr->p == moves[q]) {
	                    adjacent[adjacent_c] = numMoves - q;
	                    adjacent_c++;
	                }
	                q--;
	            }
	            curr = curr->next;
	        }
	        adjacent[adjacent_c] = 1;
	        adjacent_c++;
	        bubble_sort(adjacent, adjacent_c);
	        adjacent_c = removeDuplicates(adjacent, adjacent_c);
	        int final = adjacent_c - 1;
	        while (final >= 0) {
	            array[count] = moves[numMoves - adjacent[final]];
	            final--;
	            count++;
	        }
	    } else if (judge == 1) {	        
	        array[count] = from;
	        count = count + 1;
	    } else if (judge == 0) {	        
	        	        	        
	        ConnList curr_1 = MapGetConnections(dv->map, from);	        
	        int adjacent_1[1000];
	        int adjacent_c_1 = 0;
	        int w = numMoves - 1;	        
	        while (curr_1 != NULL) {
	            w = numMoves - 1;
	            while (w >= 0) {	                
	                if (curr_1->p == moves[w]) {	                    
	                    adjacent_1[adjacent_c_1] = numMoves - w;
	                    adjacent_c_1++;
	                }
	                w--;
	            }
	            curr_1 = curr_1->next;
	        }	        
	        adjacent_1[adjacent_c_1] = 1;
	        adjacent_c_1++;
	        bubble_sort(adjacent_1, adjacent_c_1);
	        adjacent_c_1 = removeDuplicates(adjacent_1, adjacent_c_1);	       	        
	        int final1 = adjacent_c_1 - 1;
	        while (final1 >= 0) {
	            array[count] = moves[numMoves - adjacent_1[final1]];
	            final1--;
	            count++;
	        }
	    }		
	}	 
			
	PlaceId *array_final = malloc(1000 * sizeof(PlaceId));
	*numReturnedLocs = count;
	int final_count = count - 1;
	while (final_count >= 0) {
	    array_final[final_count] = array[final_count];
	    final_count--;
	}	
	if (*numReturnedLocs == 0) {
	    return NULL;
	} else {
	    return array_final;
    } 
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
    int array[1000];	
	int count = 0;
	
	    if (GvGetRound(dv->gv) == 0) {	    
	    } else if (GvGetRound(dv->gv) < 5 && GvGetRound(dv->gv) > 0) {
	        int accept = 0;
	        int round = GvGetRound(dv->gv);
	        PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);	
	        PlaceId *array1;
	        if (road && boat) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, true, false, true, &accept);
	        } else if (road) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, true, false, false, &accept);
	        } else if (boat) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, false, false, true, &accept);
	        }
	        
	        
            int i = 0;
            int temp = accept - 1;
            while (i <= temp) {
                array[count] = array1[i];
                i++;
                count++;
            }
            
            int numMoves_judge = 0; bool canFree = false;
            PlaceId *moves_judge = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge, &canFree);
	        int p = 0;
	        int count_judge = count - 1;
	        while (p < numMoves_judge) {
	            count_judge = count - 1;
	            while (count_judge >= 0) {
	                if (moves_judge[p] == array[count_judge]) {
	                    del_element(array, count, moves_judge[p]);
	                    count--;	                
	                }
	            count_judge--;
	            }
	        p++;
	        }
	        
	        
	        int numMoves = 0; bool canFree1 = false;
            PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree1);       
            int judge = judger(dv, numMoves, moves);	    
	        if (judge == 1) {
	            array[count] = from;
	            count = count + 1;
	        }
	        if (judge == 2) {	               	        
	            ConnList curr = MapGetConnections(dv->map, from);
	            int adjacent[1000];
	            int adjacent_c = 0;
	            int q = numMoves - 1;
	            while (curr != NULL) {
	                q = numMoves - 1;
	                while (q >= 0) {
	                    if (curr->p == moves[q]) {
	                        adjacent[adjacent_c] = numMoves - q;
	                        adjacent_c++;
	                    }
	                    q--;
	                }
	                curr = curr->next;
	            }
	            adjacent[adjacent_c] = 1;
	            adjacent_c++;
	            bubble_sort(adjacent, adjacent_c);
	            adjacent_c = removeDuplicates(adjacent, adjacent_c);
	            int final = adjacent_c - 1;
	            while (final >= 0) {
	                array[count] = moves[numMoves - adjacent[final]];
	                final--;
	                count++;
	            }
	        }
	        if (judge == 0) {	               	        
	            ConnList curr_0 = MapGetConnections(dv->map, from);
	            int adjacent_0[1000];
	            int adjacent_c_0 = 0;
	            int w = numMoves - 1;
	            while (curr_0 != NULL) {
	                w = numMoves - 1;
	                while (w >= 0) {
	                    if (curr_0->p == moves[w]) {
	                        adjacent_0[adjacent_c_0] = numMoves - w;
	                        adjacent_c_0++;
	                    }
	                    w--;
	                }
	                curr_0 = curr_0->next;
	            }
	            adjacent_0[adjacent_c_0] = 1;
	            adjacent_c_0++;
	            bubble_sort(adjacent_0, adjacent_c_0);
	            adjacent_c_0 = removeDuplicates(adjacent_0, adjacent_c_0);
	            int final0 = adjacent_c_0 - 1;
	            while (final0 >= 0) {
	                array[count] = moves[numMoves - adjacent_0[final0]];
	                final0--;
	                count++;
	            }
	        }
	        
	    } else if (GvGetRound(dv->gv) >= 5) {	    
	        int accept = 0;
	        int round = GvGetRound(dv->gv);
	        PlaceId from = GvGetPlayerLocation(dv->gv, PLAYER_DRACULA);	    
	        PlaceId *array1;
	        if (road && boat) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, true, false, true, &accept);
	        } else if (road) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, true, false, false, &accept);
	        } else if (boat) {
	            array1 = GvGetReachableByType(dv->gv, PLAYER_DRACULA, round, from, false, false, true, &accept);
	        }   
            int i = 0;
            int temp = accept - 1;
            while (i <= temp) {
                array[count] = array1[i];
                i++;
                count++;
            }
            int numMoves_judge1 = 0; bool canFree = false;
            PlaceId *moves_judge1 = GvGetLocationHistory(dv->gv, PLAYER_DRACULA, &numMoves_judge1, &canFree);
	        int a = numMoves_judge1 - 1;
	        int b = numMoves_judge1 - 5;
	        int count_judge = count - 1;
	        while (a >= b) {
	            count_judge = count - 1;
	            while (count_judge >= 0) {
	                if (moves_judge1[a] == array[count_judge]) {
	                    del_element(array, count, moves_judge1[a]);
	                    count--;
	                   
	                }
	            count_judge--;
	            }
	            a--;
	        }
	        
            int numMoves = 0; bool canFree2 = false;
            PlaceId *moves = GvGetMoveHistory(dv->gv, PLAYER_DRACULA, &numMoves, &canFree2);       
            int judge = judger(dv, numMoves, moves);	    	    
	        if (judge == 2) {	        
	            ConnList curr = MapGetConnections(dv->map, from);
	            int adjacent[1000];
	            int adjacent_c = 0;
	            int q = numMoves - 1;
	            while (curr != NULL) {
	                q = numMoves - 1;
	                while (q >= 0) {
	                    if (curr->p == moves[q]) {
	                        adjacent[adjacent_c] = numMoves - q;
	                        adjacent_c++;
	                    }
	                    q--;
	                }
	                curr = curr->next;
	            }
	            adjacent[adjacent_c] = 1;
	            adjacent_c++;
	            bubble_sort(adjacent, adjacent_c);
	            adjacent_c = removeDuplicates(adjacent, adjacent_c);
	            int final = adjacent_c - 1;
	            while (final >= 0) {
	                array[count] = moves[numMoves - adjacent[final]];
	                final--;
	                count++;
	            }
	        } else if (judge == 1) {	        
	            array[count] = from;
	            count = count + 1;
	        } else if (judge == 0) {	        
	            	        	        
	            ConnList curr_1 = MapGetConnections(dv->map, from);	        
	            int adjacent_1[1000];
	            int adjacent_c_1 = 0;
	            int w = numMoves - 1;	        
	            while (curr_1 != NULL) {
	                w = numMoves - 1;
	                while (w >= 0) {	                
	                    if (curr_1->p == moves[w]) {	                    
	                        adjacent_1[adjacent_c_1] = numMoves - w;
	                        adjacent_c_1++;
	                    }
	                    w--;
	                }
	                curr_1 = curr_1->next;
	            }	        
	            adjacent_1[adjacent_c_1] = 1;
	            adjacent_c_1++;
	            bubble_sort(adjacent_1, adjacent_c_1);
	            adjacent_c_1 = removeDuplicates(adjacent_1, adjacent_c_1);	       	        
	            int final1 = adjacent_c_1 - 1;
	            while (final1 >= 0) {
	                array[count] = moves[numMoves - adjacent_1[final1]];
	                final1--;
	                count++;
	            }
	        }
	    }		
	
	
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	PlaceId *array_final = malloc(1000 * sizeof(PlaceId));
	*numReturnedLocs = count;
	int final_count = count - 1;
	while (final_count >= 0) {
	    array_final[final_count] = array[final_count];
	    final_count--;
	}	
	if (*numReturnedLocs == 0) {
	    return NULL;
	} else {
	    return array_final;
    } 
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
    *numReturnedLocs = 0;
    PlaceId from = DvGetPlayerLocation(dv, player);
    Round round = DvGetRound(dv);
    Player cur_player = dv->Curr_Player_Number;
    // if not Godalming
    if (HvgetPlayer(dv->gv) != 0) {
	turn++;
    }
    int temp = 0;
    PlaceId *locs = GvGetReachable(dv->gv, player, round, from, &temp);
    *numReturnedLocs = temp;
    return locs;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
    *numReturnedLocs = 0;
    PlaceId from = DvGetPlayerLocation(dv, player);
    Round round = DvGetRound(dv);
    Player cur_player = dv->Curr_Player_Number;
    // if not Godalming
    if (HvgetPlayer(dv->gv) != 0) {
	turn++;
    }
    int temp = 0;
    PlaceId *locs = GvGetReachableByType(dv->gv, player, round,
                              from, road, rail,
                              boat, &temp)
    *numReturnedLocs = temp;
    return locs;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
