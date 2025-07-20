#include "firstPass.h"


int IC ;
int DC ;


void first_pass(FILE* am, char* file_name, macro_list* macro)
{
	int i,valid_check=0;
	/* Indicates if an error occurred during processing */
	int flag = FALSE;
	char source_operand[MAX_CHAR_LBL];
	char destination_operand[MAX_CHAR_LBL];
	char line1[MAX_CHAR_IN_LINE];
	char* line;
	char first_word[MAX_CHAR_IN_LINE];
	char label_name[MAX_CHAR_IN_LINE];
	address_method source_address;
	address_method destination_address;
	int row_count = 0;
	int flag_label;
	int label_count=0;
	label* labels = (label*)malloc(sizeof(label));
	
	
	codes* code_arr = (codes*)malloc(sizeof(codes) * MEMORY_SIZE);
	
	data_code* data_arr = (data_code*)malloc(sizeof(data_code) * MEMORY_SIZE);
	
	cmd commands[] = {
	{"mov",valid_mov_add_sub,ZERO,ZERO},
	{"cmp",valid_cmp,ONE,ZERO},
	{"add",valid_mov_add_sub,TWO,ONE},
	{"sub",valid_mov_add_sub,TWO,TWO},
	{"lea",valid_lea,FOUR,ZERO},
	{"clr",valid_clr_not_inc_dec_red,FIVE,ONE},
	{"not",valid_clr_not_inc_dec_red,FIVE,TWO},
	{"inc",valid_clr_not_inc_dec_red,FIVE,THREE},
	{"dec",valid_clr_not_inc_dec_red,FIVE,FOUR},
	{"jmp",valid_jmp_bne_jsr,NINE,ONE},
	{"bne",valid_jmp_bne_jsr,NINE,TWO},
	{"jsr",valid_jmp_bne_jsr,NINE,THREE},
	{"red" ,valid_clr_not_inc_dec_red,TWELVE,ZERO},
	{"prn",valid_prn,THIRTEEN,ZERO},
	{"rts",valid_rts_stop,FOURTEEN,ZERO},
	{"stop",valid_rts_stop,FIFTEEN,ZERO}
	};
	
	if (!labels)
	{
		printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
		free_macro_list(macro);
		free(labels);
		exit(1);
	}
	if (!code_arr)
	{
		free_macro_list(macro);
		free(labels);
		printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
		exit(1);
	}
	if (!data_arr)
	{
		free_macro_list(macro);
		free(labels);
		free(code_arr);
		printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
		exit(1);
	}
	
	/* Initialize Instruction Counter (IC) and Data Counter (DC) */
	IC=START_ADDRESS;
	DC=1;

	
	rewind(am);/* Reset file pointer to the beginning of the file */

	/* Main loop to process each line of the file */
	while (fgets(line1, sizeof(line1), am) != NULL)
	{ 
		row_count++;
		line = line1;
		if (*line == SEMICOLON)
			continue;
		
		/* Check if line exceeds maximum length */
		if (strchr(line, '\n') == NULL)
		{
			printf("ERROR in file %s, in line %d : More than 80 chars in line\n", file_name, row_count);
			flag = TRUE;
		}
		if (is_empty_string(line1))
		{
			continue;
		}
		flag_label = FALSE;/* Reset label flag for the current line */

		line=skip_white_chars(line);

		cut_word2(line, SPACE,TAB, first_word);/* Extract the first word from the line */

		strcpy(first_word, skip_white_chars(first_word));
		if (is_lable(first_word))
		{
			cut_word(first_word, COLON, label_name);/* Extract label name before ':' */
			if (is_valid_lable(label_name, row_count, labels,label_count, macro,file_name,&flag))
			{
				flag_label = TRUE;
				label_count++;
			}
			else
				continue;
			line=skip_white_chars(line);
			cut_word2(line, SPACE, TAB, first_word);/* Get the next word (command/directive) */
		}
		
		if (strcmp(first_word, ".string") == ZERO || strcmp(first_word, ".data") == ZERO)
		{
			/* If there's a label, store it with the current DC */
			if (flag_label == TRUE) 
			{
				labels = (label*)realloc(labels, (label_count + 1) * sizeof(label));
				if (!labels)
				{
					free(code_arr);
					free(data_arr);
					free_macro_list(macro);
					free(labels);
					fclose(am);
					printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
					exit(1);
				}
				labels=insert_label(labels, label_name, label_count, data, DC, row_count,&flag);
			}

			/* Check if memory is exceeded */
			if((IC+DC-1)>MEMORY_SIZE)
			{
				free(code_arr);
				free(data_arr);
				free_macro_list(macro);
				free(labels);
				fclose(am);
				printf("ERROR in file %s, in line %d : The encoding stopped because the memory is filled", file_name, row_count);
				exit(1);
			}
			/* Encode the data directive */
			valid_check=data_codeing(line, first_word, row_count, &flag, data_arr, file_name);
			if(valid_check==FALSE)
			{
				free(code_arr);
				free(data_arr);
				free_macro_list(macro);
				free(labels);
				fclose(am);
				exit(1);
			}
	
			continue;/* Move to the next line */
		}
		if (strcmp(first_word, ".extern") == ZERO)
		{
			line=skip_white_chars(line);
			cut_word2(line, SPACE, TAB, label_name);/* Extract the external label name */
			if (is_valid_lable(label_name, row_count, labels, label_count,macro, file_name,&flag))
			{
				flag_label = TRUE;
				label_count++;
				labels = (label*)realloc(labels, (label_count + 1) * sizeof(label));
				if (!labels)
				{
					free(code_arr);
					free(data_arr);
					free_macro_list(macro);
					free(labels);
					fclose(am);
					printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
					exit(1);
				}
				labels=insert_label(labels, label_name, label_count, external, ZERO, row_count, &flag);
				line=skip_white_chars(line);
				/* Check for invalid characters after .extern */
				if (*line != '\0')
				{
					printf("ERROR in file %s : Invalid chars after use of extern in line %d\n",file_name, row_count);
					flag =TRUE;
				}
					
				
			}
			continue;/* Move to the next line */
		}
		if (strcmp(first_word, ".entry") == ZERO)
		{
			continue;/* Move to the next line */
		}
		/* Store label for code segment if present */
		if (flag_label == TRUE)
		{
			labels= (label*)realloc(labels, (label_count + 1) * sizeof(label));
			if (!labels)
				{
					free(code_arr);
					free(data_arr);
					free_macro_list(macro);
					free(labels);
					fclose(am);
					printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
					exit(1);
				}
			labels=insert_label(labels, label_name, label_count, code, IC, row_count, &flag);
		}
		/*Check if first_word is a name of a command*/
		for (i = 0; i < NUM_COMMANDS; i++)
		{
			if (strcmp(commands[i].name, first_word) == ZERO)
			{
				/* Default addressing method */
				source_address = non;
				destination_address = non;
 				/* Check if valid and extract operands */
				valid_check=commands[i].func(line,row_count , &flag, file_name, &source_address, &destination_address, source_operand, destination_operand);
				if (valid_check==TRUE)
				{
					/* Check memory bounds */
					if((IC+DC-1)>MEMORY_SIZE)
					{
						free(code_arr);
						free(data_arr);
						free_macro_list(macro);
						free(labels);
						fclose(am);
						printf("ERROR in file %s, in line %d : The encoding stopped  because the memory is filled", file_name, row_count);
						exit(1);
					}
					
					instruction_codeing(first_word, source_address, destination_address, source_operand, destination_operand, code_arr, IC, commands,row_count);
					IC++;
					/* Handle source operand addressing */
					if (source_address == direct || source_address==relative)
					{
						if((IC+DC-1)>MEMORY_SIZE)
						{
							free(code_arr);
							free(data_arr);
							free_macro_list(macro);
							free(labels);
							fclose(am);
							printf("ERROR in file %s, in line %d : The encoding stopped  because the memory is filled", file_name, row_count);
							exit(1);
						}
						save_label_operand(source_operand, code_arr,row_count);
						IC++;
					}
					else
					{
						if (source_address == instant)
						{
							if((IC+DC-1)>MEMORY_SIZE)
							{
								free(code_arr);
					                        free(data_arr);
								free_macro_list(macro);
								free(labels);
								fclose(am);
								printf("ERROR in file %s, in line %d : The encoding stopped because the memory is filled", file_name, row_count);
								exit(1);
							}
							instant_codeing(source_operand, code_arr,IC, row_count);
							IC++;
						}
					}
					/* Handle destination operand addressing */
					if (destination_address == direct || destination_address==relative)
					{
						if((IC+DC-1)>MEMORY_SIZE)
						{
							free(code_arr);
							free(data_arr);
							free_macro_list(macro);
							free(labels);
							fclose(am);
							printf("ERROR in file %s, in line %d : The encoding stopped because the memory is filled", file_name, row_count);
							exit(1);
						}
						
						save_label_operand(destination_operand, code_arr, row_count);
						IC++;
					}
					else
					{
						if (destination_address == instant)
						{
							if((IC+DC-1)>MEMORY_SIZE)
							{
								free(code_arr);
								free(data_arr);
								free_macro_list(macro);
								free(labels);
								fclose(am);
								printf("ERROR in file %s, in line %d : The encoding stopped  because the memory is filled", file_name, row_count);
								exit(1);
							}
						
							instant_codeing(destination_operand, code_arr,IC, row_count);
							IC++;
						}
					}
					
					
				}
				else if(valid_check==MEMORY_ALLOCATION_FAIL)
				{
					free(code_arr);
					free(data_arr);
					free_macro_list(macro);
					free(labels);
					fclose(am);
					printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
					exit(1);
				}
				break;/* Command found and processed, exit loop */
			}
		}

		/* If no valid command was found */
		if (i == NUM_COMMANDS)
		{
			printf("ERROR in file %s in line %d : Invalid line\n",file_name, row_count);
			flag = TRUE;
		}
	
	}
        free_macro_list(macro);
	if (flag == FALSE)
	{
		update_data_address(data_arr,labels, label_count);

		/* Proceed to the second pass */
		second_pass(am,file_name, code_arr, data_arr,labels, label_count,IC,DC);
	}

}


void save_label_operand(char *operand_name, codes* code_arr,int row)
{
	/* Mark the current code entry as unfinished (requires resolution in second pass) */
	code_arr[IC - START_ADDRESS].finish = no;
	code_arr[IC - START_ADDRESS].address = IC;
	strcpy(code_arr[IC - START_ADDRESS].code, operand_name);
	/* Store the source file row number for debugging or error reporting */
	code_arr[(IC)-START_ADDRESS].row = row;

}


int data_codeing(char* line, char * first_word, int row_count, int* flag, data_code* data_arr, char *file_name)
{
	int i = 0;
	char ch_help[MAX_CHAR_IN_LINE];
	char machine_code_string[MAX_CHAR_LBL+TWO];
	char* end = (char*)malloc(sizeof(char) * 2);
	if (!end)
	{
		free(end);
		printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
		return FALSE;
	}
	/* Validate the data or string directive line */
	if (valid_data_line(line, first_word, row_count, file_name))
	{
		/* Handle .data directive: convert comma-separated numbers to binary */
		if (strcmp(first_word, ".data") == ZERO)
		{
			while (*line != '\0')
			{
				cut_word(line, COMMA, ch_help);/* Extract next number before comma */
				data_arr[DC].address = DC;
				int_to_binary(atoi(ch_help), machine_code_string, CODE_WORD_SIZE-1);
				strcpy(data_arr[DC].code, machine_code_string);
				DC++;
			}
		}
		/* Handle .string directive: convert each character to binary */
		if (strcmp(first_word, ".string")==ZERO)
		{
			i++;
			while (line[i] != '\0')
			{
				data_arr[DC].address = DC;
				int_to_binary((int)line[i], machine_code_string, CODE_WORD_SIZE-1);
				strcpy(data_arr[DC].code, machine_code_string);
				DC++;
				i++;
			}
			/*Encode '\0' of end of string*/
			data_arr[DC].address = DC;
			int_to_binary((int)line[i], machine_code_string, CODE_WORD_SIZE-1);
			strcpy(data_arr[DC].code, machine_code_string);
			DC++;
		}

	}
	else
	{
		*flag = TRUE;
	}
	return TRUE;

}


void update_data_address(data_code *data_arr,label* labels, int label_count)
{
	int i;
	/* Update addresses of labels marked as data type */
	for (i = 1; i<=label_count; i++)
	{
		if (labels[i].type == data)
		{
			labels[i].address += IC-1;/* Adjust label address by adding final IC*/
		}
	}
	/* Update addresses of all data array entries */
	for (i = 1; i <= DC; i++)
	{
		data_arr[i].address += IC-1;/* Adjust data address by adding final IC*/
	}
}


int valid_data_line(char* line, char* first_word, int row_count, char *file_name)
{
	char help_line[MAX_CHAR_IN_LINE];
	char* ch;
	char ch_help[MAX_CHAR_IN_LINE];
	/* Handle .string directive validation */
	if (strcmp(first_word, ".string") == ZERO)
	{
		line = skip_white_chars(line);
		line = skip_white_chars_from_end(line);
		ch = line;
		if (*ch != '"')/* Check if string starts with a quotation mark */
		{
			printf("ERROR in file %s, in line %d : Invalid chars, string should start and end with quotation marks\n", file_name, row_count);
			return FALSE;
		}
		while (*ch != '\0')
		{
			ch++;
		}
		ch--;/* Step back to the last character */
		if (*ch != '"')/* Check if string ends with a quotation mark */
		{
			printf("ERROR in file%s, in line %d : Invalid chars, string sould start and end with quotation marks\n", file_name, row_count);
			return FALSE;
		}
		
		line++;/* Skip opening quote */
		line[strlen(line)-1] = '\0';/* Remove closing quote by null-terminating early */
		return TRUE;
	}
	else/* Handle .data directive validation */
	
	{
		strcpy(help_line, line);
		line = skip_white_chars(line);
	
		if (!isdigit(*line) && (*line) != '+' && (*line) != '-')
		{
			printf("ERROR in file %s, in line %d : Invalid chars for data\n", file_name, row_count);
			return FALSE;
		}
		/* Process each comma-separated value */
		while (*line != '\0')
		{
			cut_word(line, COMMA, ch_help);/* Extract next value before comma */
		
			ch = ch_help;
			/* Verify that the extracted value is a valid number */
			if (!is_number(ch))
			{
				printf("ERROR in file %s, in line %d : Invalid chars, data  only accepts numbers.\n", file_name, row_count);
				return FALSE;
			}
		}
		strcpy(line, help_line);/* Restore original line */
		return TRUE;

	}
}



int valid_mov_add_sub(char* line, int row_count, int* flag, char* file_name,  address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char first_opernd_help[MAX_CHAR_IN_LINE];
	char second_operand_help[MAX_CHAR_IN_LINE];
	char* first_opernd = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	char* second_operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	if(!first_opernd || !second_operand)
	{
		free(first_opernd);
		free(second_operand);
		return MEMORY_ALLOCATION_FAIL;
	}
	
	line = skip_white_chars(line);
	/* Check if line is empty (missing operands) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	cut_word(line, COMMA, first_opernd_help);

	first_opernd = first_opernd_help;

	/* Handle immediate addressing (starts with '#') */
	if (*first_opernd == HASH_TAG)
	{
		first_opernd++;/* Skip '#' */
		if (!isdigit(*first_opernd) && (*first_opernd) != '+' && (*first_opernd) != '-')
		{
			*flag = TRUE;
			printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
			return FALSE;
		}
		else
		{
			if ((*first_opernd) == '+' || (*first_opernd) == '-')
				first_opernd++;
			if (!is_number(first_opernd))/* Verify the remaining string is a valid number */
			{
				*flag = TRUE;
				printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
				return FALSE;
			}
			else
			{
				*source_address = instant;
				strcpy(source_operand, first_opernd);
			}
		}
	}
	else/* Handle register or direct addressing */
	{
		if (is_register(first_opernd))
		{
			remove_first_char(first_opernd);/* Remove register prefix (e.g., 'r') */
			*source_address = direct_register;
			strcpy(source_operand, first_opernd);
		}
		else
		{
			if (is_valid_lable_pattern(first_opernd))
			{
				*source_address = direct;
				strcpy(source_operand, first_opernd);
			}
			else
			{
				printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
				*flag = TRUE;
				return FALSE;
			}
		}
	}
	/* Check if second operand is missing */
	if (is_empty_string(line))
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d : Missing a second operand\n", file_name, row_count);
		return FALSE;
	}
	line = skip_white_chars(line);
	cut_word2(line, SPACE, TAB, second_operand_help);/* Extract second operand */
	second_operand = second_operand_help;
	/* Check for extra characters after second operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	/* Validate second operand */
	if (is_register(second_operand))
	{
		remove_first_char(second_operand);
		*destination_address = direct_register;
		strcpy(destination_operand, second_operand);
	}
	else
	{
		if (is_valid_lable_pattern(second_operand))
		{
			*destination_address = direct;
			strcpy(destination_operand, second_operand);
		}
		else
		{
			printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
			*flag = TRUE;
			return FALSE;
		}
	}
	
	return TRUE;/* Operands are valid */
}

int valid_cmp(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char first_opernd_help[MAX_CHAR_IN_LINE];
	char second_operand_help[MAX_CHAR_IN_LINE];
	char* first_opernd = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	char* second_operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	if(!first_opernd || !second_operand)
	{
		free(first_opernd);
		free(second_operand);
		return MEMORY_ALLOCATION_FAIL;
	}
	line = skip_white_chars(line);
	/* Check if line is empty (missing operands) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	cut_word(line, COMMA, first_opernd_help);/* Extract first operand before comma */
	first_opernd = first_opernd_help;
	/* Handle immediate addressing for source operand (starts with '#') */
	if (*first_opernd == HASH_TAG)
	{
		first_opernd++;/* Skip '#' */
		if (!isdigit(*first_opernd) && (*first_opernd) != '+' && (*first_opernd) != '-')
		{
			*flag = TRUE;
			printf("ERROR in file %s, in line %d : Unvalid char after '#'.\n", file_name, row_count);
			return FALSE;
		}
		else
		{
			if (!is_number(first_opernd))/* Verify the remaining string is a valid number */
			{
				*flag = TRUE;
				printf("ERROR in file %s, in line %d : Unvalid char after '#'.\n", file_name, row_count);
				return FALSE;
			}
			else
			{
				*source_address = instant;
				strcpy(source_operand, first_opernd);
			}
		}
	}
	else /* Handle register or direct addressing for source operand */
	{
		if (is_register(first_opernd))
		{
			remove_first_char(first_opernd);/* Remove register prefix*/
			*source_address = direct_register;
			strcpy(source_operand, first_opernd);
		}
		else
		{
			if (is_valid_lable_pattern(first_opernd))
			{
				*source_address = direct;
				strcpy(source_operand, first_opernd);
			}
			else
			{
				printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
				*flag = TRUE;
				return FALSE;
			}
		}
	}
	/* Check if second operand is missing */
	if (is_empty_string(line))
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d : Missing a second operand\n", file_name, row_count);
		return FALSE;
	}
	line = skip_white_chars(line);
	cut_word2(line, SPACE, TAB, second_operand_help);/* Extract second operand */
	second_operand = second_operand_help;
	/* Check for extra characters after second operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	/* Handle immediate addressing for destination operand (starts with '#') */
	if (*second_operand == '#')
	{
		second_operand++;/* Skip '#' */
		if (!isdigit(*second_operand) && (*second_operand) != '+' && (*second_operand) != '-')
		{
			*flag = TRUE;
			printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
			return FALSE;
		}
		else
		{
			if (!is_number(second_operand))/* Verify the remaining string is a valid number */
			{
				*flag = TRUE;
				printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
				return FALSE;
			}
			else
			{
				*destination_address = instant;
				strcpy(destination_operand, second_operand);
			}
		}
	}
	else/* Handle register or direct addressing for destination operand */
	{
		if (is_register(second_operand))
		{
			remove_first_char(second_operand);/* Remove register prefix */
			*destination_address = direct_register;
			strcpy(destination_operand, second_operand);
		}
		else
		{
			if (is_valid_lable_pattern(second_operand))
			{
				*destination_address = direct;
				strcpy(destination_operand, second_operand);
			}
			else
			{
				printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
				*flag = TRUE;
				return FALSE;
			}
		}
	}
	return TRUE;/* Operands are valid */
	
}

int valid_lea(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char* first_opernd = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	char* second_operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	char first_opernd_help[MAX_CHAR_IN_LINE];
	char second_operand_help[MAX_CHAR_IN_LINE];
	line = skip_white_chars(line);
	/* Check if line is empty (missing operands) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	cut_word(line, COMMA, first_opernd_help);
	first_opernd = first_opernd_help;
	if (is_valid_lable_pattern(first_opernd))/* Validate source operand (must be a label) */
	{
		*source_address = direct;
		strcpy(source_operand, first_opernd);
	}
	else
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d : Invalid operand '#'.\n", file_name, row_count);
		return FALSE;
	}
	/* Check if second operand is missing */
	if (is_empty_string(line))
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d : Missing a second operand\n", file_name, row_count);
		return FALSE;
	}
	line = skip_white_chars(line);
	line = skip_white_chars(line);
	cut_word2(line, SPACE, TAB, second_operand_help);/* Extract second operand */
	second_operand = second_operand_help;
	/* Check for extra characters after second operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	if (is_register(second_operand))
	{
		remove_first_char(second_operand);/* Remove register prefix*/
		*destination_address = direct_register;
		strcpy(destination_operand, second_operand);
	}
	else/* Check if it's a valid label */
	{
		if (is_valid_lable_pattern(second_operand))
		{
			*destination_address = direct;
			strcpy(destination_operand, second_operand);
		}
		else
		{
			printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
			*flag = TRUE;
			return FALSE;
		}
	}
	return TRUE;/* Operands are valid */
}

int valid_clr_not_inc_dec_red(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char opernd_help[MAX_CHAR_IN_LINE];
	char* operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	line = skip_white_chars(line);
	/* Check if line is empty (missing operand) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	operand = line;

	cut_word2(operand, TAB, SPACE, opernd_help);/* Extract operand */
	operand = opernd_help;
	/* Validate the operand */
	if (is_register(operand))
	{
		remove_first_char(operand);/* Remove register prefix*/
		*destination_address = direct_register;
		strcpy(destination_operand, operand);
	}
	else
	{
		if (is_valid_lable_pattern(operand))
		{
			*destination_address = direct;
			strcpy(destination_operand, operand);
		}
		else
		{
			printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
			*flag = TRUE;
			return FALSE;
		}
	}
	/* Check for extra characters after the operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	else
		return TRUE; /* Operand is valid */
}


int valid_jmp_bne_jsr(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char opernd_help[MAX_CHAR_IN_LINE];
	char* operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	int relative_flag = FALSE;
	line = skip_white_chars(line);
	/* Check if line is empty (missing operand) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	operand = line;
	cut_word2(operand, TAB, SPACE, opernd_help); /* Extract operand*/
	operand = opernd_help;
	/* Check for relative addressing indicator '&' */
	if (*operand=='&')
	{
		relative_flag = TRUE;
		operand++;/* Skip '&' to process the label */
	}
	/* Validate the operand (must be a label) */
	if (is_valid_lable_pattern(operand))
	{
		if (relative_flag)
		{
			*destination_address = relative;
			strcpy(destination_operand, opernd_help);
		}
		else
		{
			*destination_address = direct;
			strcpy(destination_operand, operand);
		}
	}
	else
	{
		printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	/* Check for extra characters after the operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	else
		return TRUE;/* Operand is valid */
}

int valid_prn(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	char opernd_help[MAX_CHAR_IN_LINE];
	char* operand = (char*)malloc(sizeof(char) * MAX_CHAR_LBL);
	line = skip_white_chars(line);
	/* Check if line is empty (missing operand) */
	if (*line == '\0')
	{
		*flag = TRUE;
		printf("ERROR in file %s, in line %d: Missing operands after the command.\n",file_name, row_count);
		return FALSE;
	}
	operand = line;
	cut_word2(operand, TAB, SPACE, opernd_help);/* Extract operand*/
	operand = opernd_help;

	/* Handle immediate addressing (starts with '#') */
	if (*operand == HASH_TAG)
	{
		operand++;/* Skip '#' */
		if (!isdigit(*operand) && (*operand) != '+' && (*operand) != '-')
		{
			*flag = TRUE;
			printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
			return FALSE;
		}
		else
		{
			/* Verify the remaining string is a valid number */
			if (!is_number(operand))
			{
				*flag = TRUE;
				printf("ERROR in file %s, in line %d : Invalid char after '#'.\n", file_name, row_count);
				return FALSE;
			}
			else
			{
				*destination_address = instant;
				strcpy(destination_operand, operand);
			}
		}
	}
	else/* Handle register or direct addressing */
	{
		if (is_register(operand))
		{
			remove_first_char(operand);/* Remove register prefix */
			*destination_address = direct_register;
			strcpy(destination_operand, operand);
		}
		else
		{
			if (is_valid_lable_pattern(operand))
			{
				*destination_address = direct;
				strcpy(destination_operand, operand);
			}

			else
			{
				printf("ERROR in file %s, in line %d : Invalid operand.\n", file_name, row_count);
				*flag = TRUE;
				return FALSE;
			}
		}
	}
	
	/* Check for extra characters after the operand */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after operand.\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	else
		return TRUE;/* Operand is valid */
}


int valid_rts_stop(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand)
{
	/* Check if there are any characters after the command */
	if (!is_empty_string(line))
	{
		printf("ERROR in file %s, in line %d : Invalid chars after command .\n", file_name, row_count);
		*flag = TRUE;
		return FALSE;
	}
	else
		return TRUE;/* Line is empty, command is valid */
}


