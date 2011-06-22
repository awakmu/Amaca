#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/amaca.h"

static int run = 0, failed = 0;

#define test_diag(name) printf("#%02d %s .. ", ++run, name);
#define test_cond(cond) if (cond) puts("OK"); else {puts("FAIL"); failed++;}

void test_template_generic() {
	char *result;
	char *arg = "first";
	char *tmpl = "This is the {{ return arg }} test";

	test_diag("Test number 1");
	result = Amaca_template(tmpl, "arg", arg);
	test_cond(strcmp(result, "This is the first test") == 0);
	free(result);

	/*test_diag("Test number 2");
	result = Amaca_template(tmpl, "i", "2");
	test_cond(strcmp(result, "This is the 2nd number") == 0);
	free(result);*/

	/*test_diag("Test number 3");
	result = Amaca_template(tmpl, "i", "3");
	test_cond(strcmp(result, "This is the 3rd number") == 0);
	free(result);*/
}

int main() {
	test_template_generic();

	if (failed > 0) {
		printf("%d TESTs HAVE FAILED!\n", failed);
		return -1;
	}

	return 0;
}
