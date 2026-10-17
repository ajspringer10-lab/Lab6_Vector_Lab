/**************************************************
 * Filename: myvectorarray.c
 * Description: Vector storage functions for vector calculator
 * Author: AJ Springer
 * Date: 10/15/2026
 *************************************************/

#include <string.h>
#include "myvect.h"

//addvect: Adds new vector or replaces vector with same name
int addvect(vect vectors[], vect newvect){
    int i;
    int location;

    //First check if vector name already exists
    location = findvect(vectors, newvect.name);

    if(location >= 0){
        vectors[location] = newvect;
        return 0; //End here if vector exists
    }//end if

    //If not found, put vector in first empty location
    for(i = 0; i < MAX_VECTORS; i++){
        if(vectors[i].name[0] == '\0'){
            vectors[i] = newvect;
            return 0;
        }//end if
    }//end for

    //If here, no open array locations were found
    return -1;
}//end addvect


//findvect: Searches for a vector name and returns its array location
int findvect(vect vectors[], char name[]){
    int i;

    for(i = 0; i < MAX_VECTORS; i++){
		//Confirm that name isn't null first
        if(vectors[i].name[0] != '\0'){
			//Check if names match
            if(strcmp(vectors[i].name, name) == 0){	
                return i;
            }//end if
        }//end if
    }//end for

	//If here, name couldn't be found, return -1
    return -1;
}//end findvect

//clearvectors: Clears all vector storage locations
void clearvectors(vect vectors[]){
    int i;

	//For each spot, clear vector and x/y/z
    for(i = 0; i < MAX_VECTORS; i++){
        vectors[i].name[0] = '\0';
        vectors[i].x = 0.0;
        vectors[i].y = 0.0;
        vectors[i].z = 0.0;
    }//end for
}//end clearvectors