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

#include <stdio.h>
#include <string.h>

#include "refera.h"

void repl_loop(refera_state_t* state)
{
	while (true)
	{
		printf("> ");

		char line[256];
		if (fgets(line, sizeof(line), stdin) != NULL)
		{
			line[255] = 0; // Ensure terminating string
			int line_len = strlen(line);
			if (line_len > 0 && line[line_len - 1] == '\n')
				line[line_len - 1] = 0; // Remove newline

			if (strcmp(line, ".end") == 0)
				break; // End prorgam

			bool result = refera_eval_string(state, line);
			if (!result) printf("error: %s\n", state->error_message);
		}
		else
		{
			break;
		}
	}
}

void eval_file(refera_state_t* state, const char* filename)
{
	bool result = refera_eval_file(state, filename);
        if (!result) printf("error: %s\n", state->error_message);
}

void print_usage()
{
    printf("USAGE: refera [--help] [--silent] [file_name]\n");
    printf("  --help          Display this help message\n");
    printf("  --silent        Silent output\n");
    printf("  file_name       Evaluate the file\n");
}

int main(int argc, char** argv)
{
	refera_state_t state;
	refera_state_init(&state);

	if (argc == 1)
	{
		repl_loop(&state);
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "--help") == 0) print_usage();
		else if (strcmp(argv[1], "--silent") == 0)
		{
			state.silent = true;
			repl_loop(&state);
		}
		else eval_file(&state, argv[1]);
	}
	else if (argc == 3)
	{
		if (strcmp(argv[1], "--silent") == 0)
		{
			state.silent = true;
			eval_file(&state, argv[2]);
		}
		else if (strcmp(argv[2], "--silent") == 0)
		{
			state.silent = true;
			eval_file(&state, argv[1]);
		}
		else print_usage();
	}
	else // Invalid number of arguments
	{
		print_usage();
	}

	refera_state_destroy(&state);
	return 0;
}
