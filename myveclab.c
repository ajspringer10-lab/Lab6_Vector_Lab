/**************************************************
 * Filename: myveclab.c
 * Description: User interface for Lab 7 Vector Calculator
 * Author: AJ Springer
 * Date: 10/21/2026
 * If not using makefile: gcc -Wall myveclab.c myvectorarray.c myvectop.c myvectfile.c -o myveclab
 * If using makefile: make
 * To run: ./myveclab
 * Help: ./myveclab -h
 *************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myvect.h"

//Function declarations
void user_interface(void);
void print_help(void);
void print_vector(vect value);
void list_vectors(vect_storage *storage);
int is_number(char value[]);
int perform_operation(vect_storage *storage, char first[], char operator[], char second[], vect *result);

int main(int argc, char *argv[]){
    //Run normal calculator with no command line arguments
    if(argc == 1){
        user_interface();
    //Print help information when -h is used
    }else if((argc == 2) && (strcmp(argv[1], "-h") == 0)){
        print_help();
    //Too many arguments entered
    }else{
        printf("Invalid arguments.\n");
        printf("Use ./myveclab -h for help.\n");
        return 1;
    }//end if-else

    return 0;
}//end main

//user_interface: Gets and processes commands from the user
void user_interface(void){
    //Declare all variables needed for user interface
    vect_storage storage;
    vect newvect;
    vect result;

    char user_input[100];
    char *tokens[6];
    char *token;

    int token_count;
    int location;
    int status;
    int running = 1;

    //Ensure we start with empty vector storage
    storage.vectors = NULL;
    storage.count = 0;
    storage.capacity = 0;

    while(running == 1){
        printf("minimat> ");

        //Use fgets for user input
        if(fgets(user_input, 100, stdin) == NULL){
            break;
        }//end if

        //Break input into tokens using spaces, commas, tabs, or newlines
        token_count = 0;
        token = strtok(user_input, " ,\t\n");

        //Fill tokens array with user input
        while((token != NULL) && (token_count < 6)){
            tokens[token_count] = token;
            token_count++;
            token = strtok(NULL, " ,\t\n");
        }//end while

        //Ignore blank lines
        if(token_count == 0){
            continue;
        }//end if

        //One token can be a command or vector name
        if(token_count == 1){
            if(strcmp(tokens[0], "quit") == 0){
                running = 0; //Exit since user wants to quit
            }else if(strcmp(tokens[0], "clear") == 0){
                clearvectors(&storage);
                printf("Vector memory cleared\n");
            }else if(strcmp(tokens[0], "list") == 0){
                list_vectors(&storage);
            }else{
                //Must be find if we made it here
                location = findvect(&storage, tokens[0]);
                if(location >= 0){
                    print_vector(storage.vectors[location]);
                }else{
                    printf("Vector %s does not exist\n", tokens[0]);
                }//end if-else
            }//end if-else

        //Two tokens can be load or save with a filename
        }else if(token_count == 2){
            if(strcmp(tokens[0], "load") == 0){
                status = load_vectors(&storage, tokens[1]);

                if(status >= 0){
                    printf("%d vector(s) loaded\n", status);
                    list_vectors(&storage);
                }else if(status == -1){
                    printf("File %s does not exist or could not be opened\n", tokens[1]);
                }else if(status == -2){
                    printf("File %s is not formatted correctly\n", tokens[1]);
                }else{
                    printf("Unable to allocate vector memory\n");
                }//end if-else

            }else if(strcmp(tokens[0], "save") == 0){
                status = save_vectors(&storage, tokens[1]);

                if(status == 0){
                    printf("Vectors saved to %s\n", tokens[1]);
                }else{
                    printf("File %s could not be saved\n", tokens[1]);
                }//end if-else

            }else{
                printf("Invalid command\n");
            }//end if-else

        //Three tokens should be a math operation
        }else if(token_count == 3){
            status = perform_operation(&storage, tokens[0], tokens[1], tokens[2], &result);
            if(status == 0){
                strcpy(result.name, "ans");
                print_vector(result);
            }else{
                //Must be invalid
                printf("Invalid operation or vector not found\n");
            }//end if-else

        //Five tokens can be an assignment or operation with assignment
        }else if((token_count == 5) && (strcmp(tokens[1], "=") == 0)){

            //Check for direct vector assignment (like a = 1 2 3)
            if(is_number(tokens[2]) && is_number(tokens[3]) && is_number(tokens[4])){
                //Make sure vector name can fit in structure
                if(strlen(tokens[0]) >= NAME_SIZE){
                    printf("Vector name is too long\n");
                    continue;
                }//end if

                strcpy(newvect.name, tokens[0]);
                newvect.x = atof(tokens[2]);
                newvect.y = atof(tokens[3]);
                newvect.z = atof(tokens[4]);

                status = addvect(&storage, newvect);
                if(status == 0){
                    //Vectors were able to be added
                    print_vector(newvect);
                }else{
                    //Dynamic memory couldn't be allocated
                    printf("Unable to allocate vector memory\n");
                }//end if-else

            //Otherwise try operation with assignment
            //Example: c = a + b
            }else{

                status = perform_operation(&storage, tokens[2], tokens[3], tokens[4], &result);
                if(status == 0){
                    //Operation was valid
                    if(strlen(tokens[0]) >= NAME_SIZE){
                        printf("Vector name is too long\n");
                        continue;
                    }//end if

                    strcpy(result.name, tokens[0]);
                    status = addvect(&storage, result);

                    if(status == 0){
                        print_vector(result);
                    }else{
                        printf("Unable to allocate vector memory\n");
                    }//end if-else
                }else{
                    printf("Invalid operation or vector not found\n");
                }//end if-else
            }//end if-else

        }else{
            //Command must have been invalid
            printf("Invalid command\n");
        }//end if-else

    }//end while

    //Free all dynamic memory before program exits
    clearvectors(&storage);

}//end user_interface

//print_help: Prints valid commands for the calculator
void print_help(void){
    printf("Vector calculator commands:\n");
    printf("var = x y z         Create or replace a vector\n");
    printf("var = x,y,z         Commas may also separate values\n");
    printf("var1 + var2         Add 2 vectors\n");
    printf("var1 - var2         Subtract 2 vectors\n");
    printf("var1 * number       Multiply vector by scalar\n");
    printf("number * var1       Multiply scalar by vector\n");
    printf("result = operation  Store the result of an operation\n");
    printf("var                 Display a vector\n");
    printf("list                Display all stored vectors\n");
    printf("clear               Clear all stored vectors\n");
    printf("load <name>         Load vectors from a CSV file\n");
    printf("save <name>         Save vectors to a CSV file\n");
    printf("quit                Exits the program\n");
    printf("Spaces are required around =, +, -, and *\n");
}//end print_help

//print_vector: Prints a vector name and its 3 values
void print_vector(vect value){
    printf("%s = %.2f   %.2f   %.2f\n", value.name, value.x, value.y, value.z);
}//end print_vector

//list_vectors: Prints all vectors currently stored
void list_vectors(vect_storage *storage){
    int i;
    int count = 0;

    for(i = 0; i < storage->count; i++){
        print_vector(storage->vectors[i]);
        count++;
    }//end for
    if(count == 0){
        printf("No vectors are currently stored\n");
    }//end if

}//end list_vectors

//is_number: Checks if a string contains a valid number
int is_number(char value[]){
    char *endptr;

    strtod(value, &endptr);
	//Must be a number for either of following conditions
    if((value[0] != '\0') && (*endptr == '\0')){
        return 1;
    }//end if

    return 0;
}//end is_number

//perform_operation: Performs add, subtract, or scalar multiplication
int perform_operation(vect_storage *storage, char first[], char operator[], char second[], vect *result){
    int first_location;
    int second_location;
    double scalar;

    //Vector addition
    if(strcmp(operator, "+") == 0){
        first_location = findvect(storage, first);
        second_location = findvect(storage, second);

        if((first_location < 0) || (second_location < 0)){
            //Invalid location(s) detected
            return -1;
        }//end if

        *result = add(storage->vectors[first_location], storage->vectors[second_location]);
        return 0;

    //Vector subtraction
    }else if(strcmp(operator, "-") == 0){
        first_location = findvect(storage, first);
        second_location = findvect(storage, second);

        if((first_location < 0) || (second_location < 0)){
            //Invalid location(s) detected
            return -1;
        }//end if

        *result = subtract(storage->vectors[first_location], storage->vectors[second_location]);

        return 0;

    //Scalar multiplication (can be vector * number or number * vector)
    }else if(strcmp(operator, "*") == 0){

        //First value is the scalar
        if(is_number(first)){
            second_location = findvect(storage, second);

            if(second_location < 0){
                //Invalid location detected
                return -1;
            }//end if

            scalar = atof(first);
            *result = scalar_multiply(storage->vectors[second_location], scalar);

            return 0;

        //Second value is the scalar
        }else if(is_number(second)){
            first_location = findvect(storage, first);

            if(first_location < 0){
                //Invalid location detected
                return -1;
            }//end if

            scalar = atof(second);
            *result = scalar_multiply(storage->vectors[first_location], scalar);

            return 0;
        }//end if-else

    }//end if-else

    return -1;
}//end perform_operation