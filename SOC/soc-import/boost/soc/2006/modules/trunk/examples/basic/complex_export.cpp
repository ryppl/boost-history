//#include <stdio.h>

import namespace example;
export namespace Foo::Bar {
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

export namespace Foo::Bar["Baz Baz"] {
	int crack () {
		return 0;
	}
public:
	int hack () {
		return crack ();
	}
};

