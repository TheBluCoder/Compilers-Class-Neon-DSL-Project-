/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Summer, 2025
* Author: TO_DO
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
# ECHO "    @@         S O F I A           @@    �
# ECHO "    @@                             @@    �
# ECHO "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# ECHO "                                         "
# ECHO "[READER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
***********************************************************
* File name: Reader.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012, 013]
* Assignment: A12.
* Date: Jan 01 2025
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

#include <ctype.h>
#include <string.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef CODER_H_
#include "Step1Coder.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

/*
***********************************************************
* Function name: readerCreate
* Purpose: Creates the buffer reader according to capacity, increment
	factor and operational mode ('f', 'a', 'm')
* Author: Svillen Ranev / Paulo Sousa
* History/Versions: S22
* Called functions: calloc(), malloc()
* Parameters:
*   size = initial capacity
*   increment = increment factor
*   mode = operational mode
* Return value: bPointer (pointer to reader)
* Algorithm: Allocation of memory according to initial (default) values.
* TODO ......................................................
*	- Adjust datatypes for your LANGUAGE.
*   - Use defensive programming
*	- Check boundary conditions
*	- Check flags.
*************************************************************
*/

BufferPointer readerCreate(int size) {
	BufferPointer readerPointer = NULL;
	/* TO_DO: Defensive programming: size */
	/* TO_DO: readerPointer allocation */
	/* TO_DO: Defensive programming: readerPointer */
	if (size == 0) {
		return NULL;
	}
	readerPointer = calloc(1, sizeof(Buffer));
	if (readerPointer == NULL) {
		errorPrint("Memory allocation error: Could not allocate memory for reader.");
		return NULL;
	}
	/* TO_DO: content allocation */
	char* content = malloc(size*sizeof(char));
	if (content==NULL) {
		errorPrint("Memory allocation error: Could not allocate memory for reader content.");
		free(readerPointer);
		return NULL;
	}

	readerPointer->size = size;
	readerPointer->content = content;

	readerPointer->size = size;
	readerPointer->content = content;


	/* Initialize positions */
	readerPointer->position.read = POSITION_INITIAL_POINT;
	readerPointer->position.wrte = POSITION_INITIAL_POINT;
	readerPointer->position.mark = POSITION_INITIAL_POINT;

	/* Initialize histogram */
	for (int i = 0; i < NCHAR; i++)
		readerPointer->histogram[i] = 0;

	/* Initialize errors */
	readerPointer->numReaderErrors = 0;

	/* Initialize flags */
	readerPointer->flags.isEmpty = True;
	readerPointer->flags.isFull = False;
	readerPointer->flags.isRead = False;
	readerPointer->flags.isMoved = False;

	/* TO_DO: The created flag must be signalized as EMP */
	return readerPointer;
}


/*
***********************************************************
* Function name: readerAddChar
* Purpose: Adds a char to buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   ch = char to be added
* Return value:
*	readerPointer (pointer to Buffer Reader)
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/


BufferPointer readerAddChar(BufferPointer const readerPointer, char ch) {
	char* tempReader = NULL;
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return NULL;
	}

	/* Check if character is valid (between ASCII Range) */
	if (ch < 0 || ch >= NCHAR) {
		readerPointer->numReaderErrors++;
		return readerPointer;  // Return the pointer even on error
	}

	/* Check if buffer needs reallocation BEFORE adding the character */
	if (readerPointer->position.wrte + 1 >= readerPointer->size) {
		size_t newSize = readerPointer->size * 2;
		tempReader = (char*)realloc(readerPointer->content, newSize);

		/* Defensive programming */
		if (tempReader == NULL) {
			errorPrint("Memory allocation error: Could not re-allocate memory for reader content.");
			return NULL;
		}

		if (readerPointer->content != tempReader) {
			readerPointer->flags.isMoved = True;
		}

		readerPointer->content = tempReader;
		readerPointer->size = newSize;
		readerPointer->flags.isFull = False;  // Buffer is no longer full after reallocation
	}

	/* Add the char */
	readerPointer->content[++(readerPointer->position.wrte)] = ch;

	/* Check if buffer is now full after adding the character */
	if (readerPointer->position.wrte + 1 >= readerPointer->size) {
		readerPointer->flags.isFull = True;
	}

	/* Reset the Empty flag */
	if (readerPointer->flags.isEmpty) {
		readerPointer->flags.isEmpty = False;
	}

	/* Updates histogram */
	(readerPointer->histogram[ch])++;

	/* Updates checksum */
	readerPointer->checkSum += ch;

	return readerPointer;
}
/*
***********************************************************
* Function name: readerClear
* Purpose: Clears the buffer reader
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerClear(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return False;
	}

	/* Adjust positions to zero */
	readerPointer->position.read = POSITION_INITIAL_POINT;
	readerPointer->position.wrte = POSITION_INITIAL_POINT;
	readerPointer->position.mark = POSITION_INITIAL_POINT;

	/* Adjust flags to original state */
	readerPointer->flags.isEmpty = True;   /* Buffer is now empty */
	readerPointer->flags.isFull = False;   /* Buffer is not full */
	readerPointer->flags.isRead = False;   /* Nothing has been read */
	readerPointer->flags.isMoved = False;  /* Reset moved flag */

	/* Optional: Clear the content array */
	if (readerPointer->content != NULL && readerPointer->size > 0) {
		memset(readerPointer->content, 0, readerPointer->size);
	}

	/* Reset statistics */
	for (int i = 0; i < NCHAR; i++) {
		readerPointer->histogram[i] = 0;
	}
	readerPointer->checkSum = 0;

	return True;
}

/*
***********************************************************
* Function name: readerFree
* Purpose: Releases the buffer address
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerFree(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* Free memory (buffer/content) */
	free(readerPointer->content);
	free(readerPointer);
	return True;
}

/*
***********************************************************
* Function name: readerIsFull
* Purpose: Checks if buffer reader is full
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsFull(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Check flag if buffer is FUL */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->flags.isFull;
}


/*
***********************************************************
* Function name: readerIsEmpty
* Purpose: Checks if buffer reader is empty.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerIsEmpty(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Check flag if buffer is EMP */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->flags.isEmpty;
}

/*
***********************************************************
* Function name: readerSetMark
* Purpose: Adjust the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   mark = mark position for char
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerSetMark(BufferPointer const readerPointer, int mark) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	const int oldMark = readerPointer->position.mark;
	readerPointer->position.mark = mark;
	return readerPointer->position.mark == oldMark;
}


/*
***********************************************************
* Function name: readerPrint
* Purpose: Prints the string in the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of chars printed.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerPrint(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming (including invalid chars) */
	/* TO_DO: Print the buffer content */

	if (readerPointer == NULL) {
		return READER_ERROR;
	}

	/* Check if buffer is empty */
	if (readerPointer->flags.isEmpty || readerPointer->content == NULL) {
		return 0;  /* No characters to print */
	}

	int charsPrinted = 0;
	int currentPos = readerPointer->position.read+1;
	int writePos = readerPointer->position.wrte+1;
	char currentChar;

	while (currentPos < writePos) {
		currentChar = readerPointer->content[currentPos];

		if (currentChar == READER_TERMINATOR) {
			break;  /* Stop at null terminator */
		}
		/* Print valid characters */
		if (currentChar >= 32 && currentChar <= 126) {
			/* Printable ASCII characters */
			printf("%c", currentChar);
		} else if (currentChar == '\n' || currentChar == '\t' || currentChar == '\r') {
			/* Handle common whitespace characters */
			printf("%c", currentChar);
		} else {
			/* Handle other control characters - print as escape sequence or skip */
			printf("%d", currentChar);
		}
		charsPrinted++;
		currentPos++;
	}

	/* Update read position to match write position after printing */
	readerPointer->position.read = currentPos;

	return charsPrinted;
}

/*
***********************************************************
* Function name: readerLoad
* Purpose: Loads the string in the buffer with the content of
	an specific file.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   fileDescriptor = pointer to file descriptor
* Return value:
*	Number of chars read and put in buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerLoad(BufferPointer const readerPointer, char* fileName) {
	int count = 0;
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Loads the file */
	/* TO_DO: Creates the string calling vigenereMem(fileName, STR_LANGNAME, DECYPHER) */
	char * decodedStr = vigenereMem(fileName, STR_LANGNAME, strlen(STR_LANGNAME) ,DECYPHER);
	if (decodedStr == NULL) {
		return DECODER_ERROR;
	}
	for (int i =0 ; i < strlen(decodedStr) ; i++) {
		char ch = decodedStr[i];
		if (ch =='\0') {
			continue;
		}
		count = readerAddChar(readerPointer, ch) != NULL ? count + 1 : count;
	}
	free(decodedStr);
	return count;

}

/*
***********************************************************
* Function name: readerRecover
* Purpose: Rewinds the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRecover(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	/* TO_DO: Adjust read and mark to zero */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR;
	}
	if (readerPointer->flags.isEmpty) {
		return False;
	}
	readerPointer->position.read = POSITION_INITIAL_POINT;
	readerPointer->position.mark = POSITION_INITIAL_POINT;
	return True;
}


/*
***********************************************************
* Function name: readerRetract
* Purpose: Retracts the buffer to put back the char in buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRetract(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR;
	}
	if (readerPointer->flags.isEmpty) {
		return False;
	}
	if (readerPointer->position.read > POSITION_INITIAL_POINT) {
		readerPointer->position.read--;
		return True;
	}
	return False;
}


/*
***********************************************************
* Function name: readerRestore
* Purpose: Resets the buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Boolean value about operation success
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
boolean readerRestore(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR;
	}
	if (readerPointer->flags.isEmpty) {
		return False;
	}
	readerPointer->position.read = readerPointer->position.mark;
	return True;
}



/*
***********************************************************
* Function name: readerGetChar
* Purpose: Returns the char in the getC position.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Char in the getC position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
char readerGetChar(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */

	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR;
	}
	if (readerPointer->flags.isEmpty || readerPointer->flags.isRead) {
		return '\0';
	}

	char val = readerPointer->content[++readerPointer->position.read];
	if (readerPointer->position.read == readerPointer->position.wrte) {
		readerPointer->flags.isRead = True;
	}
	return val;
}


/*
***********************************************************
* Function name: readerGetContent
* Purpose: Returns the pointer to String.
* Parameters:
*   readerPointer = pointer to Buffer Reader
*   pos = position to get the pointer
* Return value:
*	Position of string char.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
char* readerGetContent(BufferPointer const readerPointer, int pos) {
	/* Defensive programming */
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return NULL;
	}
	/* Check if pos is within valid range: 0 <= pos <= wrte */
	if (pos < 0 || pos > readerPointer->position.wrte) {
		return NULL;
	}
	return &(readerPointer->content[pos]);
}

/*
***********************************************************
* Function name: readerGetPosRead
* Purpose: Returns the value of getCPosition.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	The read position offset.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosRead(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->position.read;
}


/*
***********************************************************
* Function name: readerGetPosWrte
* Purpose: Returns the position of char to be added
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Write position
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosWrte(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->position.wrte;
}


/*
***********************************************************
* Function name: readerGetPosMark
* Purpose: Returns the position of mark in the buffer
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Mark position.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetPosMark(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->position.mark;
}


/*
***********************************************************
* Function name: readerGetSize
* Purpose: Returns the current buffer capacity
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Size of buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerGetSize(BufferPointer const readerPointer) {
	/* TO_DO: Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	/* TO_DO: Return size */
	return readerPointer->size;
}

/*
***********************************************************
* Function name: readerGetFlags
* Purpose: Returns the entire flags of Buffer.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Flags from Buffer.
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/
unsigned char readerGetFlags(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return 0;
	}
	unsigned char flags = 0;
	
	/* Handle mutually exclusive flags */
	if (readerPointer->flags.isEmpty) {
		flags |= 0x01;  // 0000 0001
	} else if (readerPointer->flags.isFull) {
		flags |= 0x02;  // 0000 0010
	}
	
	/* Independent flags */
	if (readerPointer->flags.isRead)  flags |= 0x04;  // 0000 0100
	if (readerPointer->flags.isMoved) flags |= 0x08;  // 0000 1000
	
	return flags;
}

/*
***********************************************************
* Function name: readerShowStat
* Purpose: Shows the char statistic.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value: (Void)
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
void readerPrintStat(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		printf("No statistics available.\n");
		return;
	}
	int count = 0;
	printf("Reader statistics :\n");
	for (int i = 0; i < NCHAR; i++) {
		if (readerPointer->histogram[i] > 0) {
			printf("B[%c]=%d, ", i, readerPointer->histogram[i]);
			count++;
			if (count % 10 == 0) {
				printf("\n");
			}
		}
	}

}

/*
***********************************************************
* Function name: readerNumErrors
* Purpose: Returns the number of errors found.
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	Number of errors.
* TO_DO:
*   - Use defensive programming
*	- Adjust for your LANGUAGE.
*************************************************************
*/
int readerNumErrors(BufferPointer const readerPointer) {
	/* Defensive programming */
	if (readerPointer == NULL) {
		return READER_ERROR;
	}
	return readerPointer->numReaderErrors;
}

/*
***********************************************************
* Function name: readerChecksum
* Purpose: Sets the checksum of the reader (4 bits).
* Parameters:
*   readerPointer = pointer to Buffer Reader
* Return value:
*	[None]
* TO_DO:
*   - Use defensive programming
*	- Check boundary conditions
*	- Adjust for your LANGUAGE.
*************************************************************
*/

int readerChecksum(BufferPointer const readerPointer) {
	int checksum = 0;
	if (readerPointer == NULL || readerPointer->content == NULL) {
		return READER_ERROR;
	}
	for (int i = 0; i <= readerPointer->position.wrte; i++) {
		checksum += readerPointer->content[i];
	}
	return checksum ;
}
