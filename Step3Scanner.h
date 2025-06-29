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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2024
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Step2Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_
#endif


#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 6   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 17

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	INTL_T,		/*  1: Integer literal token */
	STR_T,		/*  2: String literal token */
	LPR_T,		/*  3: Left parenthesis token */
	RPR_T,		/*  4: Right parenthesis token */
	KW_T,		/*  5: Keyword token */
	EOS_T,		/*  6: End of statement (newline) */
	RTE_T,		/*  7: Run-time error token */
	SEOF_T,		/*  8: Source end-of-file token */
	CMT_T,		/*  9: Comment token */
	VID_T,		/* 10: Variable Id tokens */
	FMT_T, 		/* 11: Format Type Token */
	COM_T,		/* 12: Comma token */
	ASSIGN_T,	/* 13: Assignment token */
	COLON_T,	/* 14: Colon token */
	ARIT_OP_T,	/* 15: Arithmetic operator token */
	FLOAT_T,	/* 16: Floating-point literal token */
};

/* TO_DO: Define the list of keywords */
static char* tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"INTL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T",
	"VID_T",
	"FMT_T",
	"COM_T",
	"ASSIGN_T",
	"COLON_T",
	"ARIT_T",
	"FLOAT_T",
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

static char* aritOpStrTable[4] = {"+","-","*","/"};
/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int codeType;						/* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	int intValue;				/* integer literal attribute (value) */
	int keywordIndex;			/* keyword index in the keyword table */
	int FormatIndex;			/* Format token index in the format table*/
	int contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	double floatValue;				/* floating-point literal attribute (value) */
	char idLexeme[VID_LEN + 1];		/* variable identifier token attribute */
	char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttributes {
	unsigned char flags;			/* Flags information */
	union {
		int intValue;				/* Integer value */
		float floatValue;			/* Float value */
		char* stringContent;		/* String value */
	} values;
} IdAttributes;

/* Token declaration */
typedef struct Token {
	int code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttributes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	int scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* TO_DO: Define lexeme FIXED classes */
/* EOF definitions */
#define EOS_CHR '\0'	// CH00
#define EOF_CHR 0xFF	// CH01
#define UND_CHR '_'		// CH02
#define QUT_CHR '"'		// CH03
#define HST_CHR '#'		// CH04
#define TAB_CHR '\t'	// CH05
#define SPC_CHR ' '		// CH06
#define NWL_CHR '\n'	// CH07
#define LPR_CHR '('		// CH08
#define RPR_CHR ')'		// CH09
#define COM_CHR ','		// CH10
#define ASSIGN_CHR '='	// CH11
#define COLON_CHR ':'	// CH12
#define PLUS_CHR '+'	// CH13
#define MINUS_CHR '-'	// CH14
#define MUL_CHR '*'		// CH15
#define DIV_CHR '/'		// CH16
#define MOD_CHR '%'		// CH17
#define POINT_CHR '.'   // CH18


/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */


/* TO_DO: Error states and illegal state */
#define ESNR	9		/* Error state with no retract */
#define ESWR	10		/* Error state with retract */
#define FS		11		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		11
#define CHAR_CLASSES	8

/* TO_DO: Transition table - type of states defined in separate table */
static int  transitionTable[NUM_STATES][CHAR_CLASSES] = {
	/*    [A-z],[0-9],    _,   \', SEOF,    #, other, .
		   L(0), D(1), U(2), Q(3), E(4), C(5), O(6), FLOAT(7) */
	{      1,    7, ESNR,    3, ESWR,    5, ESNR,    ESNR},	// S0: NOAS
	{      1, ESNR,    1, ESNR,    2,    2,    2,    ESNR},	// S1: NOAS
	{     FS,   FS,   FS,   FS,   FS,   FS,   FS,      FS},	// S2: ASWR (KEY)
	{      3,    3,    3,    4, ESWR,	3,    3,       3},	// S3: NOAS
	{     FS,   FS,   FS,   FS,   FS,   FS,   FS,      FS},	// S4: ASNR (SL)
	{      5,    5,    5,    5,    6,	5,    5,       5},	// S5: NOAS
	{     FS,   FS,   FS,   FS,   FS,   FS,   FS,      FS},	// S6: ASNR (COM)
	{   ESNR,    7, ESNR, ESNR,    8,    8,	  8,       8}, // S7: Reads digits, may go to float on dot
	{     FS,    8,   FS,   FS,   FS,   FS,   FS,     ESNR},  // S8: Accepts digit after dot, floats ASWR
	{     FS,   FS,   FS,   FS,   FS,   FS,   FS,      FS},	// S9: ASNR (ES)
	{     FS,   FS,   FS,   FS,   FS,   FS,   FS,      FS},  // S10: ASWR (ER)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static int stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSWR, /* 02 (KEY) */
	NOFS, /* 03 */
	FSNR, /* 04 (SL) */
	NOFS, /* 05 */
	FSNR, /* 06(COM) */
	NOFS,/* 07 */
	FSWR,  /* 08 (INTL & FlOATS) */
	FSNR,  /* 07 (Err1 - no retract) */
	FSWR, /* 10 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function prototypes */
int			startScanner(BufferPointer psc_buf);
static int	nextClass(char c);					/* character class function */
static int	nextState(int, char);		/* state machine function */
void			printScannerData(ScannerData scData);
Token				tokenizer(void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(char* lexeme);

/* Declare accepting states functions */
Token funcSL	(char* lexeme); // returns String Literal token
Token funcNUM	(char* lexeme); // returns integer Literal token
Token funcID	(char* lexeme); //returns ID token
Token funcCMT   (char* lexeme);
Token funcKEY	(char* lexeme);
Token funcErr	(char* lexeme);
Token funcFMT   (char* lexeme);
// Token funcNUM   (char* lexeme);

/* Declaring accepting state function helpers */
int findKeywordIndex(char* lexeme);
int findFormatIndex(char* lexeme);
Token getToken(char* lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -    [00] */
	NULL,		/* -    [01] */
	getToken,	/*		[02] */  //key, id, FMT tokens
	NULL,		/*		[03] */
	funcSL,		/* SL   [04] */
	NULL,		/* -    [05] */
	funcCMT,	/* COM  [06] */
	NULL,		/*      [07] */
	funcNUM,	/* INT_L and FLOAT [08] */
	funcErr,	/* ERR1 [09] */
	funcErr,   /* ERR2 [10] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 24

/* TO_DO: Define the list of keywords */
static char* keywordTable[KWT_SIZE] = {
	"load",		/* KW00 */
	"model",		/* KW01 */
	"using",		/* KW02 */
	"ask",		/* KW03 */
	"generate",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"doc",		/* KW09 */
	"cache", 	/* KW10 */
	"save", 	/* KW11 */
	"stream_in", 	/* KW12 */
	"stream_out", 	/* KW13 */
	"input", 	/* KW14 */
	"output", 	/* KW15 */
	"True", 	/* KW16 */
	"False", 	/* KW17 */
	"break", 	/* KW18 */
	"image",	/* KW19 */
	"text",		/* KW20 */
	"media",		/* KW21 */
	"number",	/* KW22 */
	"bigNumber",/* KW23 */
};

/* Defines the number of Formats from the language */
#define FMT_SIZE 11
static char* formatTable[FMT_SIZE] = {
	"JPG",
	"PNG",
	"JPEG",
	"WAV",
	"MP4",
	"DOCX",
	"PDF",
	"MP3",
	"OGG",
	"TXT",
	"UNKNOWN"
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT TAB_CHR  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	char indentationCharType;
	int indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
// int numScannerErrors;

/* Scanner data */
extern ScannerData scData;


