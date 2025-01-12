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

#ifndef REFERA_H
#define REFERA_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Refera structures */

typedef struct SymbolTable
{
	char symbol[10]; // array name
	int dim; // array dimension 1 or 2 or 0 (for single variables)
	int size1; // array size in dimension 1
	int size2; // array size in dimension 2
	int *base; // base address for the array
}
refera_symbol_t;

typedef struct
{
	refera_symbol_t variables[20];            // Array of global variables
	size_t variables_idx;
	char* error_message;        // Error message if exists
	bool silent;
}
refera_state_t;

/* State initalization */

void refera_state_init(refera_state_t* state);

void refera_state_destroy(refera_state_t* state);

/* Expressions */

void refera_set_variable(refera_state_t* state, const char* name,
                         refera_symbol_t value);

refera_symbol_t* refera_get_variable(refera_state_t* state,
                                     const char* name);

refera_symbol_t refera_copy_variable(refera_symbol_t* variable);

refera_symbol_t refera_create_variable(int dim, int size1, int size2);

/* Interpretarion */

bool refera_check_if(refera_state_t* state);

bool refera_eval_string(refera_state_t* state, const char* text);

bool refera_eval_file(refera_state_t* state, const char* filename);

/* Helper Functions */

bool refera_is_number(const char* str);

#endif /* REFERA_H */
