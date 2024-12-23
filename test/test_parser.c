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

#include "test_parser.h"
#include "../src/parser.h"

void test_parser_parse_statement()
{
	CU_ASSERT(false);
}

int test_parser_register(CU_pSuite* suite)
{
	*suite = CU_add_suite("Parser", 0, 0);
	if (*suite == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "parse_statement()", test_parser_parse_statement) == NULL)
		return CU_get_error();

	return 0;
}
