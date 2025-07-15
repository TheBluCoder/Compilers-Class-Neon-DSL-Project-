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
/*
************************************************************
* Function: startParser
* Purpose: Entry point for the parser. Initializes parser data and starts parsing the source file.
* BNF: (Entry point, not a direct BNF rule)
* Params: None
* Returns: void
************************************************************
*/
void startParser() {
	// Initialize parser histogram data
	int i = 0;
	for (i = 0; i < NUM_BNF_RULES; i++) {
		psData.parsHistogram[i] = 0;
	}
	// Get the first token
	lookahead = tokenizer();
	if (lookahead.code != SEOF_T) {
		program(); // Start parsing from <program>
	}
	matchToken(SEOF_T, NO_ATTR); // Ensure end of file is reached
	printf("%s%s\n", STR_LANGNAME, ": Source file parsed");
}


/*
 ************************************************************
 * Match Token
 ***********************************************************
 */
/* TO_DO: This is the main code for match - check your definition */
/*
************************************************************
* Function: matchToken
* Purpose : Matches the current lookahead token with the expected token code and attribute.
*           Advances the lookahead if matched, otherwise calls error handler.
* BNF     : (Token matching utility, not a direct BNF rule)
* Params  : tokenCode - expected token code
*           tokenAttribute - expected token attribute
* Returns : void
************************************************************
*/
void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	// Check if lookahead matches expected token and attribute
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
	// If matched and at end of file, return
	if (matchFlag && lookahead.code == SEOF_T)
		return;
	if (matchFlag) {
		lookahead = tokenizer(); // Advance to next token
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else
		syncErrorHandler(tokenCode); // Handle mismatch
}

/*
 ************************************************************
 * Syncronize Error Handler
 ***********************************************************
 */
/* TO_DO: This is the function to handler error - adjust basically datatypes */
/*
************************************************************
* Function: syncErrorHandler
* Purpose : Handles syntax errors by synchronizing to the next expected token.
* BNF     : (Error recovery utility, not a direct BNF rule)
* Params  : syncTokenCode - token code to synchronize to
* Returns : void
************************************************************
*/
void syncErrorHandler(int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	// Skip tokens until syncTokenCode or end of file
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
/*
************************************************************
* Function: printError
* Purpose : Prints a syntax error message for the current lookahead token.
* BNF     : (Error reporting utility, not a direct BNF rule)
* Params  : None
* Returns : void
************************************************************
*/
void printError() {
	// extern int numParserErrors; // link to number of errors (defined in Parser.h)
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
	case INTL_T:
		printf("INTL_T: %d\n", lookahead.attribute.intValue);
		break;
	case FLOAT_T:
		printf("FLOAT_T: %f\n", lookahead.attribute.floatValue);
		break;

	default:
		printf("%s%s%d\n", STR_LANGNAME, ": Scanner error: invalid token code: ", t.code);
		numParserErrors++; // Updated parser error
	}
}

/*
************************************************************
* Function: program
* Purpose : Parses the <program> non-terminal according to the BNF.
*           Handles comments, statements, and end of file.
* BNF     : <program> -> { <statements> }
* Params  : None
* Returns : void
************************************************************
*/
void program() {
	psData.parsHistogram[BNF_program]++;
	statements(); // Parse statements
	printf("%s%s\n", STR_LANGNAME, ": Program parsed");
}

/*
************************************************************
* Function: comment
* Purpose : Parses a comment token.
* BNF     : <comment> -> CMT_T
*           FIRST(<comment>)= {CMT_T}
* Params  : None
* Returns : void
************************************************************
*/
void comment() {
	psData.parsHistogram[BNF_comment]++;
	matchToken(CMT_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Comment parsed");
}

/*
************************************************************
* Function: statements
* Purpose : Parses a sequence of statements until end of file.
* BNF     : <statements> -> { <assignment> | <stream_expr> | <cache_statement> | <save_statement>
*							| <load_model> | <EOS>         | <comment>         | <model_context_block>}
*           FIRST(<statements>) = {KW_T, ...}
* Params  : None
* Returns : void
************************************************************
*/
void statements() {
	psData.parsHistogram[BNF_statements]++;

	// Loop through all statements until end of file
	while (lookahead.code != SEOF_T ) {
		switch (lookahead.code) {
			case CMT_T:
				comment(); // Parse comment
				break;
			case EOS_T:
				matchToken(EOS_T, NO_ATTR); // Skip end of statement
				break;
			case KW_T:
				switch (lookahead.attribute.codeType) {
				case KW_load:
						load_model(); // Parse load model statement
						break;
				case KW_stream_in:
				case KW_stream_out:
						stream_expr(); // Parse stream expression
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
						assignment(); // Parse assignment
						break;
				case KW_using:
						model_context_block(); // Parse model context block
						break;
				default:
						syncErrorHandler(lookahead.code); // Handle unexpected token
				}
				break;
			default:
				syncErrorHandler(lookahead.code); // Handle unexpected token
				break;
		}

	}
	if (lookahead.code != SEOF_T) {
		matchToken(EOS_T, NO_ATTR);
	}
	printf("%s%s\n", STR_LANGNAME, ": All Statements parsed");
}

/*
************************************************************
* Function: printBNFData
* Purpose : Prints parser statistics for each BNF rule.
* BNF     : (Utility function, not a direct BNF rule)
* Params  : psData - ParserData structure containing histogram
* Returns : void
************************************************************
*/
void printBNFData(ParserData psData) {
	// Print Parser statistics
	printf("Statistics:\n");
	printf("----------------------------------\n");
	int cont = 0;
	for (cont = 0; cont < NUM_BNF_RULES; cont++) {
		if (psData.parsHistogram[cont] > 0)
			printf("%s%s%s%d%s", "Token[", BNFStrTable[cont], "]=", psData.parsHistogram[cont], "\n");
	}
	printf("----------------------------------\n");
}

/*
************************************************************
* Function: load_model
* Purpose : Parses a load model statement.
* BNF     : <load_model> -> load model STRING
*           FIRST(<load_model>) = {KW_load}
* Params  : None
* Returns : void
************************************************************
*/
void load_model() {
	psData.parsHistogram[BNF_load_model]++;
	matchToken(KW_T, KW_load); // Match 'load' keyword
	matchToken(KW_T, KW_model); // Match 'model' keyword
	char *modelName = readerGetContent(stringLiteralTable, lookahead.attribute.contentString); // Get model name
	matchToken(STR_T, NO_ATTR); // Match model name string
	printf("%s%s %s parsed\n", STR_LANGNAME, ": Load model ", modelName);
}

/*
************************************************************
* Function: isDatatype
* Purpose : Checks if the given token attribute is a recognized datatype keyword.
* BNF     : (Utility function, not a direct BNF rule)
* Params  : t - TokenAttribute to check
* Returns : int - codeType if found, -1 otherwise
************************************************************
*/
int isDatatype(const TokenAttribute t) {
	const char* datatypes[10] = {"text", "image", "audio", "embedding", "number", "bigNumber", "file","media","doc","embed"};
	for (int i = 0; i < 9; i++) {
		if (strcmp(datatypes[i], keywordTable[t.codeType]) == 0) {
			return t.codeType;
		}
	}
	return -1;
}

/*
************************************************************
* Function: stream_expr
* Purpose : Parses a stream expression (stream_in or stream_out).
* BNF     : <stream_expr> -> stream_in ( <source_expr> , <format> )
*                        |  stream_out ( <id> , <dest_expr> )
*           FIRST(<stream_expr>) = {KW_stream_in, KW_stream_out}
* Params  : None
* Returns : void
************************************************************
*/
void stream_expr() {
	psData.parsHistogram[BNF_stream_expr]++;
	switch (lookahead.attribute.codeType) {
		case KW_stream_in:
			matchToken(KW_T, KW_stream_in); // Match 'stream_in' keyword
			matchToken(LPR_T, NO_ATTR); // Match '('
			source_expr(); // Parse source expression
			matchToken(COM_T, NO_ATTR); // Match ','
			format(); // Parse format
			matchToken(RPR_T, NO_ATTR); // Match ')'
			break;
		case KW_stream_out:
			matchToken(KW_T, KW_stream_out); // Match 'stream_out' keyword
			matchToken(LPR_T, NO_ATTR); // Match '('
			id(); // Parse id
			matchToken(COM_T, NO_ATTR); // Match ','
			dest_expr(); // Parse destination expression
			matchToken(RPR_T, NO_ATTR); // Match ')'
			break;
		default:
			syncErrorHandler(lookahead.code); // Handle unexpected token
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Stream expression parsed");
}

/*
************************************************************
* Function: assignment
* Purpose : Parses an assignment statement for various datatypes and operations.
* BNF     : <assignment> -> <type> <id> = <load_expr>
*                        |  <type> <id> = <operation_expr>
*                        |  <type> <id> = <stream_expr>
*           FIRST(<assignment>) = {KW_text, KW_image, ...}
* Params  : None
* Returns : void
************************************************************
*/
void assignment() {
	psData.parsHistogram[BNF_assignment]++;
	type(); // Parse type
	id(); // Parse identifier
	matchToken(ASSIGN_T, NO_ATTR); // Match '='
	
	switch (lookahead.code) {
		case INTL_T:
			arith_expr();
			break;
		case KW_T:
			// Check if it's a load expression (static) or operation expression (dynamic)
			switch (lookahead.attribute.codeType) {
				case KW_doc:
				case KW_file:
				case KW_image:
				case KW_audio:
				case KW_media:
						load_expr(); // Parse load expression
						break;
				case KW_ask:
				case KW_generate:
				case KW_embed:
						operation_expr(); // Parse operation expression
						break;
				case KW_stream_in:
				case KW_stream_out:
						stream_expr(); // Parse stream expression
						break;
				default:
						syncErrorHandler(lookahead.code); // Handle unexpected token
						break;
			}
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR); // Match string
			break;
		default:
			syncErrorHandler(lookahead.code); // Handle unexpected token
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": assignment statement parsed");
}
/*
************************************************************
* Function: model_context_block
* Purpose : Parses a model context block (using ... : ...).
* BNF     : <model_context_block> -> using STRING : <model_context_body>
*           FIRST(<model_context_block>) = {KW_using}
* Params  : None
* Returns : void
************************************************************
*/
void model_context_block() {
	psData.parsHistogram[BNF_model_context_block]++;
	matchToken(KW_T, KW_using); // Match 'using' keyword
	matchToken(STR_T, NO_ATTR); // Match string
	matchToken(COLON_T, NO_ATTR); // Match ':'
	model_context_body(); // Parse model context body
	printf("%s%s\n", STR_LANGNAME, ": Model context block parsed");
}
/*
************************************************************
* Function: type
* Purpose : Parses a datatype keyword.
* BNF     : <type> -> KW_text | KW_image | KW_audio | ...
*           FIRST(<type>) = {KW_text, KW_image, ...}
* Params  : None
* Returns : void
************************************************************
*/
void type() {
	psData.parsHistogram[BNF_type]++;
	matchToken(KW_T, isDatatype(lookahead.attribute)); // Match datatype keyword
}
/*
************************************************************
* Function: id
* Purpose : Parses an identifier token.
* BNF     : <id> -> VID_T
*           FIRST(<id>) = {VID_T}
* Params  : None
* Returns : void
************************************************************
*/
void id() {
	psData.parsHistogram[BNF_id]++;
	matchToken(VID_T, NO_ATTR); // Match identifier
}
/*
************************************************************
* Function: load_expr
* Purpose : Parses a load expression for static datatypes.
* BNF     : <load_expr> -> <type> ( STRING | <id> , <format> )
*           FIRST(<load_expr>) = {KW_text, KW_image, ...}
* Params  : None
* Returns : void
************************************************************
*/
void load_expr() {
	psData.parsHistogram[BNF_load_expr]++;
	type(); // Parse type
	matchToken(LPR_T, NO_ATTR); // Match '('
	if (lookahead.code == STR_T) {
		matchToken(STR_T, NO_ATTR); // Match string
	}
	else if (lookahead.code == VID_T) {
		id(); // Match identifier
	}
	else {
		syncErrorHandler(lookahead.code); // Handle unexpected token
	}
	matchToken(COM_T, NO_ATTR); // Match ','
	format(); // Parse format
	matchToken(RPR_T, NO_ATTR); // Match ')'
}


/*
************************************************************
* Function: format
* Purpose : Parses a format token.
* BNF     : <format> -> FMT_T
*           FIRST(<format>) = {FMT_T}
* Params  : None
* Returns : void
************************************************************
*/
void format() {
	psData.parsHistogram[BNF_format]++;
	matchToken(FMT_T, NO_ATTR); // Match format token
}

/*
************************************************************
* Function: model_context_body
* Purpose : Parses the body of a model context block.
* BNF     : <model_context_body> -> { TAB_T <assignment> | TAB_T <cache_statement> | TAB_T <save_statement> }
*           FIRST(<model_context_body>) = {TAB_T}
* Params  : None
* Returns : void
************************************************************
*/
void model_context_body() {
	psData.parsHistogram[BNF_model_context_body]++;
	
	// Loop through all statements in the model context body
	while (lookahead.code != SEOF_T  ) {
		if (lookahead.code == EOS_T) {
			matchToken(EOS_T, NO_ATTR); // Skip end of statement
			continue;
		}

		if (lookahead.code != TAB_T) {
			break;
		}
		matchToken(TAB_T, NO_ATTR); // Match tab (indentation)

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

/*
************************************************************
* Function: cache_statement
* Purpose : Parses a cache statement in the model context body.
* BNF     : <cache_statement> -> cache ( <id> )
*           FIRST(<cache_statement>) = {KW_cache}
* Params  : None
* Returns : void
************************************************************
*/
void cache_statement() {
	psData.parsHistogram[BNF_cache_statement]++;
	matchToken(KW_T, KW_cache); // Match 'cache' keyword
	matchToken(LPR_T, NO_ATTR); // Match '('
	id(); // Match identifier
	matchToken(RPR_T, NO_ATTR); // Match ')'
	// matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Cache statement parsed");
}

/*
************************************************************
* Function: save_statement
* Purpose : Parses a save statement in the model context body.
* BNF     : <save_statement> -> save ( <id> , STRING )
*           FIRST(<save_statement>) = {KW_save}
* Params  : None
* Returns : void
************************************************************
*/
void save_statement() {
	psData.parsHistogram[BNF_save_statement]++;
	matchToken(KW_T, KW_save); // Match 'save' keyword
	matchToken(LPR_T, NO_ATTR); // Match '('
	id(); // Match identifier
	matchToken(COM_T, NO_ATTR); // Match ','
	matchToken(STR_T, NO_ATTR); // Match string
	matchToken(RPR_T, NO_ATTR); // Match ')'
	// matchToken(EOS_T, NO_ATTR);
	printf("%s%s\n", STR_LANGNAME, ": Save statement parsed");
}

/*
************************************************************
* Function: operation_expr
* Purpose : Parses an operation expression (ask, generate, embed).
* BNF     : <operation_expr> -> ask ( STRING [, <id>] )
*                            |  generate ( STRING [, <id>] )
*                            |  embed ( <id> )
*           FIRST(<operation_expr>) = {KW_ask, KW_generate, KW_embed}
* Params  : None
* Returns : void
************************************************************
*/
void operation_expr() {
	psData.parsHistogram[BNF_operation_expr]++;
	switch (lookahead.attribute.codeType) {
		case KW_ask:
		case KW_generate:
			matchToken(KW_T, lookahead.attribute.codeType); // Match 'ask' or 'generate' keyword
			matchToken(LPR_T, NO_ATTR); // Match '('
			matchToken(STR_T, NO_ATTR); // Match string
			if (lookahead.code == COM_T) {
				matchToken(COM_T, NO_ATTR); // Match ','
				id(); // Match identifier
			}
			matchToken(RPR_T, NO_ATTR); // Match ')'
			break;
		case KW_embed:
			matchToken(KW_T, KW_embed); // Match 'embed' keyword
			matchToken(LPR_T, NO_ATTR); // Match '('
			id(); // Match identifier
			matchToken(RPR_T, NO_ATTR); // Match ')'
			break;
		default:
			syncErrorHandler(lookahead.code); // Handle unexpected token
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Operation expression parsed");
}

/*
************************************************************
* Function: source_expr
* Purpose : Parses a source expression (string or identifier).
* BNF     : <source_expr> -> <id> | STRING
*           FIRST(<source_expr>) = {VID_T, STR_T}
* Params  : None
* Returns : void
************************************************************
*/
void source_expr() {
	psData.parsHistogram[BNF_source_expr]++;
	switch (lookahead.code) {
		case VID_T:
			id(); // Match identifier
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR); // Match string
			break;
		default:
			syncErrorHandler(lookahead.code); // Handle unexpected token
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Source expression parsed");
}

/*
************************************************************
* Function: dest_expr
* Purpose : Parses a destination expression (console, string, or identifier).
* BNF     : <dest_expr> -> console | STRING | <id>
*           FIRST(<dest_expr>) = {KW_console, STR_T, VID_T}
* Params  : None
* Returns : void
************************************************************
*/
void dest_expr() {
	psData.parsHistogram[BNF_dest_expr]++;
	switch (lookahead.code) {
		case KW_T:
			if (lookahead.attribute.codeType == KW_console) {
				matchToken(KW_T, KW_console); // Match 'console' keyword
			} else {
				syncErrorHandler(lookahead.code); // Handle unexpected token
			}
			break;
		case STR_T:
			matchToken(STR_T, NO_ATTR); // Match string
			break;
		case VID_T:
			matchToken(VID_T, NO_ATTR); // Match identifier
			break;
		default:
			syncErrorHandler(lookahead.code); // Handle unexpected token
			break;
	}
	printf("%s%s\n", STR_LANGNAME, ": Destination expression parsed");
}

/*
************************************************************
* Function: arith_expr
* Purpose : Parses an arithmetic expression.
* BNF     : <arith_expr> -> <term> { (+|-) <term> }
************************************************************
*/
void arith_expr() {
	psData.parsHistogram[BNF_arith_expr]++;
    term();
    while (lookahead.code == ARIT_OP_T && 
          (lookahead.attribute.arithmeticOperator == OP_ADD ||
           lookahead.attribute.arithmeticOperator == OP_SUB)) {
        matchToken(ARIT_OP_T, lookahead.attribute.arithmeticOperator);
        term();
    }
}

/*
************************************************************
* Function: term
* Purpose : Parses a term in an arithmetic expression.
* BNF     : <term> -> <factor> { (*|/) <factor> }
************************************************************
*/
void term() {
	psData.parsHistogram[BNF_term]++;
    factor();
    while (lookahead.code == ARIT_OP_T &&
          (lookahead.attribute.arithmeticOperator == OP_MUL ||
           lookahead.attribute.arithmeticOperator == OP_DIV)) {
        matchToken(ARIT_OP_T, lookahead.attribute.arithmeticOperator);
        factor();
    }
}

/*
************************************************************
* Function: factor
* Purpose : Parses a factor in an arithmetic expression.
* BNF     : <factor> -> <number> | <id> | ( <arith_expr> )
************************************************************
*/
void factor() {
	psData.parsHistogram[BNF_factor]++;
    if (lookahead.code == INTL_T) { // Integer literal
        matchToken(INTL_T, NO_ATTR);
    } else if (lookahead.code == VID_T) { // Variable identifier
        id();
    } else if (lookahead.code == LPR_T) { // Parenthesized expression
        matchToken(LPR_T, NO_ATTR);
        arith_expr();
        matchToken(RPR_T, NO_ATTR);
    } else {
        syncErrorHandler(lookahead.code);
    }
}


