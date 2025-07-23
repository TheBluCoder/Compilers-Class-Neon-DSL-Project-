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
***********************************************************
* File name: Writer.c
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

#ifndef CODER_H_
#include "Step5Writer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Step3Scanner.h"

/* Global variables */
Variable variables[MAX_VARS];
int var_count = 0;
int initial_phase = 1; // Flag to track the initial phase
char output_buffer[MAX_EXPR_LEN * 10] = { 0 }; // Buffer to store write output

/* Finds variables */
int find_variable(const char* name) {
    int i = 0;
    for (i = 0; i < var_count; ++i) {
        if (strcmp(variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

/* Assign string variable */
void assign_string_variable(const char* name, const char* value) {
    int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = STRING;
    strncpy_s(variables[idx].value.str_value, sizeof(variables[idx].value.str_value), value, sizeof(variables[idx].value.str_value) - 1);
    variables[idx].value.str_value[sizeof(variables[idx].value.str_value) - 1] = EOS;
}

/* Gets string variable */
const char* get_string_value(const char* name) {
    int idx = find_variable(name);
    if (idx != -1 && variables[idx].type == STRING) {
        return variables[idx].value.str_value;
    }
    return STREMPTY;
}

/* Write output */
/* Helper function to skip whitespace */
void skip_whitespace(char** ptr) {
    while (isspace(**ptr)) {
        (*ptr)++;
    }
}

/* Helper function to check if current position starts with target word and clear buffer if not */
int check_console_and_clear_buffer(char* wrd, const char* targetWord, size_t targetWordLen, char* buffer, size_t bufferSize) {
    skip_whitespace(&wrd);
    if (strncmp(wrd, targetWord, targetWordLen) != 0) {
        memset(buffer, 0, bufferSize);
        return 0; // Not console
    }
    return 1; // Is console
}

void handle_write(char* expression) {
    char buffer[MAX_EXPR_LEN] = { 0 };
    char * targetWord = "console";
    size_t targetWordLen = strlen(targetWord);
    char* start = strchr(expression, LPAR) + 1;
    char* end = strrchr(expression, RPAR);
    if (start != NULL && end != NULL && start < end) {
        *end = EOS;
        while (*start != EOS) {
            if (*start == QUOTES) {
                start++; // Skip the opening quote
                while (*start != QUOTES && *start != EOS) { // Stop before the closing quote
                    strncat_s(buffer, sizeof(buffer), start, 1);
                    start++;
                }
                if (*start == QUOTES) {
                    start++; // Skip the closing quote
                }
                skip_whitespace(&start); // Skip any whitespace after the closing quote
                if (*start == COM_CHR) {
                    start++; // Skip the comma
                }
                if (check_console_and_clear_buffer(start, targetWord, targetWordLen ,buffer, sizeof(buffer))) {
                    start = end;
                    buffer[sizeof(buffer) - 1] = EOS;
                }else {
                    return;
                }
            }

            else if (isalpha(*start)) {
                char var_name[32] = { 0 };
                int i = 0;
                while (isalnum(*start) && *start != COM_CHR) {
                    var_name[i++] = *start++;
                }
                int var_idx = find_variable(var_name);
                *start++;
                skip_whitespace(&start);
                if (var_idx != -1 && (strncmp(start, targetWord, targetWordLen) == 0)) {
                    if (variables[var_idx].type == STRING) {
                        strcat_s(buffer, sizeof(buffer), variables[var_idx].value.str_value);
                    }
                    else if (variables[var_idx].type == NUMERIC) {
                        char num_str[256] = { 0 };
                        sprintf_s(num_str, sizeof(num_str), "%.2f", variables[var_idx].value.num_value);
                        strcat_s(buffer, sizeof(buffer), num_str);
                    }
                    else if (variables[var_idx].type == BOOLEAN) {
                        strcat_s(buffer, sizeof(buffer), variables[var_idx].value.bool_value ? "true" : "false");
                    }
                }
            }
            else if (isspace(*start)) {
                while (*start != COM_CHR) {
                    strncat_s(buffer, sizeof(buffer), start, 1);
                    start++;
                }
                if (!check_console_and_clear_buffer(start, targetWord, targetWordLen, buffer, sizeof(buffer)))
                    return;
            }
            else {
                start++;
            }
        }
    }
    if (initial_phase) {
        strcat_s(output_buffer, sizeof(output_buffer), buffer);
        strcat_s(output_buffer, sizeof(output_buffer), "\n");
    }
    else {
        printf("%s\n", buffer);
    }
    // printf("%s", output_buffer); // Print the buffered write output
}

/* Calculate expression */
void calculate(char* expression) {
    char var_name[32] = { 0 };
    if (strchr(expression, EQUALS)) {
        char* expr = strchr(expression, SPC_CHR) + 1;
        sscanf_s(expr, "%31s =", var_name, (unsigned)_countof(var_name));
        expr = strchr(expression, EQUALS) + 1;
        while (isspace(*expr)) expr++;
        if (*expr == QUOTES) {
            expr++;
            char str_value[256] = { 0 };
            int i = 0;
            while (*expr != QUOTES && *expr != EOS) {
                str_value[i++] = *expr++;
            }
            assign_string_variable(var_name, str_value);
            if (!initial_phase) {
                printf("%s = \"%s\"\n", var_name, str_value);
            }
        }
        else if (isdigit(*expr)) {
            char num_expr[256] = { 0 };
            int isExpr = 0;
            int i = 0;
            while (*expr != EOS) {
                num_expr[i++] = *expr++;
            }
            for (int j = 0; j < sizeof(aritOpStrTable)/sizeof(aritOpStrTable[0]); j++) {
                if (strchr(num_expr, aritOpStrTable[j][0]) != NULL) {
                    isExpr = 1;
                    break;
                }
            }
            if (isExpr) {
                assign_numeric_expression(var_name, num_expr);
                if (!initial_phase) {
                    printf("%s = %.2f\n", var_name, variables[find_variable(var_name)].value.num_value);
                }
            } else {
                assign_numeric_variable(var_name, num_expr);
                if (!initial_phase) {
                    printf("%s = %.2f\n", var_name, strtod(num_expr,NULL));
                }
            }
        }
        else if (isalpha(*expr) || *expr == LPAR) {
            char s_expr[256] = { 0 };
            int isExpr = 0;
            int i = 0;
            while (*expr != EOS) {
                s_expr[i++] = *expr++;
            }
            for (int j = 0; j < sizeof(aritOpStrTable)/sizeof(aritOpStrTable[0]); j++) {
                if (strchr(s_expr, aritOpStrTable[j][0]) != NULL) {
                    isExpr = 1;
                    break;
                }
            }
            if (isExpr) {
                assign_numeric_expression(var_name, s_expr);
                if (!initial_phase) {
                    printf("%s = %.2f\n", var_name, variables[find_variable(var_name)].value.num_value);
                }
            }
            else {
                assign_variable_to_variable(var_name, s_expr);
            }
        }
    }
    else if (strstr(expression, WRITE)) {
        handle_write(expression);
    }
}

/* Process input file */
// void process_file(const char* filename) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         exit(EXIT_FAILURE);
//     }
//     char line[MAX_EXPR_LEN];
//     while (fgets(line, sizeof(line), file)) {
//         if (line[0] == NEWLINE || line[0] == RETURN) {
//             continue; // Skip empty lines
//         }
//         line[strcspn(line, "\n")] = 0; // Remove newline character
//         calculate(line);
//     }
//     fclose(file);
//     initial_phase = 0; // End of initial phase
//     printf("%s", output_buffer); // Print the buffered write output
//     printf("\nVariable values:\n");
// 	int i = 0;
//     for (i = 0; i < var_count; i++) {
//         if (variables[i].type == STRING) {
//             printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
//         }
//         else if (variables[i].type == NUMERIC) {
//             printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
//         }
//         else if (variables[i].type == BOOLEAN) {
//             printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
//         }
//         else if (variables[i].type == CHAR) {
//             printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
//         }
//     }
// }

/* Split lines into components */
char** splitIntoLines(const char* content, int* lineCount) {
    char** lines = malloc(MAX_LINES * sizeof(char*));
    if (!lines) {
        perror("Error allocating memory");
        return NULL;
    }
    *lineCount = 0;
    const char* start = content;
    const char* end;
    while ((end = strchr(start, NEWLINE)) != NULL) {
        if (*lineCount >= MAX_LINES) {
            fprintf(stderr, "Exceeded maximum number of lines\n");
            break;
        }
        int lineLength = (int) (end - start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
            break;
        }
        // Use strncpy_s for safer copying
        if (strncpy_s(lines[*lineCount], lineLength + 1, start, lineLength) != 0) {
            perror("Error copying string using strncpy_s");
            free(lines[*lineCount]);
            break;
        }
        lines[*lineCount][lineLength] = EOS; // Null-terminate the string
        (*lineCount)++;
        start = end + 1;
    }
    // Handle the last line if it doesn't end with a newline
    if (*start != EOS && *lineCount < MAX_LINES) {
        size_t lineLength = strlen(start);
        lines[*lineCount] = malloc(lineLength + 1);
        if (!lines[*lineCount]) {
            perror("Error allocating memory for line");
        }
        else {
            // Use _strdup to copy the remaining content
            lines[*lineCount] = _strdup(start);
            if (!lines[*lineCount]) {
                perror("Error copying string using _strdup");
            }
            else {
                (*lineCount)++;
            }
        }
    }
    return lines;
}

/* Free lines */
void freeLines(char** lines, int lineCount) {
    int i = 0;
    for (i = 0; i < lineCount; i++) {
        free(lines[i]);
    }
    free(lines);
}

/* Process content */
void process_content(char* fileContent) {
    int lineCount = 0;
    char** lines = splitIntoLines(fileContent, &lineCount);
    char* line = malloc(MAX_EXPR_LEN);
    if (!lines || !line) {
        return;
    }
    printf("Lines from content:\n");
    int i = 0;
	for (i = 0; i < lineCount; i++) {
		line = lines[i];
		calculate(line);
	}
    initial_phase = 0; // End of initial phase
    printf("%s", output_buffer); // Print the buffered write output
    printf("\nVariable values:\n");
    for (i = 0; i < var_count; i++) {
        if (variables[i].type == STRING) {
            printf("%s = \"%s\"\n", variables[i].name, variables[i].value.str_value);
        }
        else if (variables[i].type == NUMERIC) {
            printf("%s = %.2lf\n", variables[i].name, variables[i].value.num_value);
        }
        else if (variables[i].type == BOOLEAN) {
            printf("%s = %s\n", variables[i].name, variables[i].value.bool_value ? "true" : "false");
        }
        else if (variables[i].type == CHAR) {
            printf("%s = '%c'\n", variables[i].name, variables[i].value.char_value);
        }
    }

    freeLines(lines, lineCount);
}

void assign_numeric_variable(const char* name, const char* value) {
    int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = strtod(value, NULL);
}

void assign_numeric_expression(const char* name, const char* value) {
    int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = NUMERIC;
    variables[idx].value.num_value = evaluate_expression(value);
}

/* Helper function to evaluate numeric expressions */
double evaluate_expression(const char* expr) {
    char* expression = _strdup(expr);
    if (!expression) return 0.0;
    
    // Remove all whitespace
    char* write_ptr = expression;
    char* read_ptr = expression;
    while (*read_ptr) {
        if (!isspace(*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
    
    double result = parse_expression(&expression);
    return result;
}

/* Parse expression with operator precedence */
double parse_expression(char** expr) {
    double left = parse_term(expr);
    
    while (**expr == PLUS || **expr == MINUS) {
        char op = **expr;
        (*expr)++;
        double right = parse_term(expr);
        
        if (op == PLUS) {
            left += right;
        } else {
            left -= right;
        }
    }
    
    return left;
}

/* Parse term (multiplication and division) */
double parse_term(char** expr) {
    double left = parse_factor(expr);
    
    while (**expr == MULT || **expr == DIV) {
        char op = **expr;
        (*expr)++;
        double right = parse_factor(expr);
        
        if (op == MULT) {
            left *= right;
        } else if (op == DIV) {
            if (right == 0.0) {
                // Handle division by zero
                return 0.0;
            }
            left /= right;
        }
    }
    
    return left;
}

/* Parse factor (numbers, variables, and parenthesized expressions) */
double parse_factor(char** expr) {
    skip_whitespace(expr);
    
    if (**expr == LPAR) {
        (*expr)++; // Skip '('
        double result = parse_expression(expr);
        if (**expr == RPAR) {
            (*expr)++; // Skip ')'
        }
        return result;
    }
    
    if (isdigit(**expr) || **expr == '.') {
        // Parse number
        char* endptr;
        double num = strtod(*expr, &endptr);
        *expr = endptr;
        return num;
    }
    
    if (isalpha(**expr)) {
        // Parse variable name
        char var_name[32] = {0};
        int i = 0;
        while (isalnum(**expr) && i < 31) {
            var_name[i++] = **expr;
            (*expr)++;
        }
        var_name[i] = '\0';
        
        // Find variable and return its value
        int var_idx = find_variable(var_name);
        if (var_idx != -1 && variables[var_idx].type == NUMERIC) {
            return variables[var_idx].value.num_value;
        }
        // Return 0 if variable not found or not numeric
        return 0.0;
    }
    
    // If we get here, there's an error in the expression
    return 0.0;
}

void assign_boolean_variable(const char* name, const char* value) {
    int idx = find_variable(name);
    if (idx == -1) {
        idx = var_count++;
        strcpy_s(variables[idx].name, sizeof(variables[idx].name), name);
    }
    variables[idx].type = BOOLEAN;
    variables[idx].value.bool_value = (strcmp(value, "true") == 0);
}

void assign_variable_to_variable(const char* name, const char* value) {
    int idx = find_variable(name);
    char errorMsg[128];
    if (idx == -1) {
        sprintf_s(errorMsg, sizeof(errorMsg), "Variable %s not found", name);
        perror(errorMsg);
        return;
    }
    Variable var = variables[idx];
    switch (var.type) {
        case NUMERIC:
            assign_numeric_variable(value, get_string_value(name));
            break;
        case BOOLEAN:
            assign_boolean_variable(value, get_string_value(name));
            break;
        case STRING:
            assign_string_variable(value, get_string_value(name));
            break;
        default:
            sprintf_s(errorMsg, sizeof(errorMsg), "Invalid variable type for var %s", var.name);
            perror(errorMsg);
    }
}
