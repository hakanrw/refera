#include "test.h"
#include "test_functions.h"

int main()
{
	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();

	CU_pSuite refera_suite = CU_add_suite("Refera Test Suite", 0, 0);
	if (refera_suite == NULL)
		return CU_get_error();

	int test_reg_result = test_functions_register(refera_suite);
	if (test_reg_result)
		return test_reg_result; // returned non-zero return value

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return 0;
}
