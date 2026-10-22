/**************************************************
 * Filename: myvectorarray.c
 * Description: Vector storage functions for vector calculator
 * Author: AJ Springer
 * Date: 10/21/2026
 *************************************************/

#include <stdlib.h>
#include <string.h>
#include "myvect.h"

//addvect: Adds new vector or replaces vector with same name
int addvect(vect_storage *storage, vect newvect){
    int location;
    int new_capacity;
    vect *new_vectors;

    //First check if vector name already exists
    location = findvect(storage, newvect.name);

    if(location >= 0){
        storage->vectors[location] = newvect;
        return 0; //End here if vector exists
    }//end if

    //If array is full, dynamically increase the storage size
    if(storage->count >= storage->capacity){
        if(storage->capacity == 0){
            new_capacity = START_VECTORS;
            new_vectors = malloc(new_capacity * sizeof(vect));
        }else{
            new_capacity = storage->capacity * 2;
            new_vectors = realloc(storage->vectors, new_capacity * sizeof(vect));
        }//end if-else

        //Confirm that memory was allocated correctly
        if(new_vectors == NULL){
            return -1;
        }//end if

        storage->vectors = new_vectors;
        storage->capacity = new_capacity;
    }//end if

    //Put new vector at the end of the used storage
    storage->vectors[storage->count] = newvect;
    storage->count++;

    return 0;
}//end addvect

//findvect: Searches for a vector name and returns its array location
int findvect(vect_storage *storage, char name[]){
    int i;

    for(i = 0; i < storage->count; i++){
        //Check if names match
        if(strcmp(storage->vectors[i].name, name) == 0){
            return i;
        }//end if
    }//end for

    //If here, name couldn't be found, return -1
    return -1;
}//end findvect

//clearvectors: Clears all vector storage locations
void clearvectors(vect_storage *storage){
    //Release any dynamically allocated vector memory
    free(storage->vectors);

    storage->vectors = NULL;
    storage->count = 0;
    storage->capacity = 0;
}//end clearvectors