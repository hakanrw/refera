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

#include "test_functions.h"
#include "../src/functions.h"

void test_functions_sum()
{
	CU_ASSERT(false);
}

void test_functions_diag()
{
	CU_ASSERT(false);
}

void test_functions_aver()
{
	CU_ASSERT(false);
}

void test_functions_print()
{
	CU_ASSERT(false);
}

int test_functions_register(CU_pSuite* suite)
{
	*suite = CU_add_suite("Functions", 0, 0);
	if (*suite == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "sum()", test_functions_sum) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "diag()", test_functions_diag) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "aver()", test_functions_aver) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "print()", test_functions_print) == NULL)
		return CU_get_error();

	return 0;
}
