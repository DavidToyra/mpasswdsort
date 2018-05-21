  /*  mpasswdsort.c
   *  David Töyrä
   *  Obligatorisk uppgift 1 Systemnära programmering HT17
   *  2017-09-26
   * 
   *  Program that will read the passwd input of a file or stdin,
   *  store the username and user ID in a sorted list and then print it.
   *
   */
   

#include "mpasswdsort.h"
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
FILE *getInputFormat(int argc, char *argv[])
{
	
	if(argc==1)
	{
		return stdin;
	}
		
	else if(argc==2)
	{
		FILE *inputFile = fopen(argv[1], "r");
		if(inputFile == NULL)
		{
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
		fseek(inputFile, 0, SEEK_END);
		if(ftell(inputFile) == 0)
		{
			fclose(inputFile);
			exit(EXIT_SUCCESS);
		}
		rewind(inputFile);
		return inputFile;
	}
	
	else
	{
		fprintf(stderr, "Wrong amount of arguments");
		exit(EXIT_FAILURE);
	}
	
}
/*
 * Counts the occurence of the colon character in a string,
 * if it's equal to 6 the format is deemed to be correct,
 * else the format is wrong.
 *
 * Paramters: s - pointer to string that should be checked.
 * Returns: bool value of whether format is correct.
 */
bool checkStringFormat(char *s)
{
	int colonCounter = 0;
	const int len = strlen(s);
	
	for(int i = 0; i<len;i++)
	{
		if(s[i]==':')
		{
			colonCounter++;
		}
	}
	
	if(colonCounter==6)
	{
		return true;
	}
	return false;
}

/*
 * Free function for mpasswd, frees the
 * allocations of the struct and the username.
 *
 * Parameters: removedPassWd - the passwd struct that is to be removed.
 */
 void passwdFreeFunc(void *removedPassWd)
 {
	 passwd *temp = (passwd*)removedPassWd;
	 free(temp->username);
	 free(temp);
 }
 
/*
 * Check if the username is of valid length.
 *
 * Parameters: s - pointer to string that will be checked.
 * Returns: bool value of whehether username has valid length.
 */
bool userNameIsValid(char *s)
{
	if(strlen(s)<1 || strlen(s)>32)
	{
		return false;
	}
	return true;
}

/*
 * Sort the list by user ID in ascending order
 * using bubblesort algorithm. Bool value sorted will be changed to
 * false if the list performs a swap. If the list has gone through it's nodes
 * without doing any swap it is fully sorted.
 *
 * Paratmeters: l - list that is to be sorted.
 * Returns: the sorted list.
 */
list sortPwdList(list *l)
{
	
	bool sorted = false;
	listPosition p;
	passwd *temp1;
	passwd *temp2;
	
	while(!sorted)
	{
		p = list_first(l);
		sorted=true;
		while(p->nextNode != l->endNode)
		{
			temp1 = list_inspect(p);
			temp2 = list_inspect(p->nextNode);
			if(temp1->UID > temp2->UID)
			{
				passwd *tempPasswd;
				tempPasswd = p->value;
				p->value = p->nextNode->value;
				p->nextNode->value = tempPasswd;
				sorted = false;
			}
			
			p = list_next(p);
		}
	}

	return *l;
}

/*
 * Goes through the entire list and prints the passwd usename and ID.
 *
 * Parameters: l - the list that should be sorted.
 * Returns: EXIT_FAILURE if unsuccessful, else 0.
 */
int printList(list *l)
{
	if(list_isEmpty(l))
	{
		fprintf(stderr, ("Cannot print an empty list.\n"));
		exit(EXIT_FAILURE);
	}
	listPosition p = list_first(l);
	passwd *printPwd;
	while(p != l->endNode)
	{
		printPwd = list_inspect(p);
		printf("%d:%s\n", printPwd->UID, printPwd->username);
		p = list_next(p);
	}
	
	return 0;
}

/*
 * Checks the passwd if every field is valid, and puts
 * username and ID into list if it is.
 *
 * Parameters: line - one line from the file.
 *			   lineCount - line counter
 *			   passwdList - list of passwd usernames and ID's.
 * Returns: 1 if atleast one line has an error, 0 otherwise.
 */
int checkPasswdLine(char *line, int lineCount, list *passwdList)
{
	char *userName;
	char *errString = NULL;
	char *ptrToBuffer=line;
	char *token;
	int lineError = 0;
	
	if(strlen(line)==0)
	{
		fprintf(stderr, "Line %d: Encountered a <BLANKLINE>", lineCount);
		lineError = 1;
		return lineError;
	}
		
	else if(!checkStringFormat(line))
	{
		fprintf(stderr, "Line %d: Invalid format: %s", lineCount, line);
		lineError = 1;
		return lineError;
		
	}
		

	/*Check if username is valid*/
	userName = strsep(&ptrToBuffer, ":");
	if(!userNameIsValid(userName))
	{
		fprintf(stderr, "Line %d: username must be 1-32 characters: %s\n", lineCount, userName);
		lineError = 1;
		return lineError;
	}
	strsep(&ptrToBuffer, ":");
		
	/* Check if UID is valid */
	token = strsep(&ptrToBuffer, ":");
	int ID = strtol(token, &errString, 10);
	if(ID<0 || *errString!=0)
	{
		fprintf(stderr, "Line %d: UID must be a positive number: %d\n", lineCount, ID);
		lineError = 1;
		return lineError;
	}
		
		
	/*Check if GID is valid*/
	token = strsep(&ptrToBuffer, ":");
	int gidInt = strtol(token, &errString, 10);
	if(gidInt<0 || *errString!=0)
	{
		fprintf(stderr, "Line %d: GID must be a positive number: %s\n", lineCount, errString);
		lineError = 1;
		return lineError;
	}
	strsep(&ptrToBuffer, ":");
	
	/*Check if directory is valid*/
	token = strsep(&ptrToBuffer, ":");
	if(strlen(token)== 0)
	{
		fprintf(stderr, "Line %d: directory field cannot be empty\n", lineCount);
		lineError = 1;
		return lineError;
	}
		
	/*Check if shell is valid*/
	token = strsep(&ptrToBuffer, "\0");
	if(strlen(token)== 0)
	{
		fprintf(stderr, "Line %d: shell field cannot be empty\n", lineCount);
		lineError = 1;
		return lineError;
	}
		
	passwd *passwdValue = malloc(sizeof(passwd));
	if(passwdValue==NULL)
	{
		perror("Allocating memory for passwd struct: ");
		exit(EXIT_FAILURE);
	}
		
	/*Insert username and ID into the list.*/
	passwdValue->UID = ID;
	passwdValue->username = malloc(strlen(userName) + 1);
	strcpy(passwdValue->username, userName);
	list_insert(passwdList, passwdValue, list_first(passwdList));
	return lineError;
	
}

/*
 * Read the file line by line and check for validness.
 * Put the username and id into the list.
 *
 * Parameters: inputFile - pointer to file that will be read.
 *			   passwdList - list that will have passwd entries inserted into it.
 * Returns: 0 if no invalid line or 1 if there was an invalid line.
 */
 int readPwdFile(FILE *inputFile, list *passwdList)
 {
 	
	char lineBuffer[BUFFER];
	int lineCount = 1;
	int errorInFile = 0;
	int checkFileResult;

	/*Loop through every line of the file*/
	while(fgets(lineBuffer, BUFFER, inputFile))
	{
		checkFileResult = checkPasswdLine(lineBuffer, lineCount, passwdList);
		if(checkFileResult == 1)
		{
			errorInFile = 1;
		}
		lineCount++;
	}
	
	fclose(inputFile);
	return errorInFile;
 }

/*	
 * Mpasswdsort main function, takes either a file or text through stdin as input.
 * Will check content of the input, and print out a sorted list of the passwd entries
 * if there are any.
 */
int main(int argc, char *argv[])
{
	int inputCheck = INVALID_INPUT;
	FILE *f = getInputFormat(argc, argv);
	
	/* Check if file is empty */
	if(f!=NULL)
	{	
		fseek(f, 0, SEEK_END);
		if(ftell(f) == 0)
		{
			exit(EXIT_SUCCESS);
		}
		rewind(f);
	}
	
	list *userList = list_makeList();
	list_setMemHandler(userList, passwdFreeFunc);
	
	inputCheck = readPwdFile(f, userList);

	if(!list_isEmpty(userList))
	{
		sortPwdList(userList);
		printList(userList);
	}
	
	list_free(userList);
	
	return inputCheck;
}
