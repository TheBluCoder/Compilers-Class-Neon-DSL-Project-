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
	KW_number,
	KW_bigNumber,
	KW_audio,
	KW_console,
	KW_file,
	KW_embedding,
};

/* TO_DO: Define the number of BNF rules */
#define NUM_BNF_RULES 25

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

/* List of BNF statements */
// enum BNF_RULES {
// 	BNF_error,										/*  0: Error token */
// 	BNF_codeSession,								/*  1 */
// 	BNF_comment,									/*  2 */
// 	BNF_dataSession,								/*  3 */
// 	BNF_optVarListDeclarations,						/*  4 */
// 	BNF_optionalStatements,							/*  5 */
// 	BNF_outputStatement,							/*  6 */
// 	BNF_outputVariableList,							/*  7 */
// 	BNF_program,									/*  8 */
// 	BNF_statement,									/*  9 */
// 	BNF_statements,									/* 10 */
// 	BNF_statementsPrime,							/* 11 */
// 	BNF_optParams,									/* 12 */
// 	BNF_returnStatement								/* 13 */
// };


enum BNF_RULES {
	BNF_,				                /*  0: Error rule */
	BNF_program,		                /* <program> ::= { <statement> } */


	BNF_statement,		                /* <statement> ::= <static_assignment>
	                                    				 | <assignment>
	                                    				 | <model_block>
	                                    				 | <stream_statement>
	                                    				 | <cache_statement>
	                                    				 | <save_statement>
	                                    				 | <load_model> */

	BNF_static_assignment,                    /* <declaration> ::= <type> <id> "=" <load_expr>
	                                  						 | <type> <id> { "," <id> } "=" <multi_load_expr> */

	BNF_dynamic_assignment,                     /* <assignment> ::= <type> <id> "=" <operation_expr> */

	BNF_load_expr,                      /* <load_expr> ::= "file" "(" <string> ")"
	                                  						 | "image" "(" <string> "," <img_format> ")"
	                                  						 | "audio" "(" <string> "," <audio_format> ")"
	                                  						 | "doc" "(" <string> "," <doc_format> ")"
	                                  						 | "media" "(" <string> "," <doc_format> ")" */

	BNF_multi_load_expr,                /* <multi_load_expr> ::= "media" "(" <string> "," <doc_format> ")" */

	BNF_model_block,                    /* <model_block> ::= "using" <id> ":" <model_body> */

	BNF_model_body,                     /* <model_body> ::= { <static assignment> | <operation_expr> | <dynamic assignment> } */

	// BNF_context_block,                  /* <context_block> ::= "using" <id> ":" <operation_expr> */

	BNF_operation_expr,                 /* <operation_expr> ::= "ask" "(" <string> [ "," <id> ] ")"
	                                  						 | "generate" "(" <string> [ "," <id> ] ")" */

	BNF_cache_statement,                /* <cache_statement> ::= "cache" "(" <id> ")"
	                                  						 | "cache" "(" <id> "," <id> ")" */

	BNF_save_statement,                 /* <save_statement> ::= "save" "(" <id> "," <string> ")" */

	BNF_stream_statement,               /* <stream_statement> ::= "stream_in" "(" <source_expr> "," <format> ")"
	                                  						 | "stream_out" "(" <id> "," <dest_expr> ")" */

	BNF_load_model,                     /* <load_model> ::= "load" "model" <id> */

	BNF_source_expr,                    /* <source_expr> ::= <id> | <string> */

	BNF_dest_expr,                      /* <dest_expr> ::= "console" | <string> */

	BNF_type,                           /* <type> ::= "text" | "image" | "audio" | "doc" | "media" | "stream" | "embedding" */

	BNF_format,                         /* <format> ::= <img_format> | <audio_format> | <doc_format> */

	BNF_img_format,                     /* <img_format> ::= "PNG" | "JPEG" | "JPG" */

	BNF_audio_format,                   /* <audio_format> ::= "WAV" | "MP3" */

	BNF_doc_format,                     /* <doc_format> ::= "PDF" | "DOCX" */

	BNF_id,                             /* <id> ::= valid identifier (excluding reserved keywords) */

	BNF_text   ,                         /* <text> ::= string literal (e.g., "Hello world") */

	BNF_comment,                        /* <comment> ::= "#" { any character }  "#" */
};



/* TO_DO: Define the list of keywords */
static char* BNFStrTable[NUM_BNF_RULES] = {
	"BNF_error",
	"BNF_program",
	"BNF_statement",
	"BNF_static_assignment",
	"BNF_dynamic_assignment",
	"BNF_load_expr",
	"BNF_multi_load_expr",
	"BNF_model_block",
	"BNF_model_body",
	// "BNF_context_block",
	"BNF_operation_expr",
	"BNF_cache_statement",
	"BNF_save_statement",
	"BNF_stream_statement",
	"BNF_load_model",
	"BNF_source_expr",
	"BNF_dest_expr",
	"BNF_type",
	"BNF_format",
	"BNF_img_format",
	"BNF_audio_format",
	"BNF_doc_format",
	"BNF_id",
	"BNF_text",
	"BNF_comment",
};

/* TO_DO: Place ALL non-terminal function declarations */
void program();
void statement();
void comment();
void static_assignment();
void dynamic_assignment();
void load_expr();
void multi_load_expr();
void model_block();
void model_body();
// void context_block();
void operation_expr();
void cache_statement();
void save_statement();
void stream_statement();
void load_model();
void source_expr();
void dest_expr();
void type();
void format();
void img_format();
void audio_format();
void id();


boolean isDataType(TokenAttribute t) ;
#endif
