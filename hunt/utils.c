
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Places.h"
#include "utils.h"

/**
 * Extracts places whose corresponding index in bools is true
 */
PlaceId *boolsToPlaces(bool bools[NUM_REAL_PLACES], int *numPlaces) {
	PlaceId *locations = malloc(NUM_REAL_PLACES * sizeof(PlaceId));
	assert(locations != NULL);
	
	*numPlaces = 0;
	for (int i = 0; i < NUM_REAL_PLACES; i++) {
		if (bools[i]) {
			locations[(*numPlaces)++] = i;
		}
	}
	return locations;
}

bool placesContains(PlaceId *places, int numPlaces, PlaceId place) {
	for (int i = 0; i < numPlaces; i++) {
		if (places[i] == place) {
			return true;
		}
	}
	return false;
}

void placesCopy(PlaceId *dest, PlaceId *src, int numPlaces) {
	for (int i = 0; i < numPlaces; i++) {
		dest[i] = src[i];
	}
}

void placesDelete(PlaceId *places, int numPlaces, PlaceId place) {
	for (int i = 0; i < numPlaces; i++) {
		if (places[i] == place) {
			places[i] = places[numPlaces - 1];
			return;
		}
	}
	assert(0);
}

void placesFill(PlaceId *places, int numPlaces, PlaceId place) {
	for (int i = 0; i < numPlaces; i++) {
		places[i] = place;
	}
}

void placesReverse(PlaceId *array, int numPlaces) {
	for (int i = 0, j = numPlaces - 1; i < j; i++, j--) {
		PlaceId tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}
