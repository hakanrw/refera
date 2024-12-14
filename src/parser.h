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

struct Ary {
	char symbol[10];
	int idx1; // index 1
	int idx2; // index 2
};

struct ParseTable {
	int operation; // operation code
	char destination[10]; // destination array
	char source1[10]; // source array 1
	char source2[10]; // source array 2
	int ifexists; // 1-exists, 0- does not exist
	int cond; // condition code
	struct Ary operand1;
	struct Ary operand2;
};

extern struct ParseTable PT[1];

void parser_parse_statement(const char* statement);
void tokenize(const char *statement, char tokens[50][10], int *token_count);
int check_operations(const char* token);
int check_cond(const char* token);
