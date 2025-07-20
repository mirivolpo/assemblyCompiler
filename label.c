#include "label.h"


int is_lable(char* word)
{
	char* word_end = word;
	if (word == NULL || *word == '\0')
               return FALSE;
	
        /* Move to the end of the string */
	while (*word_end != '\0')
		word_end++;
	if (*(word_end - 1) == COLON) /* A valid label ends with a colon */
		return TRUE;
	return FALSE;  /* Not a label */
}

int is_valid_lable(char* word, int row_count, label *labels,int label_count, macro_list* macro,char *file_name,int *flag)
{
	int i;
  
        /* Check if label follows the correct pattern */
	if (!is_valid_lable_pattern(word))
	{
		printf("ERROR in file %s, in line %d : Invalid charecters in label.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
         /* Check for duplicate label in the existing label array */
	for (i = 1; i <= label_count; i++)
	{
		if (strcmp(word, labels[1].name) == ZERO)
		{
			printf("ERROR in file %s, in line %d : A label with this name already exists.\n", file_name, row_count);
			*flag = TRUE;
			return FALSE;
		}
	}
        /* Check if the label name is already used as a macro */
	if (if_exsit_macro(word, macro))
	{
		printf("ERROR in file %s, in line %d : A macro with this name already exists.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	return TRUE;
}

int is_valid_lable_pattern(char* word)
{
	if (word != NULL)
	{
		if (is_empty_string(word))
		{
			return FALSE;
		}
		if (!isalpha(*word))/* Must start with a letter */
		{
			return FALSE;
		}
		if (strlen(word) > MAX_CHAR_LBL) /* Check if the label exceeds the maximum allowed length */
		{
			return FALSE;
		}
		/* Check if the label matches reserved directive names */
		if (strcmp(word, "string") == ZERO || strcmp(word, "data") == ZERO || strcmp(word, "entry") == ZERO || strcmp(word, "extern") == ZERO)
		{
			return FALSE;
		}
		if (is_register(word))
		{
			return FALSE;
		}
		if (is_command(word))
		{
			return FALSE;
		}

		/* Verify that all characters are alphanumeric */
		while (*word != '\0')
		{
			if (!isalpha(*word) && !isdigit(*word))
			{
				return FALSE;
			}
			word++;
		}
		return TRUE;/* All checks passed, the label is valid */
	}
	return FALSE;	/* Return false if the input is NULL */
	
}



label* insert_label(label* labels, char* label_name, int label_count, types type, int address, int row_count, int* flag)
{
	if (labels != NULL)
	{
		strcpy(labels[label_count].name, label_name);
		labels[label_count].address = address;
		/* Assign the type (e.g., data, code, external) to the label */
		labels[label_count].type = type;
		return labels;/* Return the updated labels array */
	}
	*flag = TRUE;
	return NULL;/* Return NULL to indicate failure */
}
