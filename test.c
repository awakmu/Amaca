#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/amaca.h"

static int run = 0, failed = 0;

#define test_diag(name) printf("#%02d %s .. ", ++run, name);
#define test_cond(cond) if (cond) puts("OK"); else {puts("FAIL"); failed++;}

void test_template_simple() {
	char *result;
	char *tmpl = "This is the {{ return arg }} test";

	test_diag("Test simple #1");
	result = Amaca_template(tmpl, 1, "arg", "first");
	test_cond(strcmp(result, "This is the first test") == 0);
	free(result);

	test_diag("Test simple #2");
	result = Amaca_template(tmpl, 1, "arg", "second");
	test_cond(strcmp(result, "This is the second test") == 0);
	free(result);

	test_diag("Test simple #3");
	result = Amaca_template(tmpl, 1, "arg", "third");
	test_cond(strcmp(result, "This is the third test") == 0);
	free(result);
}

int main() {
	test_template_simple();

	if (failed > 0) {
		printf("%d TESTs HAVE FAILED!\n", failed);
		return -1;
	}

	return 0;
}
