#include <stdio.h>

import namespace example;
export namespace Foo {
public:

int main (int args, char ** argv) {
	printf ("blah\n");
	return 0;
}

private:

void foo () {
	printf ("you can't see me.\n");
	return;
}

};

