
#ifndef HELPFUNCTION_H  
#define HELPFUNCTION_H  

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>



#define TRUE 1
#define FALSE 0
#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define NINE 9
#define TWELVE 12
#define THIRTEEN 13
#define FOURTEEN 14
#define FIFTEEN 15
#define SPACE ' '
#define TAB '\t'
#define SPACE_TAB " \t"
#define COMMA ','
#define COLON ':'
#define SEMICOLON ';'
#define HASH_TAG '#'
#define MAX_CHAR_IN_LINE 82
#define CODE_WORD_SIZE 25
#define NUM_COMMANDS 16
#define NUM_REGISTERS 8
#define START_ADDRESS 100
#define MEMORY_SIZE 2097152
#define MEMORY_ALLOCATION_FAIL -1


typedef enum
{
    external,
    code,
    data,
    code_entry,
    data_entry
}types;



typedef enum {
    no,
    yes
}done;

typedef struct {
    done finish;
    int address;
    char code[CODE_WORD_SIZE];
	int row;
}codes;

typedef struct {
    int address;
	char code[CODE_WORD_SIZE];
}data_code;

typedef enum {
    instant,
    direct,
    relative,
    direct_register,
    non
}address_method;

/**
 * Skips over any leading white spaces (spaces or tabs) in the input string.
 * If the input string is NULL, it returns NULL.
 *
 * @param str Pointer to the string to be processed.
 * @return Pointer to the first non-white character in the string, or NULL if the string is empty or NULL.
 */
char* skip_white_chars(char* str);

/**
 * Skips over any leading white spaces (spaces or tabs) in the input string.
 * If the input string is NULL, it returns NULL.
 *
 * @param str Pointer to the string to be processed.
 * @return Pointer to the first non-white character in the string, or NULL if the string is empty or NULL.
 */
char* skip_white_chars_from_end(char* str);

/**
 * Cuts a word from a string by a single character delimiter, and shifts the rest of the string.
 * The word is copied into the result array, and the remaining part of the string is modified in-place.
 *
 * @param word Pointer to the string to be processed.
 * @param ch1 The character delimiter to cut the word by.
 * @param result A character array to store the extracted word.
 */
void cut_word(char* word, char ch1, char result[MAX_CHAR_IN_LINE]);

/**
 * Cuts a word from a string by two character delimiters, and shifts the rest of the string.
 * The word is copied into the result array, and the remaining part of the string is modified in-place.
 *
 * @param word Pointer to the string to be processed.
 * @param ch1 The first character delimiter to cut the word by.
 * @param ch2 The second character delimiter to cut the word by.
 * @param result A character array to store the extracted word.
 */
void cut_word2(char* word, char ch1, char ch2, char result[MAX_CHAR_IN_LINE]);

/**
 * Checks if a string represents a valid number (including signs).
 * Ignores leading and trailing white spaces and checks if the string consists of digits only,
 * possibly starting with a '+' or '-' sign.
 *
 * @param number Pointer to the string to be checked.
 * @return TRUE (1) if the string represents a valid number, FALSE (0) otherwise.
 */
int is_number(char* number);

/**
 * Converts an integer to its binary representation as a string with a specific number of bits.
 *
 * @param num The integer to be converted.
 * @param str A character array to store the binary string.
 * @param bits The number of bits for the binary representation.
 */
void int_to_binary(int num, char* str, int bits);

/**
 * Checks if a string is empty (ignores white spaces).
 *
 * @param str Pointer to the string to be checked.
 * @return TRUE (1) if the string is empty, FALSE (0) otherwise.
 */
int is_empty_string(char* str);

/**
 * Opens a file for reading. If the file cannot be opened, prints an error message and returns NULL.
 *
 * @param file_name The name of the file to be opened.
 * @return A pointer to the opened file, or NULL if the file could not be opened.
 */
FILE* search_file(char* file_name);

/**
 * Creates and opens a file for writing. If the file cannot be opened, prints an error message and returns NULL.
 *
 * @param file_name The name of the file to be created.
 * @return A pointer to the created file, or NULL if the file could not be created.
 */
FILE* create_file(char* file_name);

/**
 * Changes the file extension of a file name.
 * If the file already has an extension, it is removed and replaced with the new extension.
 *
 * @param filename The file name to modify.
 * @param new_ending The new file extension (without a dot).
 */
void change_ending(char* filename, char* new_ending);

/**
 * Adds an extension to a file name.
 * The extension is appended with a dot .
 *
 * @param filename The file name to modify.
 * @param ending The extension to be added (without a dot).
 */
void add_ending(char* filename, char* ending);

/**
 * Removes the first character of a string and shifts the rest of the string to the left.
 *
 * @param str Pointer to the string to be modified.
 */
void remove_first_char(char* str);

/**
 * Checks if a string represents a valid register.
 *
 * @param str Pointer to the string to be checked.
 * @return TRUE (1) if the string is a valid register, FALSE (0) otherwise.
 */
int is_register(char* str);

/**
 * Checks if a string represents a valid command.
 *
 * @param str Pointer to the string to be checked.
 * @return TRUE (1) if the string is a valid command, FALSE (0) otherwise.
 */
int is_command(char* str);

/**
 * Adds the default file extension ".as" to a file name if it doesn't already have an extension.
 *
 * @param file_name The file name to modify.
 * @return A pointer to the modified file name.
 */
char* valid_file_name(char* file_name);

#endif


