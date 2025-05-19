/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: Ikeoluwa Oladele
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (S25)  |"
# ECHO "=---------------------------------------="
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "    @@                             @@    �
# ECHO "    @@           %&@@@@@@@@@@@     @@    �
# ECHO "    @@       @%% (@@@@@@@@@  @     @@    �
# ECHO "    @@      @& @   @ @       @     @@    �
# ECHO "    @@     @ @ %  / /   @@@@@@     @@    �
# ECHO "    @@      & @ @  @@              @@    �
# ECHO "    @@       @/ @*@ @ @   @        @@    �
# ECHO "    @@           @@@@  @@ @ @      @@    �
# ECHO "    @@            /@@    @@@ @     @@    �
# ECHO "    @@     @      / /     @@ @     @@    �
# ECHO "    @@     @ @@   /@/   @@@ @      @@    �
# ECHO "    @@     @@@@@@@@@@@@@@@         @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@          N E O N            @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[CODER SCRIPT ..........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: May 23, 2025,
* Professor: Paulo Sousa
* Purpose: This file is the main code for Buffer/Reader (A12)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * - Please check the "TODO" labels to develop your activity.
 * - Review the functions to use "Defensive Programming".
 *.............................................................................
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

/********************************************
Function name: vinegereFile
Purpose: This function validates the files to be read, checks the cipher mode and then calls the vigenereMem function to encode or decode the file.
Function In parameters: const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen, const int cypherMode
Function Out parameters: void
Version: 1.0
Author: Ikeoluwa Oladele
********************************************/
void vigenereFile(const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen, const int cypherMode) {
	//------ declarations --------
	const long input_file_size = getSizeOfFile(inputFileName);
	char* output ;

	// ------ validate the files to be read------
	if (input_file_size == -1) {
		errorPrint("The input file does not exist");
		exit(EXIT_FAILURE);
	}
	if (input_file_size == 0) {
		errorPrint("The input file provided is empty.");
		exit(EXIT_FAILURE);
	}

	//---- check the cipher mode -----
	if (cypherMode == CYPHER) {
		output = vigenereMem(inputFileName,key,keyLen,cypherMode);
	}
	else if (cypherMode == DECYPHER) {
		printf("DECYPHER\n");
		output= vigenereMem(inputFileName,key,keyLen, cypherMode);
	}
	else {
		errorPrint("The encode option provided is invalid");
		exit(EXIT_FAILURE);
	}

	//---- write result to output file ----
	FILE* outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL) {
		errorPrint("Error encountered trying to write to file");
		exit(EXIT_FAILURE);
	}
	fprintf(outputFile, "%s", output);
	fclose(outputFile);
	free(output);
}

/********************************************
Function name: vigenereMem
Purpose: This function is used to extract the content of a file and encode or decode it by calling the encode or decode function.
Function In parameters: const char* inputFileName, const char* key,const size_t keyLen, const int cypherMode
Function Out parameters: char* (encoded or decoded string)
Version: 1.0
Author: Ikeoluwa Oladele
********************************************/
char* vigenereMem(const char* inputFileName,const char* key,const size_t keyLen,const int cypherMode) {
	//--------- Declaration of variables --------
	char* fileContent = NULL;
	char* output = NULL;
	size_t currentFileContentLength = 0;
	size_t newTotalFileContentLength=0;
	size_t charRead= 0;
	const int BUFFER_SIZE = 150;
	char tempBuffer[BUFFER_SIZE];

	//--------- Extracting File Content ----------

	FILE *inputFile = fopen(inputFileName, "r");
	if (inputFile == NULL) {
		errorPrint("Error encountered trying to open file");
		exit(EXIT_FAILURE);
	}
	while (!feof(inputFile)) {
		if (ferror(inputFile)) {
			errorPrint("An error occurred while reading the input file\n");
			fclose(inputFile);
			free(fileContent);														// Free any allocated memory
			exit(EXIT_FAILURE);
		}
		charRead = fread(tempBuffer, sizeof(char), BUFFER_SIZE, inputFile);
		if (!isValidChunk(tempBuffer,charRead)) {
			free(fileContent);
			fclose(inputFile);
			errorPrint("the input file contains an invalid character\n");
			exit(EXIT_FAILURE);
		}

		// ----------Memory Allocation & Management ------
		newTotalFileContentLength = currentFileContentLength + charRead;			//new size to be allocated
		char* temp = (char*)realloc(fileContent, newTotalFileContentLength+1 );
		if (temp == NULL) {
			errorPrint("Memory reallocation failed\n");
			fclose(inputFile);
		free(fileContent);															// Free the original block if realloc failed
			exit(EXIT_FAILURE);
		}
		fileContent = temp;
		memcpy(fileContent + currentFileContentLength, tempBuffer, charRead);	//append tempBuffer to output
		currentFileContentLength += charRead;

		fileContent[currentFileContentLength] = '\0';
	}
	fclose(inputFile);

	//----- ensure the file is not empty -------
	if (fileContent  == NULL) {
		errorPrint("The input file provided is empty");
		exit(EXIT_FAILURE);
	}


	//--------- ENCODING & DECODING ------
	if (cypherMode == CYPHER ) {
		output= encode(fileContent,newTotalFileContentLength, key, keyLen);
	}
	else {
		output= decode(fileContent,newTotalFileContentLength, key, keyLen);
	}

	free(fileContent);
	return output;
}

/********************************************
Function name: isValidChunk
Purpose: This function is used to validate the content of a file.
Function In parameters: const char* strToVerify, const size_t size
Function Out parameters: int (validation result)
Version: 1.0
Author: Ikeoluwa Oladele
********************************************/
int isValidChunk(const char* strToVerify, const size_t size) {
	printf("%s\n", strToVerify);
	for (int i = 0; i < size; i++) {
		const char c = strToVerify[i];
		if ((c < ASCII_START  || c > ASCII_END) && c!='\n') {
			return 0;
		}
	}
	return 1;
}

/********************************************
Function name: encode
Purpose: The main encoding logic used to encode the content of a file.
Function In parameters: const char* str, const size_t strLen,  const char* key, const size_t keyLen
Function Out parameters: char* (encoded string)
Version: 1.0
Author: Ikeoluwa Oladele
********************************************/
char* encode(const char* str, const size_t strLen,  const char* key, const size_t keyLen) {
	//----- declaration of variables -------
	char* encodedStr = (char*)malloc((strLen+1)*sizeof(char));

	//------- encoding the characters -------
	for (int i = 0; i < strLen; i++) {
		const char c = str[i];
		if (c == '\n') {															//Avoid encoding the newline char
			encodedStr[i] = '\n';
			continue;
		}
		//--- encoding logic ----
		const char keyChar = key[i % keyLen];										//wrap around key if keyLen < strLen
		const int cShift = (int)c + (int)keyChar;									//shift c forward by key
		const int normalizedCShift = (cShift - ASCII_START) % ASCII_RANGE;		    //normalize cShift to be between 0 and ASCII_RANGE
		encodedStr[i] = (char)(normalizedCShift + ASCII_START);						//Shift c be in the range of our printable ascii characters
	}
	encodedStr[strLen] = '\0';														//append the null char to make it a string
	return encodedStr;
}

/********************************************
Function name: decode
Purpose: The main decoding logic used to decode the content of a file encoded by the encode function.
Function In parameters: const char* str, const size_t strLen,  const char* key, const size_t keyLen
Function Out parameters: char* (decoded string)
Version: 1.0
Author: Ikeoluwa Oladele
********************************************/
char* decode(const char* str, const size_t strLen,  const char* key, const size_t keyLen) {
	//----- declaration of variables -------
	char* decodedStr = (char*)malloc((strLen+1)*sizeof(char));

	//----- decoding the characters -------
	for (int i = 0; i < strLen; i++) {
		const char c = str[i];
		if (c == '\n') {
			decodedStr[i] = '\n';
			continue;
		}
		//--- decoding logic ----
		const char keyChar = key[i % keyLen];											//wrap around key if keyLen < strLen
		int cShift = ((int)c - (int)keyChar - ASCII_START) % ASCII_RANGE;				//shift c back by key
		if (cShift < 0) {
			cShift += ASCII_RANGE;														//Handle negative shifts. C doesn't handle negative modulo properly
		}
		decodedStr[i] = (char)(cShift + ASCII_START);									//Shift c be in the range of our printable ascii characters
	}
	decodedStr[strLen] = '\0';
	return decodedStr;
}

/********************************************
Function name: cypher
Purpose: This function is used to encode the content of a file.
Function In parameters: const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen
Function Out parameters: void
Version: 1.0
Author: Ikeoluwa Oladele & Paulo Sousa
********************************************/
void cypher(const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen) {
    vigenereFile(inputFileName, outputFileName, key, keyLen, CYPHER);
}

/********************************************
Function name: decypher
Purpose: This function is used to decode the content of a file.
Function In parameters: const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen
Function Out parameters: void
Version: 1.0
Author: Ikeoluwa Oladele & Paulo Sousa
********************************************/
void decypher(const char* inputFileName, const char* outputFileName, const char* key, const size_t keyLen) {
    vigenereFile(inputFileName, outputFileName, key, keyLen, DECYPHER);
}

/********************************************
Function name: getSizeOfFile
Purpose: This function is used to get the size of a file.
Function In parameters: const char* filename
Function Out parameters: long (size of a file) or -1 if the file does not exist
Version: 1.0
Author: Ikeoluwa Oladele & Paulo Sousa
********************************************/
long getSizeOfFile(const char* filename) {
	size_t size = 0;
    FILE * file = fopen(filename, "rb");
	if (file == NULL) {
		return -1;
	}
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fclose(file);
    return (long) size;
}
