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

void test_refera_state_init()
{
	refera_state_t test_state;
	refera_state_init(&test_state);
	CU_ASSERT_EQUAL(test_state.variables_idx, 0);
	CU_ASSERT_EQUAL(test_state.error_message, NULL);
}

void test_refera_set_variable()
{
	refera_state_t test_state;
	refera_state_init(&test_state);

	refera_set_variable(&test_state, "var1", refera_create_variable(0, 0, 0));
	CU_ASSERT_EQUAL(test_state.variables_idx, 1);
	CU_ASSERT_STRING_EQUAL(test_state.variables[0].symbol, "var1");

	refera_set_variable(&test_state, "var2", refera_create_variable(1, 5, 0));
	CU_ASSERT_EQUAL(test_state.variables_idx, 2);
	CU_ASSERT_STRING_EQUAL(test_state.variables[1].symbol, "var2");

	refera_state_destroy(&test_state);
}

void test_refera_get_variable()
{
	refera_state_t test_state;
	refera_state_init(&test_state);

	refera_set_variable(&test_state, "var1", refera_create_variable(0, 0, 0));
	refera_symbol_t* var1 = refera_get_variable(&test_state, "var1");
	CU_ASSERT_PTR_NOT_NULL_FATAL(var1);

	refera_symbol_t* var2 = refera_get_variable(&test_state, "var2");
	CU_ASSERT_PTR_NULL(var2);

	refera_set_variable(&test_state, "var2", refera_copy_variable(var1));
	var2 = refera_get_variable(&test_state, "var2");
	CU_ASSERT_PTR_NOT_NULL(var2);

	refera_state_destroy(&test_state);
}

void test_refera_state_destroy()
{
	refera_state_t test_state;
	refera_state_init(&test_state);
	refera_set_variable(&test_state, "a", refera_create_variable(0, 0, 0));
	refera_set_variable(&test_state, "b", refera_create_variable(1, 5, 0));
	refera_set_variable(&test_state, "c", refera_create_variable(2, 3, 5));
	refera_state_destroy(&test_state);
	CU_ASSERT_EQUAL(test_state.variables_idx, 3);

	for (size_t i = 0; i < test_state.variables_idx; i++)
		CU_ASSERT_PTR_NULL(test_state.variables[i].base);
}

void test_refera_eval_string()
{
	refera_state_t test_state;
	refera_state_init(&test_state);

	refera_eval_string(&test_state, "var1 = 5");

	CU_ASSERT_STRING_EQUAL(test_state.variables[0].symbol, "var1");
	CU_ASSERT_EQUAL(test_state.variables_idx, 1);
	CU_ASSERT_EQUAL(test_state.variables[0].dim, 0);
	CU_ASSERT_EQUAL(test_state.variables[0].size1, 0);
	CU_ASSERT_EQUAL(test_state.variables[0].size2, 0);
	CU_ASSERT_PTR_NOT_NULL_FATAL(test_state.variables[0].base);
	CU_ASSERT_EQUAL(test_state.variables[0].base[0], 5);

	refera_state_destroy(&test_state);
}

void test_refera_eval_file()
{
	refera_state_t test_state;
	refera_state_init(&test_state);

	CU_ASSERT(false); // TODO: Baris Getiren

	refera_state_destroy(&test_state);
}

void test_refera_copy_variable()
{
	refera_symbol_t simple = refera_create_variable(0, 0, 0);
	*simple.base = 100; // Mutation

	refera_symbol_t copied = refera_copy_variable(&simple);
	CU_ASSERT_EQUAL(copied.dim, 0);
	CU_ASSERT_EQUAL(copied.size1, 0);
	CU_ASSERT_EQUAL(copied.size2, 0);
	CU_ASSERT_EQUAL(*copied.base, 100);
	CU_ASSERT_PTR_NOT_EQUAL(copied.base, simple.base);

	const int arr_values[] = { 5, 4, 3, 2, 1 };
	refera_symbol_t arr = refera_create_variable(1, 5, 0);
	memcpy(arr.base, arr_values, sizeof(arr_values));

	refera_symbol_t copied_arr = refera_copy_variable(&arr);
	CU_ASSERT_EQUAL(copied_arr.dim, 1);
	CU_ASSERT_EQUAL(copied_arr.size1, 5);
	CU_ASSERT_EQUAL(copied_arr.size2, 0);
	CU_ASSERT_PTR_NOT_EQUAL(copied_arr.base, arr.base);
	for (size_t i = 0; i < sizeof(arr_values) / sizeof(int); i++)
		CU_ASSERT_EQUAL(arr.base[i], arr_values[i]);

	const int matrix_values[] = { 15, 14, 13, 12, 11, 10, 9, 8,
                                       7,  6,  5,  4,  3,  2,  1 };
	refera_symbol_t matrix = refera_create_variable(2, 3, 5);
	memcpy(matrix.base, matrix_values, sizeof(matrix_values));

	refera_symbol_t copied_matrix = refera_copy_variable(&matrix);
	CU_ASSERT_EQUAL(copied_matrix.dim, 2);
	CU_ASSERT_EQUAL(copied_matrix.size1, 3);
	CU_ASSERT_EQUAL(copied_matrix.size2, 5);
	CU_ASSERT_PTR_NOT_EQUAL(copied_matrix.base, matrix.base);
	for (size_t i = 0; i < sizeof(matrix_values) / sizeof(int); i++)
		CU_ASSERT_EQUAL(matrix.base[i], matrix_values[i]);
}

void test_refera_create_variable()
{
	refera_symbol_t simple = refera_create_variable(0, 0, 0); // Simple 0-dim variable
	CU_ASSERT_EQUAL(simple.dim, 0);
	CU_ASSERT_EQUAL(simple.size1, 0);
	CU_ASSERT_EQUAL(simple.size2, 0);
	CU_ASSERT_PTR_NOT_NULL_FATAL(simple.base);
	CU_ASSERT_EQUAL(simple.base[0], 0);

	refera_symbol_t arr = refera_create_variable(1, 5, 0); // 1-dim array
	CU_ASSERT_EQUAL(arr.dim, 1);
	CU_ASSERT_EQUAL(arr.size1, 5);
	CU_ASSERT_EQUAL(arr.size2, 0);
	CU_ASSERT_PTR_NOT_NULL_FATAL(arr.base);
	for (size_t i = 0; i < 5; i++)
		CU_ASSERT_EQUAL(arr.base[i], 0);

	refera_symbol_t matrix = refera_create_variable(2, 8, 6); // 2-dim array
	CU_ASSERT_EQUAL(matrix.dim, 2);
	CU_ASSERT_EQUAL(matrix.size1, 8);
	CU_ASSERT_EQUAL(matrix.size2, 6);
	CU_ASSERT_PTR_NOT_NULL_FATAL(arr.base);
	for (size_t i = 0; i < 6 * 8; i++)
		CU_ASSERT_EQUAL(matrix.base[i], 0);
}

int test_refera_register(CU_pSuite* suite)
{
	*suite = CU_add_suite("Refera", 0, 0);
	if (*suite == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "state_init()", test_refera_state_init) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "create_variable()", test_refera_create_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "copy_variable()", test_refera_copy_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "set_variable()", test_refera_set_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "get_variable()", test_refera_get_variable) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "state_destroy()", test_refera_state_destroy) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "eval_file()", test_refera_eval_file) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "eval_string()", test_refera_eval_string) == NULL)
		return CU_get_error();

	return 0;
}
