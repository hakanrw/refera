/*
 * This file is part of Refera, the interpreted scripting language.
 *
 * Refera is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Refera is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Refera.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2024  Ahmet Hakan Candar
 * Copyright (C) 2024  Berk Tahir Kılıç
 * Copyright (C) 2024  Barış Getiren
 * Copyright (C) 2024  Egemen Aybir
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct {
	void** data;         // Array of pointers to store values
	size_t size;         // Current number of elements in the stack
	size_t capacity;     // Total allocated capacity of the stack
}
refera_stack_t;

typedef struct
{
	void* global_variables;        // Array of global variables
	void* function_registry;       // Array of functions
	refera_stack_t stack;          // Interpreter stack
	char* error_message;     // Error message if exists
}
refera_state_t;

void refera_state_init(refera_state_t* state);

void refera_state_destroy(refera_state_t* state);

bool refera_eval_string(refera_state_t* state, const char* text);

bool refera_eval_file(refera_state_t* state, const char* filename);
