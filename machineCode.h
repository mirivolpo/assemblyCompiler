
#ifndef MACHINECODE_H  
#define MACHINECODE_H 

#include "label.h"


#define OPCODE_BITS_SHIFT 18
#define SOURCE_ADDRESS_BITS_SHIFT 16
#define SOURCE_REGISTER_BITS_SHIFT 13
#define DEST_ADDRESS_BITS_SHIFT 11
#define DEST_REGISTER_BITS_SHIFT 8
#define FUNCT_BITS_SHIFT 3
#define LABEL_VALUE_SHIFT 3
#define INSTANT_VALUE_SHIFT 3
#define A_ON 4
#define R_ON 2 
#define E_ON 1




typedef struct {
    char* name;
    int (*func)(char*, int, int*, char*, address_method*, address_method*, char*, char*);
    int opcode;
    int funct;
}cmd;


/**
 * This function encodes an instruction into machine code.
 * 
 * @param name instruction name.
 * @param source_address Addressing method of the source operand.
 * @param destination_address Addressing method of the destination operand.
 * @param sourse_operand Source operand value as a string.
 * @param destination_operand Destination operand value as a string.
 * @param code_arr Array to store the generated machine code.
 * @param IC Instruction counter.
 * @param commands Array of command definitions.
 * @param row Current row in the source file.
 */
void instruction_codeing(char* name, address_method source_address, address_method destination_address, char* sourse_operand, char* destination_operand, codes* code_arr, int IC, cmd* commands, int row);

/**
 * This function encodes an immediate operand into machine code.
 * 
 * @param operand_name The operand value as a string.
 * @param code_arr Array to store the generated machine code.
 * @param IC Instruction counter.
 * @param row Current row in the source file.
 */
void instant_codeing(char* operand_name, codes* code_arr, int IC, int row);

/**
 * This function encodes a label reference into machine code.
 * 
 * @param code_arr Array to store the generated machine code.
 * @param index Index in the code array.
 * @param label_name Name of the label.
 * @param labels Array of defined labels.
 * @param label_count Number of labels.
 * @param address Address of the instruction.
 * @param flag Pointer to an error flag.
 * @param row_count Current row in the source file.
 * @param file_name Name of the source file.
 */

void label_codeing(codes* code_arr, int index, char* label_name, label* labels, int label_count, int address, int* flag, int  row_count, char* file_name);

#endif

