/*
 * Contains a macro which declares the copy constructor and assignment operator
 * of a class as deleted.
 * Author: Dino Wernli
 */

#ifndef NOCOPYASSIGN_H_
#define NOCOPYASSIGN_H_

#define NO_COPY_ASSIGN(Type) \
    Type(const Type& other) = delete; \
    Type operator=(const Type& other) = delete;

#endif  /* NOCOPYASSIGN_H_ */
