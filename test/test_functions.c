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

int test_functions_register(CU_pSuite suite)
{
	if (CU_add_test(suite, "functions: sum()", test_functions_sum) == NULL)
		return CU_get_error();

	if (CU_add_test(suite, "functions: diag()", test_functions_diag) == NULL)
		return CU_get_error();

	if (CU_add_test(suite, "functions: aver()", test_functions_aver) == NULL)
		return CU_get_error();

	if (CU_add_test(suite, "functions: print()", test_functions_print) == NULL)
		return CU_get_error();

	return 0;
}
