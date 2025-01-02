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
	const int arr_values[] = { 5, 4, 3, 2, 1 };
        refera_symbol_t arr = refera_create_variable(1, 5, 0);
	memcpy(arr.base, arr_values, sizeof(arr_values));

	refera_symbol_t sum = refera_sum(arr);
	CU_ASSERT_EQUAL(sum.dim, 0);
	CU_ASSERT_EQUAL(sum.size1, 0);
	CU_ASSERT_EQUAL(sum.size2, 0);
	CU_ASSERT_EQUAL(*sum.base, 15);
}

void test_functions_diag()
{
	const int arr_values[4][5] = {
		{  1,  2,  3,  4,  5 },
		{  6,  7,  8,  9, 10 },
		{ 11, 12, 13, 14, 15 },
		{ 16, 17, 18, 19, 20 },
	};
        refera_symbol_t arr = refera_create_variable(2, 4, 5);
	memcpy(arr.base, arr_values, sizeof(arr_values));

	refera_symbol_t diag = refera_diag(arr);
	CU_ASSERT_EQUAL(diag.dim, 1);
	CU_ASSERT_EQUAL(diag.size1, 4);
	CU_ASSERT_EQUAL(diag.size2, 0);

	CU_ASSERT_EQUAL(diag.base[0], 1);
	CU_ASSERT_EQUAL(diag.base[1], 7);
	CU_ASSERT_EQUAL(diag.base[2], 13);
	CU_ASSERT_EQUAL(diag.base[3], 19);
}

void test_functions_aver()
{
	const int arr_values[] = { 5, 4, 3, 2, 1 };
        refera_symbol_t arr = refera_create_variable(1, 5, 0);
	memcpy(arr.base, arr_values, sizeof(arr_values));

	refera_symbol_t avg = refera_aver(arr);
	CU_ASSERT_EQUAL(avg.dim, 0);
	CU_ASSERT_EQUAL(avg.size1, 0);
	CU_ASSERT_EQUAL(avg.size2, 0);
	CU_ASSERT_EQUAL(*avg.base, 3);
}

void test_functions_print()
{
	char buffer[256];
	FILE* org_stdout = stdout;
	FILE* tmp_stdout = tmpfile();

	stdout = tmp_stdout;

	const int arr_values[] = { 5, 4, 3, 2, 1 };
        refera_symbol_t arr = refera_create_variable(1, 5, 0);
	memcpy(arr.base, arr_values, sizeof(arr_values));

	refera_print(arr);

	fseek(tmp_stdout, 0, SEEK_SET);
	fgets(buffer, sizeof(buffer), tmp_stdout);

	stdout = org_stdout;

	fclose(tmp_stdout);
	CU_ASSERT_STRING_EQUAL(buffer, "5 4 3 2 1 \n");
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
