#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "src/amaca.h"

static unsigned int run = 0, failed = 0;

#define test_diag(name) printf("#%02d %s .. ", ++run, name);
#define test_cond(cond) if (cond) puts("OK"); else {puts("FAIL"); failed++;}

void test_eval_simple() {
	char *result;
	char *tmpl = "This is the {{ return arg1 .. arg2 }} test";

	test_diag("Test simple #1");
	result = amaca_eval(tmpl, 2, "arg1", "fir", "arg2", "st");
	test_cond(strcmp(result, "This is the first test") == 0);
	free(result);

	test_diag("Test simple #2");
	result = amaca_eval(tmpl, 2, "arg1", "sec", "arg2", "ond");
	test_cond(strcmp(result, "This is the second test") == 0);
	free(result);

	test_diag("Test simple #3");
	result = amaca_eval(tmpl, 2, "arg1", "thi", "arg2", "rd");
	test_cond(strcmp(result, "This is the third test") == 0);
	free(result);
}

void test_eval_multiple() {
	char *result;
	char *tmpl = "This is the {{ return arg }} test. Yes, the {{ return arg }}!";

	test_diag("Test multiple #1");
	result = amaca_eval(tmpl, 1, "arg", "first");
	test_cond(strcmp(result, "This is the first test. Yes, the first!") == 0);
	free(result);

	test_diag("Test multiple #2");
	result = amaca_eval(tmpl, 1, "arg", "second");
	test_cond(strcmp(result, "This is the second test. Yes, the second!") == 0);
	free(result);

	test_diag("Test multiple #3");
	result = amaca_eval(tmpl, 1, "arg", "third");
	test_cond(strcmp(result, "This is the third test. Yes, the third!") == 0);
	free(result);
}

void test_eval_file() {
	char *result;
	char *title = "This is a title";
	char *body  = "This is the body";

	test_diag("Test file");
	result = amaca_eval_file("eg/example.tmpl", 2, "title", title, "body", body);
	test_cond(strcmp(result, "<!DOCTYPE html>\n\n<head>\n\t<title>This is a title</title>\n</head>\n\n<body>\n\t<h1>This is the body</h1>\n\t\n\t\t<p>Number 10</p>\n\n\t\t<p>Number 9</p>\n\n\t\t<p>Number 8</p>\n\n\t\t<p>Number 7</p>\n\n\t\t<p>Number 6</p>\n\n\t\t<p>Number 5</p>\n\n\t\t<p>Number 4</p>\n\n\t\t<p>Number 3</p>\n\n\t\t<p>Number 2</p>\n\n\t\t<p>Number 1</p>\n\n</body>\n</html>\n") == 0);
	free(result);
}

void test_eval_fd() {
	char *result;
	char *title = "This is a title";
	char *body  = "This is the body";
	int fd = open("eg/example.tmpl", O_RDONLY);

	test_diag("Test fd");
	result = amaca_eval_fd(fd, 2, "title", title, "body", body);
	test_cond(strcmp(result, "<!DOCTYPE html>\n\n<head>\n\t<title>This is a title</title>\n</head>\n\n<body>\n\t<h1>This is the body</h1>\n\t\n\t\t<p>Number 10</p>\n\n\t\t<p>Number 9</p>\n\n\t\t<p>Number 8</p>\n\n\t\t<p>Number 7</p>\n\n\t\t<p>Number 6</p>\n\n\t\t<p>Number 5</p>\n\n\t\t<p>Number 4</p>\n\n\t\t<p>Number 3</p>\n\n\t\t<p>Number 2</p>\n\n\t\t<p>Number 1</p>\n\n</body>\n</html>\n") == 0);
	free(result);
	close(fd);
}

void test_veval_simple_helper(const char *template, int argc, va_list args) {
	char *result;

	test_diag("Test vtemplate simple");
	result = amaca_veval(template, 2, args);
	test_cond(strcmp(result, "This is the first test") == 0);
	free(result);
}

void test_veval_simple(int argc, ...) {
	va_list args;
	char *tmpl = "This is the {{ return arg1 .. arg2 }} test";

	va_start(args, argc);
	test_veval_simple_helper(tmpl, argc, args);
	va_end(args);
}

void test_veval_file_helper(const char *template, int argc, va_list args) {
	char *result;

	test_diag("Test vtemplate file");
	result = amaca_veval_file(template, 2, args);
	test_cond(strcmp(result, "<!DOCTYPE html>\n\n<head>\n\t<title>This is a title</title>\n</head>\n\n<body>\n\t<h1>This is the body</h1>\n\t\n\t\t<p>Number 10</p>\n\n\t\t<p>Number 9</p>\n\n\t\t<p>Number 8</p>\n\n\t\t<p>Number 7</p>\n\n\t\t<p>Number 6</p>\n\n\t\t<p>Number 5</p>\n\n\t\t<p>Number 4</p>\n\n\t\t<p>Number 3</p>\n\n\t\t<p>Number 2</p>\n\n\t\t<p>Number 1</p>\n\n</body>\n</html>\n") == 0);
	free(result);
}

void test_veval_file(int argc, ...) {
	va_list args;

	va_start(args, argc);
	test_veval_file_helper("eg/example.tmpl", argc, args);
	va_end(args);
}

void test_veval_fd_helper(const int fd, int argc, va_list args) {
	char *result;

	test_diag("Test vtemplate fd");
	result = amaca_veval_fd(fd, 2, args);
	test_cond(strcmp(result, "<!DOCTYPE html>\n\n<head>\n\t<title>This is a title</title>\n</head>\n\n<body>\n\t<h1>This is the body</h1>\n\t\n\t\t<p>Number 10</p>\n\n\t\t<p>Number 9</p>\n\n\t\t<p>Number 8</p>\n\n\t\t<p>Number 7</p>\n\n\t\t<p>Number 6</p>\n\n\t\t<p>Number 5</p>\n\n\t\t<p>Number 4</p>\n\n\t\t<p>Number 3</p>\n\n\t\t<p>Number 2</p>\n\n\t\t<p>Number 1</p>\n\n</body>\n</html>\n") == 0);
	free(result);
}

void test_veval_fd(int argc, ...) {
	va_list args;
	int fd = open("eg/example.tmpl", O_RDONLY);

	va_start(args, argc);
	test_veval_fd_helper(fd, argc, args);
	va_end(args);

	close(fd);
}

int main() {
	putchar('\n');

	test_eval_simple();
	test_eval_multiple();
	test_eval_file();
	test_eval_fd();
	test_veval_simple(2, "arg1", "fir", "arg2", "st");
	test_veval_file(2, "title", "This is a title", "body", "This is the body");
	test_veval_fd(2, "title", "This is a title", "body", "This is the body");

	if (failed > 0) {
		printf("\n%d/%d TESTS HAVE FAILED!\n", failed, run);
		return -1;
	}

	printf("\n%d/%d TESTS SUCCESSFULLY RUN!\n", run, run);

	return 0;
}
