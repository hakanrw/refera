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

#include "test.h"
#include "test_functions.h"
#include "test_parser.h"
#include "test_refera.h"

int main()
{
	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	CU_pSuite functions_suite;
	CU_pSuite parser_suite;
	CU_pSuite refera_suite;

	int functions_reg_result = test_functions_register(&functions_suite);
	if (functions_reg_result)
		return functions_reg_result; // returned non-zero return value

	int parser_reg_result = test_parser_register(&parser_suite);
	if (parser_reg_result)
		return parser_reg_result; // returned non-zero return value

	int refera_reg_result = test_refera_register(&refera_suite);
	if (refera_reg_result)
		return refera_reg_result; // returned non-zero return value

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return 0;
}
