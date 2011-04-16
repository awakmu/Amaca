#include <stdio.h>

#include "../src/amaca.h"

int main() {
	char *str = Amaca_template_file(
		"eg/example.tmpl",
		"title", "This is a title",
		"body", "This is the body"
	);

	printf("%s", str);

	return 0;
}
