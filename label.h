#ifndef LABEL_H  
#define LABEL_H  


#include "mcr.h"


#define MAX_CHAR_LBL 31

typedef struct label
{
    char name[MAX_CHAR_LBL +TWO];
    int address;
    types type;
} label;


/**
 * This function checks if a given word is a label.
 * 
 * @param word The word to check.
 * @return TRUE if the word is a label, FALSE otherwise.
 */
int is_lable(char* word);

/**
 * This function checks if a label is valid.
 * 
 * @param word The label name.
 * @param row_count The current line number in the source file.
 * @param labels Array of existing labels.
 * @param label_count Number of labels.
 * @param macro List of existing macros.
 * @param file_name Name of the source file.
 * @param flag Pointer to an error flag.
 * @return TRUE if the label is valid, FALSE otherwise.
 */
int is_valid_lable(char* word, int row_count, label* labels, int label_count, macro_list* macro, char* file_name, int* flag);

/**
 * This function checks if a label name follows the correct pattern.
 * 
 * @param word The label name to check.
 * @return TRUE if the label pattern is valid, FALSE otherwise.
 */
int is_valid_lable_pattern(char* word);

/**
 * This function inserts a new label into the label list.
 * 
 * @param labels The existing label array.
 * @param label_name The name of the label.
 * @param label_count The number of labels.
 * @param type The type of the label.
 * @param address The address assigned to the label.
 * @param row_count The current line number in the source file.
 * @param flag Pointer to an error flag.
 * @return The updated label list or NULL on failure.
 */
label* insert_label(label* labels, char* label_name, int label_count, types type, int address, int row_count, int* flag);

#endif
