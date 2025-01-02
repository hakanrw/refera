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
#include "../src/operations.h"

void test_parser_parse_set()
{
	const char* operation = "x = 2";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_SET);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "x");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "2");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_copy()
{
	const char* operation = "B = A";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_CPY);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "B");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "A");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_sum()
{
	const char* operation = "x = sum(B)";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_SUM);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "x");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "B");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_aver()
{
	const char* operation = "x = aver(B)";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_AVG);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "x");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "B");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_diag()
{
	const char* operation = "D = diag(A)";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_DIA);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "D");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "A");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_add()
{
	const char* operation = "C = A + B";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_ADD);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "C");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "A");
	CU_ASSERT_STRING_EQUAL(PT[0].source2, "B");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_sub()
{
	const char* operation = "C = C - x";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_SUB);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "C");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "C");
	CU_ASSERT_STRING_EQUAL(PT[0].source2, "x");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_exchange()
{
	const char* operation = "D[3] <> C[5]";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_EXC);
	CU_ASSERT_FALSE(PT[0].ifexists);
}

void test_parser_parse_if()
{
	const char* operation = "if (A[1,2] > 10) D = A * B";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_MUL);
	CU_ASSERT_STRING_EQUAL(PT[0].destination, "D");
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "A");
	CU_ASSERT_STRING_EQUAL(PT[0].source2, "B");
	CU_ASSERT(PT[0].ifexists);
	CU_ASSERT_STRING_EQUAL(PT[0].operand1.symbol, "A");
	CU_ASSERT_EQUAL(PT[0].operand1.idx1, 1);
	CU_ASSERT_EQUAL(PT[0].operand1.idx2, 2);
	CU_ASSERT_STRING_EQUAL(PT[0].operand2.symbol, "10");
}

void test_parser_parse_print()
{
	const char* operation = "print B";
	parser_parse_statement(operation);

	CU_ASSERT_EQUAL(PT[0].operation, R_PRI);
	CU_ASSERT_STRING_EQUAL(PT[0].source1, "B");
	CU_ASSERT_FALSE(PT[0].ifexists);
}

int test_parser_register(CU_pSuite* suite)
{
	*suite = CU_add_suite("Parser", 0, 0);
	if (*suite == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "( set) x = 2", test_parser_parse_set) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(copy) B = A", test_parser_parse_copy) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "( sum) x = sum(B)", test_parser_parse_sum) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(aver) x = aver(B)", test_parser_parse_aver) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(diag) D = diag(A)", test_parser_parse_diag) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "( add) C = A + B", test_parser_parse_add) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "( sub) C = C - x", test_parser_parse_sub) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(exch) D[3] <> C[5]", test_parser_parse_exchange) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(  if) if (A[1,2] > 10) D = A * B", test_parser_parse_if) == NULL)
		return CU_get_error();

	if (CU_add_test(*suite, "(prnt) print B", test_parser_parse_print) == NULL)
		return CU_get_error();

	return 0;
}
