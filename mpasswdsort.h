 /*   mpasswdsort.h
   *  David Töyrä
   *  Obligtorisk Uppgift 1 Systemnära progremmering Ht17
   *  2017-09-26
   *  
   */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "list.h"


#define BUFFER 1024
#define INVALID_INPUT 1
#define VALID_INPUT 0

typedef struct passwd
{
	int UID;
	char *username;
}passwd;

/*
 * Returns the input stream depending on number of arguments
 * provided in the command prompt.
 * 
 * Will return stdin stream if no argument is found,
 * FILE pointer if exactly one argument is found, and
 * exit if argument count is neither.
 * Parameters: argc - amount of arguments sent to main.
 			   argv - pointer to argument string.
 *
 * Returns: argv as FILE or stdin.
 *
 */
FILE *getInputFormat(int argc, char *argv[]);

/*
 * Counts the occurence of the colon character in a string,
 * if it's equal to 6 the format is deemed to be correct,
 * else the format is wrong.
 *
 * Paramters: s - pointer to string that should be checked.
 * Returns: bool value of whether format is correct.
 */
bool checkStringFormat(char *s);

/*
 * Free function for mpasswd, frees the
 * allocations of the struct and the username.
 *
 * Parameters: removedPassWd - the passwd struct that is to be removed.
 */
 void passwdFreeFunc(void *removedPassWd);
 
/*
 * Check if the username is of valid length.
 *
 * Parameters: s - pointer to string that will be checked.
 * Returns: bool value of whehether username has valid length.
 */
bool userNameIsValid(char *s);


/*
 * Sort the list by user ID in ascending order
 * using bubblesort algorithm. Bool value sorted will be changed to
 * false if the list performs a swap. If the list has gone through it's nodes
 * without doing any swap it is fully sorted.
 *
 * Paratmeters: l - list that is to be sorted.
 * Returns: the sorted list.
 */
list sortPwdList(list *l);

/*
 * Goes through the entire list and prints the passwd usename and ID.
 *
 * Parameters: l - the list that should be sorted.
 * Returns: EXIT_FAILURE if unsuccessful, else 0.
 */
int printList(list *l);

/*
 * Checks the passwd if every field is valid, and puts
 * username and ID into list if it is.
 *
 * Parameters: line - one line from the file.
 *			   lineCount - line counter
 *			   passwdList - list of passwd usernames and ID's.
 * Returns: 1 if atleast one line has an error, 0 otherwise.
 */
int checkPasswdLine(char *line, int lineCount, list *passwdList);

/*
 * Read the file line by line and check for validness.
 * Put the username and id into the list.
 *
 * Parameters: inputFile - pointer to file that will be read.
 *			   passwdList - list that will have passwd entries inserted into it.
 * Returns: 0 if no invalid line or 1 if there was an invalid line.
 */
 int readPwdFile(FILE *inputFile, list *passwdList);

