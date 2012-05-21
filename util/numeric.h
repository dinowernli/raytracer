/*
 * Contains basic numerics-related definitions such as the types used for
 * vectors and colors.
 * Author: Dino Wernli
 */

#ifndef NUMERIC_H_
#define NUMERIC_H_

#define EPSILON 0.000001
#define PI 3.1415926535897932384626433
#define MILLI_TO_MICRO 1000

// Represents the type of a scalar value such as a coefficient of a vector.
typedef double Scalar;

// Represents the value which can be taken by a red, green or blue channel.
typedef float Intensity;

#endif  /* NUMERIC_H_ */
