#ifndef FIRSTPASS_H  
#define FIRSTPASS_H  

#include "mcr.h"
#include "secondPass.h"



/**
 * This function performs the first pass of the assembly process.(Encodes all instruction and data components, with the exception of label operands.)
 * @param am Input assembly file stream (after macro expansion).
 * @param file_name Name of the assembly file.
 * @param macro List of defined macros.
 * @return void
 * 
 */
void first_pass(FILE* am, char* file_name, macro_list* macro);

/**
 * This function saves a label operand into the code array for later encodeing.
 * @param operand_name The name of the label operand to save .
 * @param code_arr Pointer to the array of code structures where the operand will be stored.
 * @param row The line number in the source file where this operand appears, used for error tracking.
 * 
 */
void save_label_operand(char* operand_name, codes* code_arr, int row);

/**
 * This function Encodes .data or .string directives into the data array in binary format.
 * 
 * @param line The input line containing the directive and its arguments.
 * @param first_word The directive type .
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param data_arr Pointer to the data array where the encoded values are stored.
 * @param file_name The name of the source file, used for error messages.
 * 
 * @return int Returns TRUE if encoding succeeds, FALSE if memory allocation fails or validation fails.
 */
int data_codeing(char* line, char* first_word, int row_count, int* flag, data_code* data_arr, char* file_name);

/**
 * This function updates the addresses of data labels and data array entries after the first pass.
 * @param data_arr Pointer to the data array containing encoded data directives.
 * @param labels Pointer to the array of labels, some of which are associated with data.
 * @param label_count The number of labels in the labels array.
 */
void update_data_address(data_code* data_arr, label* labels, int label_count);

/**
 * This function validates the syntax of a .string or .data directive line. 
 * @param line The input line containing the directive and its arguments.
 * @param first_word The directive type (e.g., ".string" or ".data").
 * @param row_count The current line number in the source file, used for error reporting.
 * @param file_name The name of the source file, used for error messages.
 * 
 * @return int Returns TRUE if the line is valid, FALSE if it contains syntax errors.
 */
int valid_data_line(char* line, char* first_word, int row_count, char* file_name);

/**
 * This function checks the syntax and validity of source and destination operands for the mov, 
 * add, or sub instructions and updates the addressing methods and operand strings if valid.
 *
 * @param line The input line containing the operands after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (updated on success).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (updated on success).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if operands are valid, FALSE if validation fails, or MEMORY_ALLOCATION_FAIL if memory allocation fails.
 */
int valid_mov_add_sub(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 * This function checks the syntax and validity of source and destination operands for the cmp 
 *  instruction and updates the addressing methods and operand strings if valid.
 * @param line The input line containing the operands after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (updated on success).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (updated on success).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if operands are valid, FALSE if validation fails, or MEMORY_ALLOCATION_FAIL if memory allocation fails.
 * 
 */
int valid_cmp(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 * This function checks the syntax and validity of source and destination operands for the lea 
 * instruction and updates the addressing methods and operand strings if valid.
 * 
 * @param line The input line containing the operands after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (updated on success).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (updated on success).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if operands are valid, FALSE if validation fails.
 * 
 */
int valid_lea(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 * This function checks the syntax and validity of a single operand for the clr, not, inc, dec, or 
 * red instructions and updates the addresing method and operand string if valid.
 * 
 * @param line The input line containing the operand after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (unused, remains unchanged).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (unused, remains unchanged).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if the operand is valid, FALSE if validation fails.
 * 
 */
int valid_clr_not_inc_dec_red(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 * This function checks the syntax and validity of a single operand for the jmp, bne, or jsr 
 * instructions and updates the addresing method and operand string if valid.
 *
 * @param line The input line containing the operand after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (unused, remains unchanged).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (unused, remains unchanged).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if the operand is valid, FALSE if validation fails.
 * 
 */
int valid_jmp_bne_jsr(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 * This function checks the syntax and validity of a single operand for the prn instruction and updates the addresing method and operand string if valid.
 * 
 * @param line The input line containing the operand after the command.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (unused, remains unchanged).
 * @param destination_address Pointer to the addressing method of the destination operand (updated on success).
 * @param source_operand Buffer to store the source operand string (unused, remains unchanged).
 * @param destination_operand Buffer to store the destination operand string (updated on success).
 * 
 * @return int Returns TRUE if the operand is valid, FALSE if validation fails.
 */
int valid_prn(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);

/**
 
 * This function checks that no operands or extra characters follow the rts or stop instructions, 
 * as these commands do not accept any operands. 
 *
 * @param line The input line after the command, expected to be empty.
 * @param row_count The current line number in the source file, used for error reporting.
 * @param flag Pointer to an error flag; set to TRUE if validation fails.
 * @param file_name The name of the source file, used for error messages.
 * @param source_address Pointer to the addressing method of the source operand (unused, remains unchanged).
 * @param destination_address Pointer to the addressing method of the destination operand (unused, remains unchanged).
 * @param source_operand Buffer to store the source operand string (unused, remains unchanged).
 * @param destination_operand Buffer to store the destination operand string (unused, remains unchanged).
 * 
 * @return int Returns TRUE if the line is empty (valid), FALSE if extra characters are present (invalid).
 */
int valid_rts_stop(char* line, int row_count, int* flag, char* file_name, address_method* source_address, address_method* destination_address, char* source_operand, char* destination_operand);


#endif
