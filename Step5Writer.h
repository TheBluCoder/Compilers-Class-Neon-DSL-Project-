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
# ECHO "[WRITER SCRIPT .........................]"
# ECHO "                                         "
*/

/*
************************************************************
* File name: Writer.h
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

#ifndef WRITER_H_
#define WRITER_H_

#define MAX_VARS 100
#define MAX_EXPR_LEN 256
#define MAX_LINES 100

#define WRITE "output"

#define EOS '\0'
#define ZERO 0.0
#define STREMPTY ""
#define LPAR '('
#define RPAR ')'
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'

#define EQUALS '='
#define MOD '%'
#define SPACE ' '
#define TAB '\t'
#define NEWLINE '\n'
#define RETURN '\r'
#define QUOTES '"'
#define QUOTE '\''

#define TRUE "True"
#define FALSE "False"

typedef enum { NUMERIC, STRING, BOOLEAN, CHAR } VarType;

typedef struct {
    char name[32];
    VarType type;
    union {
        double num_value;
        char str_value[256];
        int bool_value; // For BOOLEAN type (1 for true, 0 for false)
        char char_value; // For CHAR type
    } value;
} Variable;

int find_variable(const char* name);

void assign_string_variable(const char* name, const char* value);
const char* get_string_value(const char* name);

void handle_write(char* expression);  //adjust this so that when we simulate stream when we stream and dump, when we use output
void process_file(const char* filename);
void process_content(char* content);

/* my defined functions*/
void assign_numeric_variable(const char* name, const char* value);
// double get_numeric_value(const char* name);
void assign_boolean_variable(const char* name, const char* value);
// int get_boolean_value(const char* name);
void assign_other_type_variable(const char* name, char value); //Here is where we assign image() , embed() , audio()
char get_other_type_value(const char* name); // returns the filepath for image and audio,
                                               //embedd should technically print out the vector embedding of the file, rn we can't
void assign_numeric_expression(const char* name, const char* value);
double evaluate_expression(const char* expr);
double parse_expression(char** expr);
double parse_term(char** expr);
double parse_factor(char** expr);
void assign_variable_to_variable(const char* name, const char* value);

#endif
