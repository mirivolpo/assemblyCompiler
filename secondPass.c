
#include "secondPass.h"


void second_pass(FILE* am, char* file_name, codes *code_arr, data_code* data_arr, label *labels, int label_count,int IC, int DC)
{

	int ext_count = 0, ent_count = 0;
	ext_ent_lbl* ext_ent_lbl_arr = NULL;
	
	int flag = FALSE;
	/* Read the file and update labels (mark entry labels) */
	read_from_file(am, file_name, labels, label_count,&flag);
	fclose(am);

	/* Identify external and entry labels and populate ext_ent_lbl_arr */
	ext_ent_lbl_arr=find_ext_ent(data_arr,code_arr, labels, label_count, &flag,IC, &ext_count,&ent_count,file_name);
	free(labels);
	if (!flag)
	{
		create_output_files(code_arr, data_arr, IC, DC, file_name, ext_ent_lbl_arr,  ext_count, ent_count);
	}
	
	/* Free allocated memory for code, data, and external/entry label arrays */
	free(code_arr);
	free(data_arr);
	free(ext_ent_lbl_arr);
}

void read_from_file(FILE* am, char* file_name, label* labels, int label_count, int *flag)
{
	char line[MAX_CHAR_IN_LINE];
	char* first_word;
	char first_word_help[MAX_CHAR_IN_LINE];
	int row_count = 0;
	rewind(am);

	/* Process each line in the file */
	while (fgets(line, sizeof(line), am) != NULL)
	{
		row_count++;	
		skip_white_chars(line);
		cut_word2(line, SPACE, TAB, first_word_help);
		first_word = first_word_help;
		first_word=skip_white_chars(first_word);
		if (is_lable(first_word))
		{
			cut_word2(line, SPACE, TAB, first_word_help);
			first_word = skip_white_chars(first_word);
		}

		/* Skip lines with .string, .data, or .extern directives */
		if (strcmp(first_word, ".string") == ZERO || strcmp(first_word, ".data") == ZERO || strcmp(first_word, ".extern") == ZERO)
		{
			continue;
		}

		/* Handle .entry directive */
		if (strcmp(first_word, ".entry") == ZERO)
		{
			cut_word2(line, SPACE, TAB, first_word_help);
			first_word = skip_white_chars(first_word);
			update_entry(labels, label_count, first_word, row_count, file_name, flag);
			if (!is_empty_string(line))/* Check for extra characters after label */
			{
				printf("ERROR in file %s, in line %d : Invalid chars after .entry", file_name, row_count);
				*flag = TRUE;
			}
		}
	}
}

ext_ent_lbl* find_ext_ent(data_code* data_arr, codes* code_arr, label* labels, int label_count,int *flag,  int IC, int *ext_count, int *ent_count, char * file_name)
{
	int i=0,ext=0,ent=0,total=0;
	int j;
	char* label_name;
	ext_ent_lbl* temp = NULL;
	ext_ent_lbl* ext_ent_lbl_arr = (ext_ent_lbl*)malloc(sizeof(ext_ent_lbl));
	label_name = (char*)malloc(MAX_CHAR_LBL * sizeof(char));
	
	if (!label_name)
	{
		free(code_arr);
		free(data_arr);
		free(label_name);
		free(labels);
		printf("ERROR in file %s: Memory allocation failed in function find_ext_ent\n",file_name);
		exit(1);
	}
	if (!ext_ent_lbl_arr)
	{
		free(code_arr);
		free(data_arr);
		free(ext_ent_lbl_arr);
		free(label_name);
		free(labels);
		printf("ERROR in file %s: Memory allocation failed in function find_ext_ent\n",file_name);
		exit(1);
	}
	
	
	/* Scan labels for entry types */	
	for (i = 1; i <= label_count; i++)
	{
		if (labels[i].type == code_entry || labels[i].type == data_entry)
		{
			total = ext + ent;
			temp = (ext_ent_lbl*)realloc(ext_ent_lbl_arr, (total + 2) * sizeof(ext_ent_lbl));
			if (!temp)
			{
				free(code_arr);
				free(data_arr);
				free(temp);
				free(ext_ent_lbl_arr);
				free(label_name);
				free(labels);
				printf("ERROR in file %s: Memory allocation failed in function find_ext_ent\n",file_name);
				exit(1);
			}
			else
			{

				ext_ent_lbl_arr = temp;
				strcpy(ext_ent_lbl_arr[total + 1].name, labels[i].name);
				ext_ent_lbl_arr[total + 1].address = labels[i].address;
				ext_ent_lbl_arr[total + 1].type = ent_lbl;
				ent++;
			}
		}
	}

	/* Scan code array for external label references */
	for (i = 0; i < IC - START_ADDRESS; i++)
	{
		if (code_arr[i].finish == no)
		{
			strcpy(label_name, code_arr[i].code);
			label_codeing(code_arr, i, code_arr[i].code, labels, label_count, code_arr[i - 1].address, flag, code_arr[i].row, file_name);
			if (*label_name == '&')
				label_name++;
			for (j = 1; j <= label_count; j++)
			{
				if (strcmp(label_name, labels[j].name) == ZERO)
				{
					if (labels[j].type == external)
					{
						total = ext + ent;
						temp = (ext_ent_lbl*)realloc(ext_ent_lbl_arr, (total+2) * sizeof(ext_ent_lbl));
						if (!temp)
						{
							free(code_arr);
							free(data_arr);
							free(temp);
							free(ext_ent_lbl_arr);
							free(label_name);
							free(labels);
							printf("ERROR in file %s: Memory allocation failed in function find_ext_ent\n",file_name);
							exit(1);
						}
						else
						{

							ext_ent_lbl_arr = temp;
							strcpy(ext_ent_lbl_arr[total + 1].name, label_name);
							ext_ent_lbl_arr[total + 1].address = code_arr[i].address;
							ext_ent_lbl_arr[total + 1].type = ext_lbl;
							ext++;
							break;
						}	
					}	
						
				}
			}

		}
	}

	/* Update counters */
	*ext_count = ext;
	*ent_count = ent;

	/* Return array of external and entry labels */	
	return ext_ent_lbl_arr;
}


void update_entry(label *labels,int label_count, char* name, int row_count, char *file_name, int* flag)
{
	int i;
	skip_white_chars(name);
        skip_white_chars_from_end(name);

	/* Search for the label in the labels array */
	for (i = 1; i <= label_count; i++)
	{
		if (strcmp(labels[i].name, name) == ZERO)
		{
			if (labels[i].type == code)
				labels[i].type = code_entry;
			if(labels[i].type == data)
				labels[i].type = data_entry;
			break;/* Exit loop once updated */
		}
	}

	if (i > label_count)/* Check if label was not found */
	{
		printf("ERROR in file %s, in line %d : The label does not exist\n",file_name, row_count);
		*flag = TRUE;
	}
}

void create_output_files(codes *code_arr, data_code* data_arr,int IC, int DC, char* file_name, ext_ent_lbl* ext_ent_lbl_arr,  int ext_count, int ent_count)
{
	/* Create .ob file */
	FILE* ob_file,*ext_file=NULL,*ent_file=NULL;
	change_ending(file_name, "ob");
	ob_file = create_file(file_name);
	if (!ob_file)
	{
		free(ext_ent_lbl_arr);;
		printf("ERROR in file %s: Can't create ob file", file_name);
		exit(1);
	}
	write_ob(ob_file, code_arr, data_arr, IC, DC,file_name);

	/* Create .ext file if there are external labels */
	if (ext_count != ZERO)
	{
		change_ending(file_name, "ext");
		ext_file= create_file(file_name);
		if (!ext_file)
		{
			free(ext_ent_lbl_arr);;
			printf("ERROR in file %s: Can't create ext file", file_name);
			exit(1);
		}
		write_ext_ent(ext_file,ext_ent_lbl_arr, ext_count+ent_count,ext_lbl);
	}

	/* Create .ent file if there are entry labels */
	if (ent_count != ZERO)
	{
		change_ending(file_name, "ent");
		ent_file = create_file(file_name);
		if (!ent_file)
		{
			free(ext_ent_lbl_arr);
			printf("ERROR in file %s: Can't create ent file", file_name);
			exit(1);
		}
		write_ext_ent(ent_file,ext_ent_lbl_arr, ext_count + ent_count,ent_lbl);
	}

	/* Close all open files */
	if (ob_file)
		fclose(ob_file);
	if(ent_file)	
		fclose(ent_file);
	if (ext_file)
		fclose(ext_file);


}

void write_ob(FILE* file,codes* code_arr, data_code* data_arr,int IC,int DC,char *file_name)
{
	int i, num = 0;
	char *end;

	/* Write header with code and data sizes */
	fprintf(file,"     %d %d\n", IC - START_ADDRESS, DC-1);

	/* Write code section */
	for (i = 0; i < IC - START_ADDRESS; i++)
	{
		num = strtol(code_arr[i].code, &end, TWO);
		fprintf(file, "%07d %06x\n", code_arr[i].address, num);	
	}

	/* Write data section */
	for (i = 1; i < DC; i++)
	{
		num = strtol(data_arr[i].code, &end, TWO);
		fprintf(file, "%07d %06x\n", data_arr[i].address, num);
	}
	
}

void write_ext_ent(FILE* file,ext_ent_lbl* ext_ent_arr, int ext_ent_count, ext_ent type)
{
	int i;

	/* Write labels of the specified type (external or entry) */
	for (i = 1; i <= ext_ent_count; i++)
	{
		if (ext_ent_arr[i].type == type)
		{
			fprintf(file,"%s %07d\n", ext_ent_arr[i].name, ext_ent_arr[i].address);
		}
	}
}

