#include"mcr.h"


macro_list* macro_conversion(FILE* as, char* file_name, FILE* am)
{
    int row_count = ZERO;
    int flag = FALSE;
    char str[MAX_CHAR];
    char str2[MAX_CHAR];
    char *str3=NULL;
    char str_help[MAX_CHAR];
    char* new_word = (char*)malloc(MAX_CHAR_IN_MACRO_NAME * sizeof(char));
    macro_list* macro = (macro_list*)malloc(sizeof(macro_list));
    macro_list* current = (macro_list*)malloc(sizeof(macro_list));
    if (!macro || !current)
    {
	printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
        free(macro);
        free(current);
        return NULL;
    }
    macro->name = NULL;
    macro->content = (list_content*)malloc(sizeof(list_content)); 
    macro->content->value = NULL;
    macro->content->next = NULL;
    macro->next = NULL;

    current->name = NULL;
    current->content = (list_content*)malloc(sizeof(list_content)); 
    current->content->value = NULL;
    current->content->next = NULL;
    current->next = NULL;

    if (!macro->content || !current->content)
    {
        printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,row_count);
        free(macro->content);
        free(current->content);
        free(macro);
        free(current);
        return NULL;
    }
    if (as == NULL) {
        printf("Error: file pointer of %s is NULL!\n",file_name);
        free_macro_list(macro);
        free_macro_list(current);
        return NULL;
    }

    /* Process each line from the input file */
    while (fgets(str, MAX_CHAR, as) != NULL)
    {
        strcpy(str_help, str);
        if (is_empty_string(str_help))/* Skip empty lines */
        {
            fprintf(am, "%s", str);
            continue;
        }
        strcpy(str2, str);
        row_count++;
        current = is_macro(str2, macro);
        strcpy(str2, str);
        if (current != NULL)
        {
            write_macro(current, am);
        }
        else if (strcmp(strtok(str2, " "), "mcro") == ZERO)/* Check if line defines a macro */
        {
            new_word = strtok(NULL, SPACE_TAB);/* Extract macro name */
	    str3 = strtok(NULL, SPACE_TAB);/* Check for extra charecters */
            if (str3!=NULL && *str3 != '\n')
            {
            	flag = TRUE;
    		printf("ERROR in file %s, in line %d : Invalid chars after mcro\n", file_name, row_count);
		fprintf(am, "%s", str);
		continue;
            }
            macro = is_new_macro(new_word, macro, &flag, as, &row_count, file_name);
            if(macro==NULL)
            {
            	free_macro_list(current);
            	return NULL;
            }
            if (flag == TRUE)
            {
                fprintf(am, "%s", str);
            }
        }
        else/* Handle non-macro lines */
        {
		strcpy(str_help, str);
		cut_word2(str_help, TAB, SPACE, str2);/* Extract first word */
		if (strcmp(str2, "mcroend") == ZERO && !is_empty_string(str_help))/* Check for invalid mcroend */
		{
    			printf("ERROR in file %s, in line %d : Invalid chars after mcroend.\n", file_name, row_count);
    			flag = TRUE;
		}
            	fprintf(am, "%s", str);
        }


    }
    
    if (flag == FALSE)
    {
        return macro;
    }
    remove(file_name);/* Remove output file if errors occurred */
    return NULL;
}


macro_list* is_macro(char* str, macro_list* macro)
{
    char* c;
    macro_list* current1 = macro;
    if (is_empty_string(str))
    {
        return NULL;/* Empty string cannot be a macro */
    }

    /* Check if macro list is empty */
    if ((macro->name) == NULL)
    {
        current1 = NULL;
    }
    
    /* Extract the first word from the string */
    c = strtok(str, " ");
    if (current1 != NULL)
    {
	/* Search the macro list to find a match */
        while (current1 != NULL)
        {
            if (strcmp(c, (current1->name)) == 0)
            {
                return current1;
            }
            current1 = current1->next;/* Move to next macro */
        }
    }
    return NULL;/* No match found */
}

macro_list* conect_marco_list(macro_list* macro, macro_list* current)
{
    macro_list* lst1 = macro;
    if (current == NULL)
        return NULL;

    /* If macro list is empty, set current as head */
    if (macro->name == NULL)
    {
        macro = current;
        return macro;
    }

    /* Find the end of the macro list */
    while (lst1->next != NULL)
    {
        lst1 = lst1->next;
    }

    /* Connect current to the end of the list */
    lst1->next = current;

    return macro;
}


macro_list* is_new_macro(char* str, macro_list* macro, int* flag, FILE* as, int* row_count, char* file_name)
{
    macro_list* new_link = (macro_list*)malloc(sizeof(macro_list));
    if (!new_link)
    {
        free(new_link);
        printf("ERROR in file %s: Memory allocation failed in row %d.\n",file_name,*row_count);
        return NULL;
    }

    new_link->name = (char*)malloc(MAX_CHAR_IN_MACRO_NAME * sizeof(char));
    if (!new_link->name)
    {
        free(new_link->name);
        free(new_link);
        printf("ERROR in file %s: Memory allocation failed in row %d.\n",file_name,*row_count);
        return NULL;
    }

    /* Validate the macro name */
    if (!is_valid_macro_name(flag, str, *row_count,file_name))
        return macro;

    /* Check if macro name already exists */
    if (if_exsit_macro(str, macro))
    {
        printf("ERROR in file %s, in line %d :A macro with this name already exists.", file_name, *row_count);
        *flag = TRUE;
        return macro;
    }

    
    
    strcpy(new_link->name, str);

    new_link->content = (list_content*)malloc(sizeof(list_content));
    if (!new_link->content)
    {
        free(new_link->content);
        free(new_link);
        printf("ERROR in file %s: Memory allocation failed in row %d.\n",file_name,*row_count);
        return NULL;
    }
    new_link->content->value = NULL;
    new_link->content->next = NULL;

    new_link->content = create_content_list(as, row_count, flag, file_name);
    if (!new_link->content)
    {
        free_macro_list(new_link);
        printf("ERROR in file %s: Memory allocation failed in row %d.\n",file_name,*row_count);
        return NULL;
    }
    new_link->next = NULL;

    /* Connect new macro to the list */
    macro = conect_marco_list(macro, new_link);
   
    
    return macro;

}



void write_macro(macro_list* current, FILE* am)
{
    list_content* current1 = current->content;

    /* Write each content line to the output file */
    while (current1 != NULL)
    {
        fprintf(am, "%s", current1->value);
        current1 = current1->next;
    }
}

int is_valid_macro_name(int* flag, char* str, int row_count,char *file_name)
{
    char help[MAX_CHAR_IN_LINE];
    int len = strlen(str);
    char* s;
    char str1[MAX_CHAR];
    int not_valid=FALSE;
    strcpy(str1, str);
    s = str1;
    str1[len - 1] = '\0';
    if (strlen(s) > MAX_CHAR_IN_MACRO_NAME)
    {
        printf("ERROR in file %s in line %d :The macro name is too long\n.",file_name, row_count);
        *flag = TRUE;
	not_valid=TRUE;
    }

    /* Verify each character is alphabetic or underscore */
    while (*s != '\0')
    {
        if (isalpha(*s) || (*s) == '_')
        {
            s++;
        }
        else
        {
            printf("ERROR in file %s in line %d :The macro name contains invalid characters.\n",file_name, row_count);
            *flag = TRUE;
	    not_valid=TRUE;
            break;
        }

    }

    if (is_register(str1))
    {
        printf("ERROR in file %s in line %d :The macro name is the name of a registers.\n",file_name, row_count);
        *flag = TRUE;
	not_valid=TRUE;
    }
    if (is_command(str1))
    {
        printf("ERROR in file %s in line %d :The macro name is the name of a command.\n",file_name, row_count);
        *flag = TRUE;
	not_valid=TRUE;
    }
   
    if (strcmp(str1, "string") == 0 || strcmp(str1, "data") == 0 || strcmp(str1, "extern") == 0 || strcmp(str1, "entry") == 0)
    {
        
        printf("ERROR in file %s in line %d :The macro name is the name of a directive.\n",file_name, row_count);
        *flag = TRUE;
	not_valid=TRUE;

    }
    cut_word2(str1, SPACE, TAB, help);
    if (!is_empty_string(str1))
    {
        printf("ERROR in file %s in line %d :Invalid chars after macro name.\n",file_name, row_count);
        *flag = TRUE;
	not_valid=TRUE;
    }

    /* Return validation result */
    if (not_valid == TRUE)
    {
    	return FALSE;
    }
    return TRUE;
}


void free_macro_list(macro_list* macro)
{
    macro_list* temp;

    /* Free each node in the macro list */
    while (macro != NULL)
    {
        temp = macro;
        free_list_content(macro->content);
        free(macro->name);
        macro = macro->next;
        free(temp);
    }
}

list_content* create_content_list(FILE* as, int* row_count, int* flag, char* file_name)
{

    char help[MAX_CHAR_IN_LINE];
    list_content* lst;
    list_content* current;

    char str[MAX_CHAR];
    char ptr[MAX_CHAR];
    
    /* Read first line after macro definition */
    fgets(str, MAX_CHAR, as);
    (*row_count)++;
    lst = (list_content*)malloc(sizeof(list_content));
    if (!lst)
    {
        printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,*row_count);
        free(lst);
        return NULL;
    }
    lst->value = (char*)malloc(strlen(str) + 1); 
    if (!lst->value)
    {
	printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,*row_count);
        free(lst->value);
        free(lst);
        return NULL;
    }
    current = (list_content*)malloc(sizeof(list_content));
    if (!current)
    {
	printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,*row_count);
        free(current);
        free(lst->value);
        free(lst);
        return NULL;
    }
    current->value = (char*)malloc(strlen(str) + 1);
    if (!current->value)
    {
	printf("ERROR in file %s: Memory allocation failed in row %d\n",file_name,*row_count);
        free(current->value);
        free(current);
        free(lst->value);
        free(lst);
        return NULL;
    }

    /* Store first line in head node */
    strcpy(lst->value, str);
    lst->next = NULL;

    /* Read subsequent lines until mcroend */
    while (fgets(str, MAX_CHAR, as))
    {

        strcpy(ptr, str);
        strcpy(ptr, skip_white_chars(ptr));
		strcpy(ptr, skip_white_chars_from_end(ptr));
        if (strcmp("mcroend", ptr) != ZERO)
        {
            (*row_count)++;
            strcpy(current->value, str);
            current->next = NULL;
            connect_list(lst, current);
            cut_word2(ptr, TAB, SPACE, help);/* Check for mcroend with extra characters */
            if (strcmp(help, "mcroend") == ZERO)
            {
                printf("ERROR in file %s, in line %d : Invalid chars after mcroend.\n", file_name, *row_count);
                *flag = TRUE;
                break;
            }

        }
        else/* End of macro definition */
        {
            break;
        }

    }

    /* Return content list */
    return lst;
}

void connect_list(list_content* lst, list_content* current)
{
    list_content* lst1 = lst;

    /* Find the end of the content list */
    while (lst1->next != NULL)
    {
        lst1 = lst1->next;
    }

    /* Connect current to the end */
    lst1->next = current;
}

void free_list_content(list_content* list)
{

    list_content* temp;

    /* Free each node in the content list */
    while (list != NULL)
    {
        temp = list;
        free(list->value);
        list = list->next;
        free(temp);
    }
}


int if_exsit_macro(char* name, macro_list* macro)
{
    macro_list* temp = macro;
	if (temp == NULL)
	{
		return FALSE;
	}

    /* Search for macro with matching name */
    while ( temp !=NULL && temp->name != NULL)
    {
        if (strcmp(temp->name, name) == ZERO)
        {
            return TRUE;
        }
		temp = temp->next;
    }
    return FALSE;/* No match found */
}

