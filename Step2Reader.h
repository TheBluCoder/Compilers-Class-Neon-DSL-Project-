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
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: Jan 01 2025
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Constants about controls (not need to change) */
#define READER_ERROR				(-1)	/* General error message */
#define DECODER_ERROR				(-2)
#define POSITION_INITIAL_POINT		(-1)
#define READER_TERMINATOR			'\0'	/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME .................................. */

/* TO_DO: You should add your own constant definitions here */
#define READER_MAX_SIZE			0		/* maximum capacity */

#define READER_DEFAULT_SIZE		250		/* default initial buffer reader capacity */

#define NCHAR				    128		/* Chars from 0 to 127 */

#define CHARSEOF			    (-1)	/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	int read;			/* the offset to the get a char position (in chars) */
	int wrte;			/* the offset to the add chars (in chars) */
	int mark;			/* the offset to the mark position (in chars) */
} Position;


typedef struct flag {
	boolean isEmpty; /* checks if there is no content */
	boolean isFull; /* the content is using all size */
	boolean isRead; /* all content was read */
	boolean isMoved; /* the content was moved in reallocation */
} Flag;

/* Buffer structure */
typedef struct bufferReader {
	char*  content;			/* pointer to the beginning of character array (character buffer) */
	size_t	 size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	Flag flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position position;			/* Offset / position field */
	int		histogram[NCHAR];	/* Statistics of chars */
	int		numReaderErrors;	/* Number of errors from Reader */
	int		checkSum;			/* Sum of bytes(chars) */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */

/* General Operations */
BufferPointer	readerCreate		(int);
BufferPointer	readerAddChar		(BufferPointer const, char);
boolean		readerClear		    (BufferPointer const);
boolean		readerFree		    (BufferPointer const);
boolean		readerIsFull		(BufferPointer const);
boolean		readerIsEmpty		(BufferPointer const);
boolean		readerSetMark		(BufferPointer const, int);
int		readerPrint		    (BufferPointer const);
int		readerLoad			(BufferPointer const, char*    );
boolean		readerRecover		(BufferPointer const);
boolean		readerRetract		(BufferPointer const);
boolean		readerRestore		(BufferPointer const);
int		readerChecksum		(BufferPointer const);
/* Getters */
char		readerGetChar		(BufferPointer const);
char*    		readerGetContent	(BufferPointer const, int);
int		readerGetPosRead	(BufferPointer const);
int		readerGetPosWrte	(BufferPointer const);
int		readerGetPosMark	(BufferPointer const);
int		readerGetSize		(BufferPointer const);
unsigned char readerGetFlags		(BufferPointer const);
void		readerPrintStat		(BufferPointer const);
int		readerNumErrors		(BufferPointer const);

#endif
