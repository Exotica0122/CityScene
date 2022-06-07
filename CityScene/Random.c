#include "Random.h"

// This returns a random float between two values 
// Start and End are both inclusive
// [a, b]
float RandomFloat(float min, float max) {
return ((max - min) * ((float)rand() / RAND_MAX)) + min;
}