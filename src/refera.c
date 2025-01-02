/*
 * This file is part of Refera, the interpreted scripting language.
 *
 * Refera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Refera is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Refera.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024  Ahmet Hakan Candar
 * Copyright (C) 2024  Berk Tahir Kılıç
 * Copyright (C) 2024  Barış Getiren
 * Copyright (C) 2024  Egemen Aybir
 */

#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "refera.h"
#include "parser.h"
#include "functions.h"
#include "operations.h"

static void set_error_message(refera_state_t* state, char* error_message)
{
	if (state->error_message != NULL)
		free(state->error_message);

	if (error_message == NULL)
	{
		state->error_message = NULL;
		return;
	}

	size_t error_len = strlen(error_message) + 1;
	state->error_message = malloc(error_len * sizeof(char));

	strcpy(state->error_message, error_message); // Copy
}

static size_t get_variable_base_len(refera_symbol_t* symbol)
{
	if (symbol->dim == 0)
		return 1;
	else if (symbol->dim == 1)
		return symbol->size1;
	else if (symbol->dim == 2)
		return symbol->size1 * symbol->size2;
	else
		assert(false); // this should never execute
}

void refera_state_init(refera_state_t* state)
{
	memset(state, 0, sizeof(refera_state_t));
}

void refera_state_destroy(refera_state_t* state)
{
	set_error_message(state, NULL);

	for (size_t i = 0; i < state->variables_idx; i++)
	{
		free(state->variables[i].base); // Free
		state->variables[i].base = NULL; // No dangling pointers
	}
}

refera_symbol_t* refera_get_variable(refera_state_t* state,
                                     const char* name)
{
	for (size_t i = 0; i < state->variables_idx; i++)
	{
		if (strcmp(name, state->variables[i].symbol) == 0)
			return &state->variables[i];
	}

	return NULL; // Not found
}

void refera_set_variable(refera_state_t* state, const char* name,
                         refera_symbol_t value)
{
	strncpy(value.symbol, name, 10); // Set symbol name
	value.symbol[10 - 1] = 0; // Ensure termination

	refera_symbol_t* existing_variable =
                         refera_get_variable(state, name);

	if (existing_variable == NULL)
	{
		state->variables[state->variables_idx] = value;
		state->variables_idx++;
	}
	else
	{
		free(existing_variable->base); // Free
		*existing_variable = value; // Overwrite
	}
}

refera_symbol_t refera_copy_variable(refera_symbol_t* value)
{
	refera_symbol_t copied_var = refera_create_variable(value->dim,
                                                            value->size1,
                                                            value->size2);

	size_t base_len = get_variable_base_len(&copied_var);
	memcpy(copied_var.base, value->base, base_len * sizeof(int)); // Copy integers

	return copied_var;
}

refera_symbol_t refera_create_variable(int dim, int size1, int size2)
{
	refera_symbol_t variable;
	memset(&variable.symbol, 0, 10); // Clear name
	variable.dim = dim;
	variable.size1 = size1;
	variable.size2 = size2;

	size_t base_len = get_variable_base_len(&variable);
	variable.base = malloc(base_len * sizeof(int));
	memset(variable.base, 0, base_len * sizeof(int)); // Set all to 0

	return variable;
}

bool refera_check_if(refera_state_t* state)
{
	if(PT[0].ifexists == 0) return true;
	int idx1_1, idx1_2, idx2_1, idx2_2;
	idx1_1 = idx1_2 = idx2_1 = idx2_2 = -1;
	refera_symbol_t* opr1 = refera_get_variable(state, PT[0].operand1.symbol);
	refera_symbol_t* opr2 = refera_get_variable(state, PT[0].operand2.symbol);
	int opr1_value = -1;
	int opr2_value = -1;
	if(opr1->dim != 0)
	{
		if(opr1->dim == 1)
		{
			idx1_1 = PT[0].operand1.idx1;
			opr1_value = *(opr1->base + idx1_1);
		}
		else
		{
			idx1_1 = PT[0].operand1.idx1;
			idx1_2 = PT[0].operand1.idx2;
			int array_index = (idx1_1 * opr1->size2) + idx1_2;
			opr1_value = *(opr1->base + array_index);
		}
	}
	else opr1_value = *(opr1->base);
	if(opr2 != NULL)
	{
		if(opr2->dim != 0)
		{
			if(opr2->dim == 1)
			{
				idx2_1 = PT[0].operand2.idx1;
				opr2_value = *(opr2->base + idx2_1);
			}
			else
			{
				idx2_1 = PT[0].operand2.idx1;
				idx2_2 = PT[0].operand2.idx2;
				int array_index = (idx2_1 * opr2->size2) + idx2_2;
				opr2_value = *(opr2->base + array_index);
			}
		}
		else opr2_value = *(opr2->base);
	}
	else opr2_value = atoi(PT[0].operand2.symbol);
	int condition = PT[0].cond;
	switch (condition)
	{
		case R_EQU:
			if(opr1_value == opr2_value) return true;
			break;
		case R_NEQ:
			if(opr1_value != opr2_value) return true;
			break;
		case R_GRT:
			if(opr1_value > opr2_value) return true;
			break;
		case R_GTE:
			if(opr1_value >= opr2_value) return true;
			break;
		case R_LSR:
			if(opr1_value < opr2_value) return true;
			break;
		case R_LRE:
			if(opr1_value <= opr2_value) return true;
			break;
	}
	return false;
}

bool refera_eval_string(refera_state_t* state, const char* text)
{
	if (strlen(text) == 0) return true;

	parser_parse_statement(text);
	if (!state->silent) parser_display_table();

	int operation = PT[0].operation;
	refera_symbol_t* opr = NULL;
	refera_symbol_t* ary1 = NULL;
	refera_symbol_t* ary2 = NULL;
	refera_symbol_t* ary3 = NULL;
	int src2_value = 0;

	switch (operation)
	{
		case R_ADD:
			if(!refera_check_if(state)) return true;
			ary1 = refera_get_variable(state, PT[0].destination);
			ary2 = refera_get_variable(state, PT[0].source1);
			ary3 = refera_get_variable(state, PT[0].source2);
			if(ary1 == NULL)
			{
				refera_set_variable(state, PT[0].destination,refera_create_variable(ary2->dim,ary2->size1,ary2->size2));
				ary1 = refera_get_variable(state, PT[0].destination);
			}
			if(ary2 != NULL && ary2->dim != 0 && (ary2->size1 != ary1->size1 || ary2->size2 != ary1->size2)) return false;
			if(ary3 != NULL && ary3->dim != 0 && (ary3->size1 != ary1->size1 || ary3->size2 != ary1->size2)) return false;
			if(ary3 == NULL) 
			{
				src2_value = atoi(PT[0].source2);
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) + src2_value;
				else if(ary1->dim == 1)
					for (int i = 0; i < ary1->size1; i++)
					{
						*(ary1->base + i) = *(ary2->base) + src2_value;
					}
				else
					for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
					{
						*(ary1->base + i) = *(ary2->base) + src2_value;
					}
			}
			else 
			{
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) + *(ary3->base);
				else if(ary1->dim == 1)
				{
					if(ary3->dim == 0)
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) + *(ary3->base);
						}
					else
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) + *(ary3->base + i);
						}
				}
				else
				{
					if(ary3->dim == 0)
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) + *(ary3->base);
						}
					else
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) + *(ary3->base + i);
						}
				}
			}
			return true;
			break;
		case R_SUB:
			if(!refera_check_if(state)) return true;
			ary1 = refera_get_variable(state, PT[0].destination);
			ary2 = refera_get_variable(state, PT[0].source1);
			ary3 = refera_get_variable(state, PT[0].source2);
			if(ary1 == NULL)
			{
				refera_set_variable(state, PT[0].destination,refera_create_variable(ary2->dim,ary2->size1,ary2->size2));
				ary1 = refera_get_variable(state, PT[0].destination);
			}
			if(ary2 != NULL && ary2->dim != 0 && (ary2->size1 != ary1->size1 || ary2->size2 != ary1->size2)) return false;
			if(ary3 != NULL && ary3->dim != 0 && (ary3->size1 != ary1->size1 || ary3->size2 != ary1->size2)) return false;
			if(ary3 == NULL) 
			{
				src2_value = atoi(PT[0].source2);
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) - src2_value;
				else if(ary1->dim == 1)
					for (int i = 0; i < ary1->size1; i++)
					{
						*(ary1->base + i) = *(ary2->base) - src2_value;
					}
				else
					for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
					{
						*(ary1->base + i) = *(ary2->base) - src2_value;
					}
			}
			else 
			{
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) - *(ary3->base);
				else if(ary1->dim == 1)
				{
					if(ary3->dim == 0)
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) - *(ary3->base);
						}
					else
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) - *(ary3->base + i);
						}
				}
				else
				{
					if(ary3->dim == 0)
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) - *(ary3->base);
						}
					else
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) - *(ary3->base + i);
						}
				}
			}
			return true;
			break;
		case R_MUL:
			if(!refera_check_if(state)) return true;
			ary1 = refera_get_variable(state, PT[0].destination);
			ary2 = refera_get_variable(state, PT[0].source1);
			ary3 = refera_get_variable(state, PT[0].source2);
			if(ary1 == NULL)
			{
				refera_set_variable(state, PT[0].destination,refera_create_variable(ary2->dim,ary2->size1,ary2->size2));
				ary1 = refera_get_variable(state, PT[0].destination);
			}
			if(ary2 != NULL && ary2->dim != 0 && (ary2->size1 != ary1->size1 || ary2->size2 != ary1->size2)) return false;
			if(ary3 != NULL && ary3->dim != 0 && (ary3->size1 != ary1->size1 || ary3->size2 != ary1->size2)) return false;
			if(ary3 == NULL) 
			{
				src2_value = atoi(PT[0].source2);
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) * src2_value;
				else if(ary1->dim == 1)
					for (int i = 0; i < ary1->size1; i++)
					{
						*(ary1->base + i) = *(ary2->base) * src2_value;
					}
				else
					for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
					{
						*(ary1->base + i) = *(ary2->base) * src2_value;
					}
			}
			else 
			{
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) * *(ary3->base);
				else if(ary1->dim == 1)
				{
					if(ary3->dim == 0)
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) * *(ary3->base);
						}
					else
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) * *(ary3->base + i);
						}
				}
				else
				{
					if(ary3->dim == 0)
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) * *(ary3->base);
						}
					else
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) * *(ary3->base + i);
						}
				}
			}
			return true;
			break;
		case R_DIV:
			if(!refera_check_if(state)) return true;
			ary1 = refera_get_variable(state, PT[0].destination);
			ary2 = refera_get_variable(state, PT[0].source1);
			ary3 = refera_get_variable(state, PT[0].source2);
			if(ary1 == NULL)
			{
				refera_set_variable(state, PT[0].destination,refera_create_variable(ary2->dim,ary2->size1,ary2->size2));
				ary1 = refera_get_variable(state, PT[0].destination);
			}
			if(ary2 != NULL && ary2->dim != 0 && (ary2->size1 != ary1->size1 || ary2->size2 != ary1->size2)) return false;
			if(ary3 != NULL && ary3->dim != 0 && (ary3->size1 != ary1->size1 || ary3->size2 != ary1->size2)) return false;
			if(ary3 == NULL) 
			{
				src2_value = atoi(PT[0].source2);
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) / src2_value;
				else if(ary1->dim == 1)
					for (int i = 0; i < ary1->size1; i++)
					{
						*(ary1->base + i) = *(ary2->base) / src2_value;
					}
				else
					for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
					{
						*(ary1->base + i) = *(ary2->base) / src2_value;
					}
			}
			else 
			{
				if(ary1->dim == 0) *(ary1->base) = *(ary2->base) / *(ary3->base);
				else if(ary1->dim == 1)
				{
					if(ary3->dim == 0)
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) / *(ary3->base);
						}
					else
						for (int i = 0; i < ary1->size1; i++)
						{
							*(ary1->base + i) = *(ary2->base + i) / *(ary3->base + i);
						}
				}
				else
				{
					if(ary3->dim == 0)
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) / *(ary3->base);
						}
					else
						for (int i = 0; i < (ary1->size1 * ary1->size2); i++)
						{
							*(ary1->base + i) = *(ary2->base + i) / *(ary3->base + i);
						}
				}
			}
			return true;
			break;
		case R_INI:
			if(!refera_check_if(state)) return true;
			int dim;
			if(PT[0].source1[0] == '\0') dim = 0;
			else if(PT[0].source2[0] == '\0') dim = 1;
			else dim = 2;
			refera_symbol_t new_var = refera_create_variable(dim, atoi(PT[0].source1), atoi(PT[0].source2));
			if(dim == 1)
			{
				for (int i = 0; i < new_var.size1; i++)
				{
					*(new_var.base + i) = values[i];
				}
			}
			else if(dim == 2)
			{
				for (int i = 0; i < new_var.size1 * new_var.size2; i++)
				{
					*(new_var.base + i) = values[i + 1];
				}
			}	
			refera_set_variable(state, PT[0].destination, new_var);
			return true;
			break;
		case R_SET:
			if(!refera_check_if(state)) return true;
			refera_symbol_t* variable = refera_get_variable(state, PT[0].destination);
			if (variable == NULL)
			{
				refera_symbol_t new_variable = refera_create_variable(0, 0, 0);
				refera_set_variable(state, PT[0].destination, new_variable);
				variable = refera_get_variable(state, PT[0].destination);
			}
			if (variable && variable->base)
			{
				if(variable->dim == 0)  *variable->base = atoi(PT[0].source1);
				else if(variable->dim == 1)
					for (int i = 0; i < variable->size1; i++)
					{
						*(variable->base + i) = atoi(PT[0].source1);
					}
				else if(variable->dim == 2)
					for (int i = 0; i < (variable->size1 * variable->size2); i++)
					{
						*(variable->base + i) = atoi(PT[0].source1);
					}
					
					
			}
			return true;
			break;
		case R_CPY:
			if(!refera_check_if(state)) return true;
			refera_symbol_t* dest = refera_get_variable(state, PT[0].destination);
			refera_symbol_t* source = refera_get_variable(state, PT[0].source1);
			if(dest == NULL)
			{
				refera_set_variable(state, PT[0].destination,refera_create_variable(source->dim,source->size1,source->size2));
				dest = refera_get_variable(state, PT[0].destination);
			}
			if((dest->size1 != source->size1) || (dest->size2 != source->size2))
			{
				set_error_message(state, "Array sizes do not match!");
				return false;
			}
			if(dest->dim == 0) *dest->base = *source->base;
			else if(dest-> dim == 1)
				for (int i = 0; i < dest->size1; i++)
				{
					*(dest->base + i) = *(source->base + i);
				}
			else if(dest->dim == 2)
				for (int i = 0; i < (dest->size1 * dest->size2); i++)
				{
					*(dest->base + i) = *(source->base + i);
				}
			return true;
			break;
		case R_SUM:
			if(!refera_check_if(state)) return true;
			opr = refera_get_variable(state, PT[0].source1);
			if(opr == NULL)
			{
				set_error_message(state, "Can't find operand or destination");
				return false;
			}
			refera_set_variable(state, PT[0].destination, refera_sum(*opr));
			return true;
			break;
		case R_AVG:
			if(!refera_check_if(state)) return true;
			opr = refera_get_variable(state, PT[0].source1);
			if(opr == NULL)
			{
				set_error_message(state, "Can't find operand or destination");
				return false;
			}
			refera_set_variable(state, PT[0].destination, refera_aver(*opr));
			return true;
			break;
		case R_DIA:
			if(!refera_check_if(state)) return true;
			opr = refera_get_variable(state, PT[0].source1);
			if(opr == NULL)
			{
				set_error_message(state, "Can't find operand or destination");
				return false;
			}
			refera_set_variable(state, PT[0].destination, refera_diag(*opr));
			return true;
			break;
		case R_EXC:
			if(!refera_check_if(state)) return true;
			ary1 = refera_get_variable(state, PT[0].operand1.symbol);
			ary2 = refera_get_variable(state, PT[0].operand2.symbol);
			int temp_element = *(ary1->base + ((PT[0].operand1.idx2 * ary1->size2) + PT[0].operand1.idx1));
			*(ary1->base + ((PT[0].operand1.idx2 * ary1->size2) + PT[0].operand1.idx1)) = 
			*(ary2->base + ((PT[0].operand2.idx2 * ary1->size2) + PT[0].operand2.idx1));
			*(ary2->base + ((PT[0].operand2.idx2 * ary1->size2) + PT[0].operand2.idx1)) = temp_element;
			return true;
			break;
		case R_PRI:
			if(!refera_check_if(state)) return true;
			if(strcmp(PT[0].source1, "vars") == 0)
			{
				for (size_t i = 0; i < state->variables_idx; i++)
				{
					printf("%s: ", state->variables[i].symbol);
					refera_print(state->variables[i]);
				}
				return true;
			}
			refera_symbol_t* var = refera_get_variable(state, PT[0].source1);
			if(var == NULL)
			{
				set_error_message(state, "Can't find operand");
				return false;
			}
			refera_print(*var);
			return true;
			break;
	}

	set_error_message(state, "Unimplemented statement");

	return false;
}

bool refera_eval_file(refera_state_t* state, const char* filename)
{
	FILE *file = fopen(filename, "r");

	if (file == NULL)
	{
		char error_message[256];
		snprintf(error_message, 256, "Error opening file %s", filename);
		set_error_message(state, error_message);
		return false;
	}

	char line[256];
	int current_line = 1;

	while (fgets(line, sizeof(line), file) != NULL)
	{
		line[255] = 0; // Ensure terminating string
		int line_len = strlen(line);
		if (line_len > 0 && line[line_len - 1] == '\n')
			line[line_len - 1] = 0; // Remove newline

		bool result = refera_eval_string(state, line);
		if (!result)
		{
			char error_message[256];
	                snprintf(error_message, 256, "(%s:%d) %s", filename, current_line, state->error_message);
			set_error_message(state, error_message);
			return false;
		}
		current_line++;
	}

	state->error_message = NULL;
	return true;
}

bool refera_is_number(const char* str)
{
	size_t string_len = strlen(str);
	for (size_t i = 0; i < string_len; i++)
	{
		bool digit = isdigit(str[i]);
		if (!digit) return false;
	}

	return true;
}
