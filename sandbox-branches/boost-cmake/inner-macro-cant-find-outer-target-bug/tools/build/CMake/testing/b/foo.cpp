#include <iostream>

int bg(int i);

int bf(int i) { bg(i); }
int bg(int i) { bf(i); }



