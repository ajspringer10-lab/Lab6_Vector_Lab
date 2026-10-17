/**************************************************
 * Filename: myvectop.c
 * Description: Vector math operations for vector calculator
 * Author: AJ Springer
 * Date: 10/15/2026
 *************************************************/

#include "myvect.h"

//add: Adds 2 vectors together
vect add(vect a, vect b){
    vect returnval;
    returnval.name[0] = '\0';
    returnval.x = a.x + b.x;
    returnval.y = a.y + b.y;
    returnval.z = a.z + b.z;

    return returnval;
}//end add

//subtract: Subtracts vector b from vector a
vect subtract(vect a, vect b){
    vect returnval;
    returnval.name[0] = '\0';
    returnval.x = a.x - b.x;
    returnval.y = a.y - b.y;
    returnval.z = a.z - b.z;

    return returnval;
}//end subtract

//scalar_multiply: Multiplies a vector by a scalar value
vect scalar_multiply(vect a, double scalar){
    vect returnval;
    returnval.name[0] = '\0';
    returnval.x = a.x * scalar;
    returnval.y = a.y * scalar;
    returnval.z = a.z * scalar;

    return returnval;
}//end scalar_multiply