
#include <stdbool.h>
#include <stdlib.h>

#include "Places.h"

#ifndef FOD__UTILS_H_
#define FOD__UTILS_H_

/**
 * Extracts places whose corresponding index in bools is true
 *
 * Examples:
 * [false, true, false, true] --> [1, 3]
 * [true, false, true, true] --> [0, 2, 3]
 */
PlaceId *boolsToPlaces(bool bools[NUM_REAL_PLACES], int *numPlaces);

bool placesContains(PlaceId *places, int numPlaces, PlaceId place);

/**
 * Copies an array of places from 'src' to 'dest'
 * Assumes the destination array has enough room
 */
void placesCopy(PlaceId *dest, PlaceId *src, int numPlaces);

/**
 * Deletes one instance of a place from the given array
 */
void placesDelete(PlaceId *places, int numPlaces, PlaceId place);

/**
 * Fills an array of places with the given place
 */
void placesFill(PlaceId *places, int numPlaces, PlaceId place);

/**
 * Reverses an array of places
 */
void placesReverse(PlaceId *array, int numPlaces);

#endif // !defined (FOD__TEST_UTILS_H_)
