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

#include "test_refera.h"
#include "refera.h"

void test_refera_set_variable()
{
	CU_ASSERT(false);
}

void test_refera_get_variable()
{
	CU_ASSERT(false);
}

void test_refera_eval_string()
{
	CU_ASSERT(false);
}

void test_refera_eval_file()
{
	CU_ASSERT(false);
}

int test_refera_register(CU_pSuite* suite)
{
	*suite = CU_add_suite("Interpreter", 0, 0);
	if (*suite == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "refera_set_variable()", test_refera_set_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "refera_get_variable()", test_refera_get_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "refera_eval_file()", test_refera_eval_file) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "refera_eval_string()", test_refera_eval_string) == NULL)
		return CU_get_error();

	return 0;
}
