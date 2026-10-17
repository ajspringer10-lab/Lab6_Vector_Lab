/**************************************************
 * Filename: myvect.h
 * Description: Declarations for vector calculator
 * Author: AJ Springer
 * Date: 10/15/2026
 *************************************************/

#ifndef MYVECT_H
#define MYVECT_H

#define MAX_VECTORS 10
#define NAME_SIZE 20

//Structure used to store a vector
typedef struct{
    char name[NAME_SIZE];
    double x;
    double y;
    double z;
} vect;

//Vector math function declarations
vect add(vect a, vect b);
vect subtract(vect a, vect b);
vect scalar_multiply(vect a, double scalar);

//Vector storage function declarations
int addvect(vect vectors[], vect newvect);
int findvect(vect vectors[], char name[]);
void clearvectors(vect vectors[]);

#endif