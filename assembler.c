#include "assembler.h"

/*Maman 14- Final project.
Written by Zipi Shechter (i.d: 215114273) and Miryam Volpo (i.d: 209784925)*/

/*This is the primary function that processes assembly input files written in a specific language as defined by the project's requirements.

The function then forwards them for analysis.

@param argc The number of command - line arguments.

@param argv An array of strings containing the command - line arguments.

@return 0 if the process succeeded.
*/
int main(int argc, char* argv[])
{
    int i;
    char file_name[MAX_CHAR_IN_LINE];
    FILE* as, * am;
    macro_list* macro=NULL;
    
    /* Check if any file names were provided as arguments */
    if (argc < TWO)
    {  
        printf("No files entered");
        return ONE;  
    }


    /*Iterate over each file name provided in the command line*/
    for(i=1;i<argc;i++)
    {
        /* Copy the original file name and validate its format (by adding .as)*/
        strcpy(file_name, argv[i]);
        strcpy(file_name, valid_file_name(file_name));
	
	/* Skip file if name is invalid */
        if (file_name == NULL)
            continue;
        if ((as = search_file(file_name)))
        {
            change_ending(file_name, "am");
	    /* Create a new file for macro-expanded output */
            am = create_file(file_name);
            
            /* Process macros and write the expanded content to .am file */
            macro = macro_conversion(as, file_name, am);
	    fclose(as);
	    /* If macros were successfully processed, continue to first pass */
            if (macro != NULL)
            {
                first_pass(am, file_name, macro);
            }

        }

    }

    return ZERO;
}
