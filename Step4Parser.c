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
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "    @@                             @@    ”
# "    @@           %&@@@@@@@@@@@     @@    ”
# "    @@       @%% (@@@@@@@@@  @     @@    ”
# "    @@      @& @   @ @       @     @@    ”
# "    @@     @ @ %  / /   @@@@@@     @@    ”
# "    @@      & @ @  @@              @@    ”
# "    @@       @/ @*@ @ @   @        @@    ”
# "    @@           @@@@  @@ @ @      @@    ”
# "    @@            /@@    @@@ @     @@    ”
# "    @@     @      / /     @@ @     @@    ”
# "    @@     @ @@   /@/   @@@ @      @@    ”
# "    @@     @@@@@@@@@@@@@@@         @@    ”
# "    @@                             @@    ”
# "    @@         S O F I A           @@    ”
# "    @@                             @@    ”
# "    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@    ”
# "                                         "
# "[READER SCRIPT .........................]"
# "                                         "
*/


/*
************************************************************
* File name: Parser.c
* Compiler: MS Visual Studio 2022
* Course: CST 8152 – Compilers, Lab Section: [011, 012]
* Assignment: A32.
* Date: May 01 2023
* Purpose: This file contains all functionalities from Parser.
* Function list: (...).
************************************************************
*/

/* TO_DO: Adjust the function header */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef SCANNER_H_
#include "Step3Scanner.h"
#endif

#ifndef PARSER_H_
#include "Step4Parser.h"
#endif


/* BNF statistics */

/*
************************************************************
 * Process Parser
 ***********************************************************
 */
/* TO_DO: This is the function to start the parser - check your program definition */

void startParser() {
	/* TO_DO: Initialize Parser data */
	int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	/* Proceed parser */
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program();
	}
	matchToken(SEOF_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
		case KW_T:
			if (lookahead.attribute.codeType != tokenAttribute)
				matchFlag = 0;
			break;
		case VID_T:
				if (strlen(lookahead.attribute.idLexeme) < 1) {
					matchFlag = 0;
				}
				break;
		case FMT_T:
		default:
			if (lookahead.code != tokenCode)
				matchFlag = 0;
			break;
	}
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode);
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
void syncErrorHandler(int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T)
			exit(syntaxErrorNumber);
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T)
		lookahead = tokenizer();
}

/*
 ************************************************************
 * Print Error
 ***********************************************************
 */
/* TO_DO: This is the function to error printing - adjust basically datatypes */
void printError() {
	// extern int numParserErrors;			/* link to number of errors (defined in Parser.h) */
	Token t = lookahead;
	printf("%s%s%3d\n", STR_LANGNAME, ": Syntax error:  Line:", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("*ERROR*: %s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("SEOF_T\t\t%d\t\n", t.attribute.seofType);
		break;
	case STR_T:
		printf("STR_T: %s\n", readerGetContent(stringLiteralTable, t.attribute.contentString));
		break;
	case KW_T:
		printf("KW_T: %s\n", keywordTable[t.attribute.codeType]);
		break;
	case LPR_T:
		printf("LPR_T\n");
		break;
	case RPR_T:
		printf("RPR_T\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	case COM_T:
		printf("COMMA\n");
		break;
	case VID_T:
		printf("VID_T: %s\n", t.attribute.idLexeme);
		break;
	case FMT_T:
		printf("FMT_T: %s\n", formatTable[t.attribute.formatIndex]);
		break;
	case TAB_T:
		printf("TAB_T\n");
	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
 ************************************************************
 * Program statement
 * BNF: <program> -> main& { <opt_statements> }
 * FIRST(<program>)= {CMT_T, MNID_T (main&), SEOF_T}.
 ***********************************************************
 */
void program() {
	psData.parsHistogram[BNF_program]++;
	switch (lookahead.code) {
	case CMT_T:
		while (lookahead.code == CMT_T) {
			comment();
		}
	case KW_T:
		statements();
	case SEOF_T:
		; // Empty
		break;
	default:
		syncErrorHandler(lookahead.code);
	}
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
 ************************************************************
 * comment
 * BNF: comment
 * FIRST(<comment>)= {CMT_T}.
 ***********************************************************
 */
void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

void statements() {
	psData.parsHistogram[BNF_statements]++;

	while (lookahead.code != SEOF_T ) {
		if (lookahead.code == EOS_T) {
			matchToken(EOS_T, NO_ATTR);
			continue;
		}
		switch (lookahead.attribute.codeType) {
			case KW_load:
				load_model();
				break;
			case KW_stream_in:
			case KW_stream_out:
				stream_expr();
				break;
			case KW_image:
			case KW_text:
			case KW_audio:
			case KW_file:
			case KW_media:
			case KW_doc:
			case KW_embedding:
			case KW_number:
			case KW_bigNumber:
				assignment();
				break;
			case KW_using:
				model_context_block();
				break;
			default:
				syncErrorHandler(lookahead.code);
			}
	}
	if (lookahead.code != SEOF_T) {
		matchToken(EOS_T, NO_ATTR);
	}
	printf("%s%s\n", STR_LANGNAME, ": Statement parsed");
}

void printBNFData(ParserData psData) {
	/* Print Parser statistics */
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

void load_model() {
	psData.parsHistogram[BNF_load_model]++;
	matchToken(KW_T, KW_load);
	matchToken(KW_T, KW_model);
	char *modelName = readerGetContent(stringLiteralTable, lookahead.attribute.contentString);
	matchToken(STR_T, NO_ATTR);
	printf("%s%s %s parsed\n", STR_LANGNAME, ": Load model ", modelName);
}

int isDatatype(const TokenAttribute t) {
	const char* datatypes[10] = {"text", "image", "audio", "embedding", "number", "bigNumber", "file","media","doc","embed"};
	for (int i = 0; i < 9; i++) {
		if (strcmp(datatypes[i], keywordTable[t.codeType]) == 0) {
			return t.codeType;
		}
	}
	return -1;
}

void stream_expr() {
	psData.parsHistogram[BNF_stream_expr]++;
	switch (lookahead.attribute.codeType) {
		case KW_stream_in:
			matchToken(KW_T, KW_stream_in);
			matchToken(LPR_T, NO_ATTR);
			source_expr();
			matchToken(COM_T, NO_ATTR);
			format();
			matchToken(RPR_T, NO_ATTR);
			break;
		case KW_stream_out:
			matchToken(KW_T, KW_stream_out);
			matchToken(LPR_T, NO_ATTR);
			id();
			matchToken(COM_T, NO_ATTR);
			dest_expr();
			matchToken(RPR_T, NO_ATTR);
			break;
		default:
			syncErrorHandler(lookahead.code);
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Stream expression parsed");
}

void assignment() {
	psData.parsHistogram[BNF_assignment]++;
	type();
	id();
	matchToken(ASSIGN_T, NO_ATTR);
	
	// Check if it's a load expression (static) or operation expression (dynamic)
	switch (lookahead.attribute.codeType) {
		case KW_doc:
		case KW_file:
		case KW_image:
		case KW_audio:
		case KW_media:
			load_expr();
			break;
		case KW_ask:
		case KW_generate:
		case KW_embed:
			operation_expr();
			break;
		case KW_stream_in:
		case KW_stream_out:
			stream_expr();
			break;
		default:
			syncErrorHandler(lookahead.code);
			break;
	}


	printf("%s%s\n", STR_LANGNAME, ": assignment statement parsed");
}
void model_context_block() {
	psData.parsHistogram[BNF_model_context_block]++;
	matchToken(KW_T, KW_using);
	matchToken(STR_T, NO_ATTR);
	matchToken(COLON_T, NO_ATTR);
	model_context_body();
	printf("%s%s\n", STR_LANGNAME, ": Model context block parsed");
}
void type() {
	psData.parsHistogram[BNF_type]++;
	matchToken(KW_T, isDatatype(lookahead.attribute));
}
void id() {
	psData.parsHistogram[BNF_id]++;
	matchToken(VID_T, NO_ATTR);
}
void load_expr() {
	psData.parsHistogram[BNF_load_expr]++;
	type();
	matchToken(LPR_T, NO_ATTR);
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR);
	}
	else if (lookahead.code == VID_T) {
		id();
	}
	else {
		syncErrorHandler(lookahead.code);
	}
	matchToken(COM_T, NO_ATTR);
	format();
	matchToken(RPR_T, NO_ATTR);
}


void format() {
	psData.parsHistogram[BNF_format]++;
	matchToken(FMT_T, NO_ATTR);
}

void model_context_body() {
	psData.parsHistogram[BNF_model_context_body]++;
	
	while (lookahead.code != SEOF_T  ) {
		if (lookahead.code == EOS_T) {
			matchToken(EOS_T, NO_ATTR);
			continue;
		}

		if (lookahead.code != TAB_T) {
			break;
		}
		matchToken(TAB_T, NO_ATTR);

		switch (lookahead.code) {
			case KW_T:
				switch (lookahead.attribute.codeType) {
					case KW_text:
					case KW_image:
					case KW_audio:
					case KW_embedding:
						// Static assignment: type id = operation_expr
						assignment();
						break;
					case KW_cache:
						cache_statement();
						break;
					case KW_save:
						save_statement();
						break;
					default:
						syncErrorHandler(lookahead.code);
						break;
				}
				break;
			default:
				syncErrorHandler(lookahead.code);
				break;
		}
	}
	printf("%s%s\n", STR_LANGNAME, ": Model context body parsed");
}

void cache_statement() {
	psData.parsHistogram[BNF_cache_statement]++;
	matchToken(KW_T, KW_cache);
	matchToken(LPR_T, NO_ATTR);
	id();
	matchToken(RPR_T, NO_ATTR);
	// matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Cache statement parsed");
}

void save_statement() {
	psData.parsHistogram[BNF_save_statement]++;
	matchToken(KW_T, KW_save);
	matchToken(LPR_T, NO_ATTR);
	id();
	matchToken(COM_T, NO_ATTR);
	matchToken(STR_T, NO_ATTR);
	matchToken(RPR_T, NO_ATTR);
	// matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Save statement parsed");
}

void operation_expr() {
	psData.parsHistogram[BNF_operation_expr]++;
	switch (lookahead.attribute.codeType) {
		case KW_ask:
		case KW_generate:
			matchToken(KW_T, lookahead.attribute.codeType);
			matchToken(LPR_T, NO_ATTR);
			matchToken(STR_T, NO_ATTR);
			if (lookahead.code == COM_T) {
				matchToken(COM_T, NO_ATTR);
				id();
			}
			matchToken(RPR_T, NO_ATTR);
			break;
		case KW_embed:
			matchToken(KW_T, KW_embed);
			matchToken(LPR_T, NO_ATTR);
			id();
			matchToken(RPR_T, NO_ATTR);
			break;
		default:
			syncErrorHandler(lookahead.code);
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Operation expression parsed");
}

void source_expr() {
	psData.parsHistogram[BNF_source_expr]++;
	switch (lookahead.code) {
		case VID_T:
			id();
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			break;
		default:
			syncErrorHandler(lookahead.code);
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Source expression parsed");
}

void dest_expr() {
	psData.parsHistogram[BNF_dest_expr]++;
	switch (lookahead.code) {
		case KW_T:
			if (lookahead.attribute.codeType == KW_console) {
				matchToken(KW_T, KW_console);
			} else {
				syncErrorHandler(lookahead.code);
			}
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR);
			break;
		case VID_T:
			matchToken(VID_T, NO_ATTR);
			break;
		default:
			syncErrorHandler(lookahead.code);
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Destination expression parsed");
}

