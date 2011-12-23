#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../src/amaca.h"

int main() {
	char *title = "This is a title";
	char *body  = "This is the body";

	char *str = amaca_eval_fd(
		STDIN_FILENO, 2,
		"title", title,
		"body", body
	);

	printf("%s", str);

	free(str);
	return 0;
}
