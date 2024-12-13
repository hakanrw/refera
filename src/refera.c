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

#include "refera.h"
#include "parser.h"

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

void refera_state_init(refera_state_t* state)
{
	memset(state, 0, sizeof(refera_state_t));
}

void refera_state_destroy(refera_state_t* state)
{
	set_error_message(state, NULL);
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
		*existing_variable = value; // Overwrite
	}
}

bool refera_eval_string(refera_state_t* state, const char* text)
{
	if (strlen(text) == 0) return true;

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
