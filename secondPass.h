#ifndef SECONDPASS_h  
#define SECONDPASS_h  


#include "machineCode.h"


#define MAX_CHAR_IN_LINE 82

typedef enum
{
   ext_lbl,
   ent_lbl
}ext_ent;

typedef struct ext_ent_lbl
{
    char name[MAX_CHAR_LBL+1];
    int address;
	ext_ent type;
} ext_ent_lbl;


/*
 * second_pass - Executes the second pass of an assembler.
 * @param am: Pointer to the assembly file.
 * @param file_name: Name of the source file.
 * @param code_arr: Array containing the machine code instructions.
 * @param data_arr: Array containing the data section.
 * @param labels: Array of labels.
 * @param label_count: Number of labels.
 * @param IC: Instruction Counter.
 * @param DC: Data Counter.
 */
void second_pass(FILE* am, char* file_name, codes* code_arr, data_code* data_arr, label* labels, int label_count, int IC, int DC);

/*
 * read_from_file - Reads and processes an assembly file.
 * @param am: Pointer to the assembly file.
 * @param file_name: Name of the source file.
 * @param labels: Array of labels.
 * @param label_count: Number of labels.
 * @param flag: Pointer to an error flag.
 */

void read_from_file(FILE* am, char* file_name, label* labels, int label_count, int* flag);

/*
 * find_ext_ent - Finds external and entry labels and stores them.
 *@param data_arr: Array containing the data section.
 * @param code_arr: Array containing the machine code instructions.
 * @param labels: Array of labels.
 * @param label_count: Number of labels.
 * @param flag: Pointer to an error flag.
 * @param IC: Instruction Counter.
 * @param ext_count: Pointer to external label count.
 * @param ent_count: Pointer to entry label count.
 * @param file_name: Name of the source file.
 * @return Pointer to an array of external and entry labels.
 */
ext_ent_lbl* find_ext_ent(data_code *data_arr, codes* code_arr, label* labels, int label_count,int *flag,  int IC, int *ext_count, int *ent_count, char * file_name);

/*
 * update_entry - Marks an existing label as an entry label.
 * @param labels: Array of labels.
 * @param label_count: Number of labels.
 * @param name: Label name.
 * @param row_count: Line number in the file.
 * @param file_name: Name of the source file.
 * @param flag: Pointer to an error flag.
 */
void update_entry(label* labels, int label_count, char* name, int row_count, char* file_name, int* flag);

/*
 * create_output_files - Creates output files for object code, externals, and entries.
 * @param code_arr: Array containing the machine code instructions.
 * @param data_arr: Array containing the data section.
 * @param IC: Instruction Counter.
 * @param DC: Data Counter.
 * @param file_name: Name of the source file.
 * @param ext_ent_lbl_arr: Array of external and entry labels.
 * @param ext_count: Number of external labels.
 * @param ent_count: Number of entry labels.
 */
void create_output_files(codes* code_arr, data_code* data_arr, int IC, int DC, char* file_name, ext_ent_lbl* ext_ent_lbl_arr, int ext_count, int ent_count);

/*
 * write_ob - Writes object code to a file.
 * @param file: Pointer to the output object file.
 * @param code_arr: Array containing the machine code instructions.
 * @param data_arr: Array containing the data section.
 * @param IC: Instruction Counter.
 * @param DC: Data Counter.
 * @param file_name: Name of the source file.
 * @return Integer indicating success or failure.
 */
void write_ob(FILE* file,codes* code_arr, data_code* data_arr,int IC,int DC,char *file_name);

/*
 * write_ext_ent - Writes external or entry labels to a file.
 * @param file: Pointer to the output file.
 * @param ext_ent_arr: Array of external and entry labels.
 * @param ext_ent_count: Number of external and entry labels.
 * @param type: Type of labels to write (external or entry).
 */
void write_ext_ent(FILE* file, ext_ent_lbl* ext_ent_arr, int ext_ent_count, ext_ent type);

#endif
