
#include "machineCode.h"


void instruction_codeing(char* name, address_method source_address, address_method destination_address, char* sourse_operand, char* destination_operand, codes* code_arr, int IC,cmd* commands, int row)
{
	int i;
	int sourse_operand_value;
	int destination_operand_value;
	char machine_code_string[CODE_WORD_SIZE];
	int ARE, funct, dest_register = 0, dest_address, src_register = 0, src_address, opcode, machine_code;

	/* Search for the command in the commands array */
	for (i = 0; i < NUM_COMMANDS; i++)
	{
		if (strcmp(name, commands[i].name) == ZERO)
		{
			/* Extract and shift opcode to its position */
			opcode = commands[i].opcode << OPCODE_BITS_SHIFT;

			/* Set source addressing bits; 0 if no source operand */
			if (source_address == non)
			{
				src_address = 0;
			}
			else
			{
				src_address = source_address << SOURCE_ADDRESS_BITS_SHIFT;
			}

			/* Set destination addressing bits; 0 if no destination operand */
			if (destination_address == non)
			{
				dest_address = 0;
			}
			else
			{
				dest_address = destination_address << DEST_ADDRESS_BITS_SHIFT;
			}

			/* Extract and shift function code to its position */
			funct = commands[i].funct << FUNCT_BITS_SHIFT;

			/* Set A bit as ON */
			ARE = A_ON;

			/* Handle source register operand */
			if (source_address == direct_register)
			{
				sourse_operand_value = atoi(sourse_operand);
				src_register = sourse_operand_value << SOURCE_REGISTER_BITS_SHIFT;
			}

			/* Handle destination register operand */
			if (destination_address == direct_register)
			{
				destination_operand_value = atoi(destination_operand);
				dest_register = destination_operand_value << DEST_REGISTER_BITS_SHIFT;
			}

			/* Combine all bit fields into a single machine code word */
			machine_code = opcode | dest_register | dest_address | funct | ARE | src_register | src_address;
			/* Convert the machine code to binary string */
			int_to_binary(machine_code, machine_code_string, CODE_WORD_SIZE-1);

			/* Store the instruction in the code array at the current IC */
			code_arr[(IC) - START_ADDRESS].address = IC;
			strcpy(code_arr[(IC) - START_ADDRESS].code, machine_code_string);
			code_arr[(IC)-START_ADDRESS].finish = yes;/* Mark as fully encoded */
			code_arr[(IC)-START_ADDRESS].row = row;
			
			break;/* Exit loop once command is processed */
		}
	}
}

void instant_codeing(char* operand_name, codes* code_arr, int  IC, int row)
{
	char machine_code_string[MAX_CHAR_LBL+TWO];
	int value, ARE, machine_code;

	/* Convert the immediate operand to an integer */
	value = atoi(operand_name);

	/* Set Absolute addressing mode */
	ARE = A_ON;

	/* Shift the immediate value to its bit position */
	value = value << INSTANT_VALUE_SHIFT;

	/* Combine ARE and value into machine code */
	machine_code = ARE | value;

	/* Convert the machine code to binary string */
	int_to_binary(machine_code, machine_code_string, CODE_WORD_SIZE-1);
	code_arr[(IC) - START_ADDRESS].address = IC;
	strcpy(code_arr[(IC) - START_ADDRESS].code, machine_code_string);
	code_arr[(IC)-START_ADDRESS].finish = yes;/* Mark as fully encoded */
	code_arr[(IC)-START_ADDRESS].row = row;
}



void label_codeing(codes* code_arr, int index, char* label_name, label* labels, int label_count, int address,int * flag,int  row_count, char * file_name)
{
	int i, ARE, value=0, machine_code;
	char machine_code_string[MAX_CHAR_LBL+TWO];

	/* Check if the label uses relative addressing */
	if (*label_name == '&')
	{
		ARE = A_ON;/* Set A as on*/
		label_name++;/*Skip the '&' character */

		/* Search for the label in the labels array */
		for (i = 1; i <= label_count; i++)
		{
			if (strcmp(label_name, labels[i].name) == ZERO)
			{
				value = labels[i].address - address;
				break;
			}
		}

		/* Check if the label was not found */
		if (i > label_count)
		{
			printf("ERROR in file %s, in line %d : The label does not exist\n", file_name, row_count);
			*flag = TRUE;
			return;
		}
	}
	else
	{

		/* Search for the label in the labels array */
		for (i = 1; i <= label_count; i++)
		{
			if (strcmp(label_name, labels[i].name) == ZERO)
			{
				value = labels[i].address;
				if (labels[i].type == external)/* Use absolute address */
				{
					ARE = E_ON;/*Set E on*/
				}
				else
				{
					ARE = R_ON;/*Set R on*/
				}
				break;
			}
		}

		/* Check if the label was not found */
		if (i  > label_count)
		{
			printf("ERROR in file %s, in line %d : The label does not exist\n", file_name, row_count);
			*flag = TRUE;
			return;
		}
	}
	value = value << LABEL_VALUE_SHIFT;

	/* Combine ARE and value into machine code */
	machine_code = ARE | value;

	/* Convert the machine code to binary string */
	int_to_binary(machine_code, machine_code_string, CODE_WORD_SIZE-1);
	strcpy(code_arr[index].code, machine_code_string);
}
