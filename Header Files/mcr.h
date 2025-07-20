


#ifndef MCR_H  
#define MCR_H 

#include "helpFunctions.h"

#define NUM_COMMANDS 16
#define NUM_REGISTERS 8
#define MAX_CHAR_IN_MACRO_NAME 31
#define MAX_CHAR 80
#define MACROEND 7

typedef struct list_content
{
    char* value;
    struct list_content* next;
}list_content;


typedef struct macro_list
{
    char* name;
    list_content* content;
     struct macro_list* next;
}macro_list;


/* This function processes assembly source files, detects macros,
   and expands them accordingly.

   @param as The input assembly file.
   @param file_name The name of the source file.
   @param am The output file after macro expansion.
   @return A pointer to the macro list, or NULL in case of failure. */
macro_list* macro_conversion(FILE* as, char* file_name, FILE* am);

/* This function checks if a given string matches an existing macro name.

   @param str The input string.
   @param macro The linked list of defined macros.
   @return A pointer to the found macro, or NULL if not found. */
macro_list* is_macro(char* str, macro_list* macro);

/* This function checks if a new macro can be created and adds it to the macro list.

   @param str The macro name.
   @param macro The macro list.
   @param flag A pointer to a flag indicating errors.
   @param as The input file.
   @param row_count A pointer to the current row count.
   @param file_name The name of the source file.
   @return The updated macro list or NULL in case of an error. */
macro_list* is_new_macro(char* str, macro_list* macro, int* flag, FILE* as, int* row_count, char* file_name);

/* This function connects a new macro node to an existing macro list.


   @param macro The head of the macro list.
   @param current The macro node to be added.
   @return The updated macro list. */

macro_list* conect_marco_list(macro_list* macro, macro_list* current);

/* This function writes the content of a macro to the output file.

   @param current The macro to be written.
   @param am The output file. */
void write_macro(macro_list* current, FILE* am);

/* This function verifies if a macro name is valid.

   @param flag A pointer to an error flag.
   @param str The macro name.
   @param row_count The current row number.
   @param file_name The source file name.
   @return 1 if the name is valid, 0 otherwise. */
int is_valid_macro_name(int* flag, char* str, int row_count,char *file_name);

/* This function frees the allocated memory of the macro list.

   @param macro The head of the macro list. */
void free_macro_list(macro_list* macro);
/*void create_new_filename(char* file_name, char* new_filename);*/

/* This function creates a linked list containing the macro content.

   @param as The input file.
   @param row_count A pointer to the row count.
   @param flag A pointer to an error flag.
   @param file_name The source file name.
   @return A pointer to the created content list. */
list_content* create_content_list(FILE* as, int* row_count, int* flag, char* file_name);

/* This function connects a new content node to the content list.

   @param lst The head of the content list.
   @param current The new content node. */
void connect_list(list_content* lst, list_content* current);

/* This function frees the allocated memory of the macro content list.

   @param list The head of the content list. */
void free_list_content(list_content* list);

/* This function checks if a macro with the given name already exists.

   @param name The macro name.
   @param macro The macro list.
   @return 1 if the macro exists, 0 otherwise. */
int if_exsit_macro(char* name, macro_list* macro);


#endif
