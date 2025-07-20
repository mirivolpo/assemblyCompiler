#include "helpFunctions.h"


char* skip_white_chars(char* str)
{
    /* Check if the input string is NULL to avoid dereferencing */
    if (str == NULL) 
	return NULL;
    
   /* Move the pointer forward while the current character is a space or tab */
    while (*str == ' ' || *str == '\t') 
    {
        str++;
    }
    return str;
}


char* skip_white_chars_from_end(char* str) 
{
    int len = strlen(str);
    /* Return unchanged if the string is NULL or empty */
    if (str == NULL || *str == '\0')
    {
        return str;
    }

    /* Move backwards from the end while the character is a whitespace */
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        len--;
    }

    /* Null-terminate the string at the new end to trim trailing whitespace */
    str[len] = '\0';
    return str;
}


void cut_word(char* word, char ch1, char result[82]) 
{
    int i = 0, j = 0;
    /* Check for NULL pointers to avoid dereferencing */
    if (!word || !result) {
        return;
    }

   
    /* Copy characters from word to result until delimiter ch1 or end of string */
    while (word[i] != ch1 && word[i] != '\0') {
        result[i] = word[i];
        i++;
    }
    result[i] = '\0'; /* Null-terminate the result string */

    
    /* Skip the delimiter if it was found */
    if (word[i] == ch1) 
    {
        i++;
    }

    /* Shift remaining characters in word to the beginning */
    while (word[i] != '\0') {
        word[j++] = word[i++];
    }
    word[j] = '\0'; /* Null-terminate the modified word */
}


void cut_word2(char* word, char ch1, char ch2, char result[82])
{
    int i = 0, j = 0;
    
    /* Check for NULL pointers to avoid dereferencing */
    if (!word || !result) {
        return;
    }

    
    /* Copy characters from word to result until either delimiter (ch1 or ch2) or end of string */
    while (word[i] != ch1 && word[i] != ch2 && word[i] != '\0') {
        result[i] = word[i];
        i++;
    }
    result[i] = '\0'; /* Null-terminate the result string */

 
    /* Skip the delimiter if it was found (either ch1 or ch2) */
    if (word[i] == ch1 || word[i] == ch2) {
        i++;
    }

    
    /* Shift remaining characters in word to the beginning */
    while (word[i] != '\0') {
        word[j++] = word[i++];
    }
    word[j] = '\0'; /* Null-terminate the modified word */
}


int is_number(char* number)
{
    number = skip_white_chars(number);
    number = skip_white_chars_from_end(number);

    /* Skip optional leading sign (+ or -) */
    if (*number == '+' || *number == '-')
        number++;
    /* Check if the string is empty after the sign*/
    if(*number=='\0')
	return FALSE;

    /* Check each character to ensure it's a digit */
    while (*number != '\0')
    {
        if (!isdigit(*number))
            return FALSE;
        number++;
    }
    return TRUE;
}

void int_to_binary(int num, char* str, int bits)
{
    int i;

    /* Convert each bit of num to '1' or '0' in the string */
    for (i = bits - 1; i >= 0; i--)
    {
        str[bits - 1 - i] = (num & (1 << i)) ? '1' : '0';
    }
    str[bits] = '\0';/* Null-terminate the string after filling all bits */
}



int is_empty_string(char* str)
{

    /* Check if the string is NULL, treat as empty */
    if (str == NULL) 
	return TRUE;  

    str = skip_white_chars(str);  
    str=skip_white_chars_from_end(str);

    /* Return true if the resulting string is empty (only whitespace or null) */
    return (*str == '\0'); 
}

FILE* search_file(char* file_name)
{
    /* Attempt to open the file in read mode */
    FILE* file = fopen(file_name, "r");

    /* Check if file opening failed */
    if (!file)
    {
        printf("ERROR in opening file %s: Opening the file has failed 1", file_name);
        return NULL;
    }
    return file;
}

FILE* create_file(char* file_name)
{
    /* Attempt to open (or create) the file in read/write mode, overwriting if it exists */
    FILE* file = fopen(file_name, "w+");

    /* Check if file opening/creation failed */
    if (!file)
    {
        printf("ERROR in opening file %s: Opening the file has failed 2", file_name);
		return NULL;
    }
    return file;
}

void change_ending(char* filename, char* new_ending) 
{
    /* Find the last occurrence of '.' in the filename */
    char* dot = strrchr(filename, '.');

    /* If a dot is found, truncate the string at that point */
    if (dot) {
        *dot = '\0';/* Remove the existing extension by null-terminating at the dot */
    }
    strcat(filename, ".");

    strcat(filename, new_ending);/* Append the new ending to the filename */
}

void add_ending(char* filename, char* ending)
{
    strcat(filename, ".");

    strcat(filename, ending);/* Append the specified ending to the filename */
}


void remove_first_char(char* str)
{
    int i = 0;

    if (str == NULL || str[0] == '\0')
	 return; 
   
    /* Shift all characters to the left by one position */
    while (str[i] != '\0') {
        str[i] = str[i + 1]; 
        i++;
    }
}

int is_register(char* str)
{
    int i;
    char* registers[] = { "r0","r1","r2","r3","r4","r5","r6","r7" };
    skip_white_chars(str);
    skip_white_chars_from_end(str);
   
    /* Check against valid register names */
    for (i = 0; i < NUM_REGISTERS; i++)
    {
        if (strcmp(registers[i], str) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int is_command(char* str)
{
    int i;
    char* commands[] = { "mov","cmp","add","sub", "lea","clr","not","inc","dec","jmp","bne","jsr","red","prn","rts","stop" };
    skip_white_chars(str);
    skip_white_chars_from_end(str);

    /* Compare against names of commands */
    for (i = 0; i < NUM_COMMANDS; i++)
    {
        if (strcmp(commands[i], str) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}


char* valid_file_name(char* file_name)
{
    if (file_name == NULL)
        return NULL;
    add_ending(file_name,"as");/* Adds .as extension */
    return file_name;
}
