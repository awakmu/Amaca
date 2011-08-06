#include <stdio.h>

#include "../src/amaca.h"

int main() {
	char *title = "This is a title";
	char *body  = "This is the body";

	char *str = amaca_eval_file(
		"eg/example.tmpl", 2,
		"title", title,
		"body", body
	);

	printf("%s", str);

	return 0;
}
