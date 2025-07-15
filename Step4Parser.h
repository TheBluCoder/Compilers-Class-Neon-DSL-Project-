/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2024
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
#
# "=---------------------------------------="
# "|  COMPILERS - ALGONQUIN COLLEGE (F24)  |"
# "=---------------------------------------="
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# "    @@                             @@    �
# "    @@           %&@@@@@@@@@@@     @@    �
# "    @@       @%% (@@@@@@@@@  @     @@    �
# "    @@      @& @   @ @       @     @@    �
# "    @@     @ @ %  / /   @@@@@@     @@    �
# "    @@      & @ @  @@              @@    �
# "    @@       @/ @*@ @ @   @        @@    �
# "    @@           @@@@  @@ @ @      @@    �
# "    @@            /@@    @@@ @     @@    �
# "    @@     @      / /     @@ @     @@    �
# "    @@     @ @@   /@/   @@@ @      @@    �
# "    @@     @@@@@@@@@@@@@@@         @@    �
# "    @@                             @@    �
# "    @@         S O F I A           @@    �
# "    @@                             @@    �
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    �
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Parser (.h)
************************************************************
*/

#ifndef PARSER_H_
#define PARSER_H_

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif
#ifndef READER_H_
#include "Reader.h"
#endif
#ifndef SCANNER_H_
#include "Scanner.h"
#endif

/* Global vars */
static Token			lookahead;
extern BufferPointer	stringLiteralTable;
extern int		line;
extern Token			tokenizer(void);
extern char*		keywordTable[KWT_SIZE];
static int		syntaxErrorNumber = 0;

#define LANG_WRTE		"print&"
#define LANG_READ		"input&"
#define LANG_MAIN		"main&"

/* TO_DO: Create ALL constants for keywords (sequence given in table.h) */

/* Constants */
enum KEYWORDS {
	NO_ATTR = -1,
	KW_load,		/* 00 */
	KW_model,		/* 01 */
	KW_using,		/* 02 */
	KW_ask,			/* 03 */
	KW_generate,	/* 04 */
	KW_if,			/* 05 */
	KW_then,		/* 06 */
	KW_else,		/* 07 */
	KW_while,		/* 08 */
	KW_doc,			/* 09 */
	KW_cache,		/* 10 */
	KW_save,		/* 11 */
	KW_stream_in,	/* 12 */
	KW_stream_out,	/* 13 */
	KW_input,		/* 14 */
	KW_output,		/* 15 */
	KW_True,		/* 16 */
	KW_False,		/* 17 */
	KW_break,		/* 18 */
	KW_image,		/* 19 */
	KW_text,		/* 20 */
	KW_media,		/* 21 */
	KW_number,		/* 22 */
	KW_bigNumber,  /* 23 */
	KW_audio,		/*24 */
	KW_console,		/*25*/
	KW_file,		/* 26 */
	KW_embedding,  /* 27 */
	KW_embed,		/* 28 */
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 18

/* Parser */
typedef struct parserData {
	int parsHistogram[NUM_BNF_RULES];	/* Number of BNF Statements */
} ParserData, * pParsData;

/* Number of errors */
extern int numParserErrors;

/* Scanner data */
extern ParserData psData;

/* Function definitions */
void startParser();
void matchToken(int, int);
void syncErrorHandler(int);
void printError();
void printBNFData(ParserData psData);


enum BNF_RULES {
	BNF_,				                /* 00: Error rule */
	BNF_program,		                /* 01 <program> ::= { <statement> } */


	BNF_statements,		                /* 02 <statement> ::= <assignment>
	                                    					 | <assignment>
	                                    					 | <model_context_block>
	                                    					 | <stream_expr>
	                                    					 | <cache_statement>
	                                    					 | <save_statement>
	                                    					 | <load_model> */

	BNF_assignment,                    /*  03 <declaration> ::= <type> <id> "=" <load_expr>
	                                  						 | <type> <id> { "," <id> } "=" <multi_load_expr> */

	BNF_load_expr,                      /* 04 <load_expr> ::= "file" "(" <string> ")"
	                                  						 | "image" "(" <string> "," <img_format> ")"
	                                  						 | "audio" "(" <string> "," <audio_format> ")"
	                                  						 | "doc" "(" <string> "," <doc_format> ")"
	                                  						 | "media" "(" <string> "," <doc_format> ")" */

	BNF_model_context_block,            /* 05 <model_context_block> ::= "using" <id> ":" <model_context_body> */

	BNF_model_context_body,             /* 06 <model_context_body> ::= { <static assignment> | <operation_expr> | <dynamic assignment>
															 | <cache statement>   | <save statement> } */

	BNF_operation_expr,                 /* 07 <operation_expr> ::= "ask" "(" <string> [ "," <id> ] ")"
	                                  						 | "generate" "(" <string> [ "," <id> ] ")" */

	BNF_cache_statement,                /* 08 <cache_statement> ::= "cache" "(" <id> ")"
	                                  						 | "cache" "(" <id> "," <id> ")" */

	BNF_save_statement,                 /* 09 <save_statement> ::= "save" "(" <id> "," <string> ")" */

	BNF_stream_expr,                    /* 10 <stream_expr> ::= "stream_in" "(" <source_expr> "," <format> ")"
	                                  						 | "stream_out" "(" <id> "," <dest_expr> ")" */

	BNF_load_model,                     /* 11 <load_model> ::= "load" "model" <id> */

	BNF_source_expr,                    /* 12 <source_expr> ::= <id> | <string> */

	BNF_dest_expr,                      /* 13 <dest_expr> ::= "console" | <string> */

	BNF_type,                           /* 14 <type> ::= "text" | "image" | "audio" | "doc" | "media" | "stream" | "embedding" */

	BNF_format,                         /* 15 <format> ::= <img_format> | <audio_format> | <doc_format> */

	BNF_id,                             /* 16 <id> ::= valid identifier (excluding reserved keywords) */

	BNF_comment,                        /* 17 <comment> ::= "#" { any character }  "#" */
};



/* TO_DO: Define the list of keywords */
static char* BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",					/* 00 */
	"BNF_program",					/* 01 */
	"BNF_statements",				/* 02 */
	"BNF_assignment",				/* 03 */
	"BNF_load_expr",				/* 04 */
	"BNF_model_context_block",		/* 05 */
	"BNF_model_context_body",		/* 06 */
	"BNF_operation_expr",			/* 07 */
	"BNF_cache_statement",			/* 08 */
	"BNF_save_statement",			/* 09 */
	"BNF_stream_expr",				/* 10 */
	"BNF_load_model",
	"BNF_source_expr",
	"BNF_dest_expr",
	"BNF_type",
	"BNF_format",
	"BNF_id",
	"BNF_comment",
};

/* TO_DO: Place ALL non-terminal function declarations */
void program();
void statements();
void comment();
void assignment();
void load_expr();
void model_context_block();
void model_context_body();
void operation_expr();
void cache_statement();
void save_statement();
void stream_expr();
void load_model();
void source_expr();
void dest_expr();
void type();
void format();
void id();


boolean isDataType(TokenAttribute t) ;
#endif
