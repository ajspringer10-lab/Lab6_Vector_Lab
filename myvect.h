/**************************************************
 * Filename: myvect.h
 * Description: Declarations for vector calculator
 * Author: AJ Springer
 * Date: 10/21/2026
 *************************************************/

#ifndef MYVECT_H
#define MYVECT_H

#define START_VECTORS 10
#define NAME_SIZE 20

//Structure used to store a vector
typedef struct{
    char name[NAME_SIZE];
    double x;
    double y;
    double z;
}vect;

//Structure used to manage dynamic vector storage
typedef struct{
    vect *vectors;
    int count;
    int capacity;
}vect_storage;

//Vector math function declarations
vect add(vect a, vect b);
vect subtract(vect a, vect b);
vect scalar_multiply(vect a, double scalar);

//Vector storage function declarations
int addvect(vect_storage *storage, vect newvect);
int findvect(vect_storage *storage, char name[]);
void clearvectors(vect_storage *storage);

//Vector file function declarations
int load_vectors(vect_storage *storage, char filename[]);
int save_vectors(vect_storage *storage, char filename[]);

#endif