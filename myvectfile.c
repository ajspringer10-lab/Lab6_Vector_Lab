/**************************************************
 * Filename: myvectfile.c
 * Description: File input and output functions for vector calculator
 * Author: AJ Springer
 * Date: 10/21/2026
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myvect.h"

//Declare function since it is used elsewhere in this file
int is_file_number(char value[]);

//load_vectors: Loads vectors from a CSV file and merges them into storage
int load_vectors(vect_storage *storage, char filename[]){
    FILE *file_ptr;
    char input[200];
    char *tokens[5];
    char *token;
    vect newvect;
    vect_storage file_storage;
    int token_count;
    int loaded;
    int status;
    int i;

    //Start temporary file storage empty
    file_storage.vectors = NULL;
    file_storage.count = 0;
    file_storage.capacity = 0;

    //Open entered file for reading
    file_ptr = fopen(filename, "r");

    if(file_ptr == NULL){
        return -1;
    }//end if

    //Read each line from the file
    while(fgets(input, 200, file_ptr) != NULL){
        token_count = 0;
        token = strtok(input, ", \t\r\n");

        //Break CSV line into name, x, y, and z values
        while((token != NULL) && (token_count < 5)){
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, ", \t\r\n");
        }//end while

        //Ignore any empty lines
        if(token_count == 0){
            continue;
        }//end if

        //Valid line must have exactly 4 items
        if(token_count != 4){
            clearvectors(&file_storage);
            fclose(file_ptr);
            return -2;
        }//end if

        //Check vector name and all 3 number values
        if((strlen(tokens[0]) >= NAME_SIZE) ||
           !is_file_number(tokens[1]) ||
           !is_file_number(tokens[2]) ||
           !is_file_number(tokens[3])){
            clearvectors(&file_storage);
            fclose(file_ptr);
            return -2;
        }//end if

        //Create vector from CSV data
        strcpy(newvect.name, tokens[0]);
        newvect.x = atof(tokens[1]);
        newvect.y = atof(tokens[2]);
        newvect.z = atof(tokens[3]);

        //Store file vectors temporarily until whole file is valid
        status = addvect(&file_storage, newvect);

        if(status != 0){
            clearvectors(&file_storage);
            fclose(file_ptr);
            return -3;
        }//end if
    }//end while

    fclose(file_ptr);

    loaded = file_storage.count;

    //Merge all loaded vectors into current calculator storage
    for(i = 0; i < file_storage.count; i++){
        status = addvect(storage, file_storage.vectors[i]);

        if(status != 0){
            clearvectors(&file_storage);
            return -3;
        }//end if
    }//end for

    clearvectors(&file_storage);

    return loaded;
}//end load_vectors

//save_vectors: Saves all current vectors to a CSV file
int save_vectors(vect_storage *storage, char filename[]){
    FILE *file_ptr;
    int i;

    //Open entered file for writing, overwriting if it already exists
    file_ptr = fopen(filename, "w");

    if(file_ptr == NULL){
        return -1;
    }//end if

    //Write each currently stored vector to the file
    for(i = 0; i < storage->count; i++){
        if(fprintf(file_ptr, "%s,%.6f,%.6f,%.6f\n",
                   storage->vectors[i].name,
                   storage->vectors[i].x,
                   storage->vectors[i].y,
                   storage->vectors[i].z) < 0){
            fclose(file_ptr);
            return -2;
        }//end if
    }//end for

    fclose(file_ptr);

    return 0;
}//end save_vectors

//is_file_number: Checks if a CSV value contains a valid number
int is_file_number(char value[]){
    char *endptr;

    strtod(value, &endptr);
    if((value[0] != '\0') && (*endptr == '\0')){
        return 1;
    }//end if

    return 0;
}//end is_file_number