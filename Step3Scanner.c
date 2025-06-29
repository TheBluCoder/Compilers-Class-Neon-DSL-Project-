/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# ECHO "=---------------------------------------="
# ECHO "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
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
* File name: Scanner.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file contains all functionalities from Scanner.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/* #define NDEBUG to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef BUFFER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

/*
----------------------------------------------------------------
TO_DO: Global vars definitions
----------------------------------------------------------------
*/

/* Global objects - variables */
/* This buffer is used as a repository for string literals. */
extern BufferPointer stringLiteralTable;	/* String literal table */
int line;								/* Current line number of the source code */
extern int errorNumber;				/* Defined in platy_st.c - run-time error number */

/* Local(file) global objects - variables */
static BufferPointer lexemeBuffer;			/* Pointer to temporary lexeme buffer */
static BufferPointer sourceBuffer;			/* Pointer to input source buffer */

/* Define the scanner data structure */
ScannerData scData = {0};  /* Add this line */

/*
 ************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ***********************************************************
 */
 /* TO_DO: Follow the standard and adjust datatypes */

int startScanner(BufferPointer psc_buf) {
	/* TO_DO: Start histogram */
	int i = 0;
	for (i=0; i<NUM_TOKENS;i++)
		scData.scanHistogram[i] = 0;
	/* Basic scanner initialization */
	/* in case the buffer has been read previously  */
	if (readerRecover(psc_buf) == READER_ERROR) return EXIT_FAILURE;
	readerClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/*
 ************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states 
 *		in the Transition Diagram).
 ***********************************************************
 */

Token tokenizer(void) {

	/* TO_DO: Follow the standard and adjust datatypes */

	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	char c;			/* input symbol */
	int state = 0;	/* initial state of the FSM */
	int lexStart;	/* start offset of a lexeme in the input char buffer (array) */
	int lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	int lexLength;	/* token length */
	int i;			/* counter */
	///char newc;		// new char

	/* Starting lexeme */
	char* lexeme;	/* lexeme (to check the function) */
	lexeme = (char*)malloc(VID_LEN * sizeof(char));
	if (!lexeme)
		return currentToken;
	lexeme[0] = EOS_CHR;

	/* Skip whitespace characters */
	do {
		c = readerGetChar(sourceBuffer);

		// TO_DO: Defensive programming
		if (c < 0 || c >= NCHAR) {
			free(lexeme);
			return currentToken;
		}

	} while (c == SPC_CHR || c == TAB_CHR);

	/* ------------------------------------------------------------------------
		Part 1: Implementation of token driven scanner.
		Every token is possessed by its own dedicated code
		-----------------------------------------------------------------------
	*/

	/* TO_DO: All patterns that do not require accepting functions */
	switch (c) {

	case NWL_CHR:
		line++;
		currentToken.code = EOS_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case LPR_CHR:
		currentToken.code = LPR_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case RPR_CHR:
		currentToken.code = RPR_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case COM_CHR:
		currentToken.code = COM_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;


	case COLON_CHR:
		currentToken.code = COLON_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case ASSIGN_CHR:
		currentToken.code = ASSIGN_T;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	/* Cases for Operators */
	case PLUS_CHR:
		currentToken.code= ARIT_OP_T;
		currentToken.attribute.arithmeticOperator = OP_ADD;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case MINUS_CHR:
		currentToken.code= ARIT_OP_T;
		currentToken.attribute.arithmeticOperator = OP_SUB;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case MUL_CHR:
		currentToken.code= ARIT_OP_T;
		currentToken.attribute.arithmeticOperator = OP_MUL;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	case DIV_CHR:
		currentToken.code= ARIT_OP_T;
		currentToken.attribute.arithmeticOperator = OP_DIV;
		scData.scanHistogram[currentToken.code]++;
		free(lexeme);
		return currentToken;

	/* Cases for END OF FILE */
	case EOS_CHR:
		currentToken.code = SEOF_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.seofType = SEOF_0;
		free(lexeme);
		return currentToken;



	case (char) EOF_CHR:
		currentToken.code = SEOF_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.seofType = SEOF_255;
		return currentToken;

	/* ------------------------------------------------------------------------
		Part 2: Implementation of Finite State Machine (DFA) or Transition Table driven Scanner
		Note: Part 2 must follow Part 1 to catch the illegal symbols
		-----------------------------------------------------------------------
	*/

	/* TO_DO: Adjust / check the logic for your language */

	default: // general case
		state = nextState(state, c);
		lexStart = readerGetPosRead(sourceBuffer);
		readerSetMark(sourceBuffer, lexStart);
		// int pos = 0;
		while (stateType[state] == NOFS) {
			c = readerGetChar(sourceBuffer);
			state = nextState(state, c);
			// pos++;
		}
		if (stateType[state] == FSWR)
			readerRetract(sourceBuffer);
		lexEnd = readerGetPosRead(sourceBuffer)+1;
		lexLength = lexEnd - lexStart;
		// lexemeBuffer = readerCreate((int)lexLength + 2);
		lexemeBuffer = readerCreate((int)lexLength + 1);
		if (!lexemeBuffer) {
			errorPrint("Scanner error: Can not create buffer\n");
			exit(EXIT_FAILURE);
		}
		readerRestore(sourceBuffer);
		for (i = 0; i < lexLength; i++)
			readerAddChar(lexemeBuffer, readerGetChar(sourceBuffer));
		readerAddChar(lexemeBuffer, READER_TERMINATOR);
		free(lexeme);
		lexeme = readerGetContent(lexemeBuffer, 0);
		// TO_DO: Defensive programming
		if (!lexeme) {
			return currentToken;
		}
		currentToken = (*finalStateTable[state])(lexeme);
		readerRestore(lexemeBuffer);
		free(lexeme);
		return currentToken;
	} // switch

} // tokenizer


/*
 ************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUG is used - see the top of the file.
 ***********************************************************
 */
 /* TO_DO: Just change the datatypes */

int nextState(int state, char c) {
	int col;
	int next;
	col = nextClass(c);
	next = transitionTable[state][col];
	if (DEBUG)
		printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
	assert(next != FS);
	if (DEBUG)
		if (next == FS) {
			printf("Scanner Error: Illegal state:\n");
			printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
			exit(1);
		}
	return next;
}

/*
 ************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
 ***********************************************************
 */
/* TO_DO: Use your column configuration */

/* Adjust the logic to return next column in TT */
/*    [A-z],[0-9],    _,    &,   \', SEOF,    #, other
	   L(0), D(1), U(2), M(3), Q(4), E(5), C(6),  O(7) */

int nextClass(char c) {
	int val = -1;
	switch (c) {
	case UND_CHR:
		val = 2;
		break;
	case QUT_CHR:
		val = 3;
		break;
	case HST_CHR:
		val = 5;
		break;
	case EOS_CHR:
	case (char) EOF_CHR:
		val = 4;
		break;
	case POINT_CHR:
		val=7;
		break;
	default:
		if (isalpha(c))
			val = 0;
		else if (isdigit(c))
			val = 1;
		else
			val = 6;
	}
	return val;
}

/*
 ************************************************************
 * Acceptance State Function COM
 *		Function responsible to identify COM (comments).
 ***********************************************************
 */
 /* TO_DO: Adjust the function for IL */

Token funcCMT(char* lexeme) {
	Token currentToken = { 0 };
	// int i = 0, len = (int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable);
	// for (i = 1; i < len - 1; i++) {
	// 	if (lexeme[i] == NWL_CHR)
	// 		line++;
	// }
	currentToken.code = CMT_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


 /*
  ************************************************************
  * Acceptance State Function IL
  *		Function responsible to identify IL (integer literals).
  * - It is necessary respect the limit (ex: 2-byte integer in C).
  * - In the case of larger lexemes, error shoul be returned.
  * - Only first ERR_LEN characters are accepted and eventually,
  *   additional three dots (...) should be put in the output.
  ***********************************************************
  */
  /* TO_DO: Adjust the function for IL */

Token funcIL(char* lexeme) {
	Token currentToken = { 0 };
	long tlong;
	if (lexeme[0] != EOS_CHR && strlen(lexeme) > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
	}
	else {
		tlong = atol(lexeme);
		if (tlong >= 0 && tlong <= SHRT_MAX) {
			currentToken.code = INTL_T;
			scData.scanHistogram[currentToken.code]++;
			currentToken.attribute.intValue = (int)tlong;
		}
		else {
			currentToken = (*finalStateTable[ESNR])(lexeme);
		}
	}
	return currentToken;
}


/*
 ************************************************************
 * Acceptance State Function ID
 *		In this function, the pattern for IDs must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for ID */

Token funcID(char* lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);

	if (length > NUM_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
		return currentToken;
	}

	currentToken.
	return currentToken;
}

Token funcFLOAT(char* lexeme) {
	Token currentToken = { 0 };
	size_t length = strlen(lexeme);

	if (length > VID_LEN) {
		currentToken = (*finalStateTable[ESNR])(lexeme);
		return currentToken;
	}

	strncpy(currentToken.attribute.idLexeme, lexeme, VID_LEN);
	currentToken.code = VID_T;
	currentToken.attribute.idLexeme[VID_LEN] = EOS_CHR;
	return currentToken;
}

Token getToken(char* lexeme) {
	if (findKeywordIndex(lexeme) != -1) {
		return funcKEY(lexeme);
	}
	else if (findFormatIndex(lexeme) != -1) {
		return funcFMT(lexeme);
	}
	else {
		return funcID(lexeme);
	}
}


/*
************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table 
 *   (stringLiteralTable). You need to include the literals in 
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ***********************************************************
 */

Token funcSL(char* lexeme) {
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	currentToken.attribute.contentString = readerGetPosWrte(stringLiteralTable)+1;
	for (i = 1; i < len - 1; i++) {
		if (lexeme[i] == NWL_CHR)
			line++;
		if (!readerAddChar(stringLiteralTable, lexeme[i])) {
			currentToken.code = RTE_T;
			scData.scanHistogram[currentToken.code]++;
			strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
			errorNumber = RTE_CODE;
			return currentToken;
		}
	}
	if (!readerAddChar(stringLiteralTable, EOS_CHR)) {
		currentToken.code = RTE_T;
		scData.scanHistogram[currentToken.code]++;
		strcpy(currentToken.attribute.errLexeme, "Run Time Error:");
		errorNumber = RTE_CODE;
		return currentToken;
	}
	currentToken.code = STR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a keyword.
 * - Tip: Remember to use the keywordTable to check the keywords.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Keywords */

Token funcKEY(char* lexeme) {
	Token currentToken = { 0 };
	const int kwIndex = findKeywordIndex(lexeme);
	if (kwIndex != -1) {
		currentToken.code = KW_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = kwIndex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * This function checks if one specific lexeme is a Format Type Token.
 ***********************************************************
 */
Token funcFMT(char* lexeme) {
	Token currentToken = { 0 };
	const int fmIndex = findFormatIndex(lexeme);
	if (fmIndex != -1) {
		currentToken.code = FMT_T;
		scData.scanHistogram[currentToken.code]++;
		currentToken.attribute.codeType = fmIndex;
	}
	else {
		currentToken = funcErr(lexeme);
	}
	return currentToken;
}


/*
************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ***********************************************************
 */
 /* TO_DO: Adjust the function for Errors */

Token funcErr(char* lexeme) {
	Token currentToken = { 0 };
	int i = 0, len = (int)strlen(lexeme);
	if (len > ERR_LEN) {
		strncpy(currentToken.attribute.errLexeme, lexeme, ERR_LEN - 3);
		currentToken.attribute.errLexeme[ERR_LEN - 3] = EOS_CHR;
		strcat(currentToken.attribute.errLexeme, "...");
	}
	else {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (i = 0; i < len; i++)
		if (lexeme[i] == NWL_CHR)
			line++;
	currentToken.code = ERR_T;
	scData.scanHistogram[currentToken.code]++;
	return currentToken;
}

int findKeywordIndex(char* lexeme) {
	for (int i = 0; i < KWT_SIZE; i++) {
		if (strcmp(lexeme, keywordTable[i]) == 0) {
			return i;  // Return index if found
		}
	}
	return -1;  // Return -1 if not found
}

// Helper function to check if lexeme is a format token
int findFormatIndex(char* lexeme) {
	for (int i = 0; i < FMT_SIZE; i++) {
		if (strcmp(lexeme, formatTable[i]) == 0) {
			return i;  // Return index if found
		}
	}
	return -1;  // Return -1 if not found
}



/*
 ************************************************************
 * The function prints the token returned by the scanner
 ***********************************************************
 */

void printToken(Token t) {
	extern char* keywordTable[]; /* link to keyword table in */
	switch (t.code) {
	case RTE_T:
		printf("RTE_T\t\t%s", t.attribute.errLexeme);
		/* Call here run-time error handling component */
		if (errorNumber) {
			printf("%d", errorNumber);
			exit(errorNumber);
		}
		printf("\n");
		break;
	case ERR_T:
		printf("ERR_T\t\t%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case STR_T:
		printf("STR_T\t\t%d\t ", (int)t.attribute.contentString);
		printf("%s\n", readerGetContent(stringLiteralTable, (int)t.attribute.contentString));
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case KW_T:
		printf("KW_T\t\t%s\n", keywordTable[t.attribute.codeType]);
		break;
	case CMT_T:
		printf("CMT_T\n");
		break;
	case EOS_T:
		printf("EOS_T\n");
		break;
	case FMT_T:
		printf("FMT_T\t\t%s\n", formatTable[t.attribute.codeType]);
		break;
	case VID_T:
		printf("VID_T\t\t%s\n", t.attribute.idLexeme);
		break;
	case COM_T:
		printf("COM_T\n");
		break;
	case ASSIGN_T:
		printf("ASSIGN_T\n");
		break;
	case COLON_T:
		printf("COLON_T\n");
		break;

	case ARIT_OP_T:
		printf("ARIT_OP_T\t\t%s\n", aritOpStrTable[t.attribute.arithmeticOperator]);
		break;

	default:
		printf("Scanner error: invalid token code: %d\n", t.code);
	}
}

/*
 ************************************************************
 * The function prints statistics of tokens
 * Param:
 *	- Scanner data
 * Return:
 *	- Void (procedure)
 ***********************************************************
 */
void printScannerData(ScannerData scData) {
	/* Print Scanner statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_TOKENS; cont++) {
		if (scData.scanHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", tokenStrTable[cont], "]=", scData.scanHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
TO_DO: (If necessary): HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
*/
