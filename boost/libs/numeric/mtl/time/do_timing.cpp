#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>

std::string int2str(int i) {
  char str[80];
  snprintf(str, 80, "%d", i);
  return str;
}

int main(int argc, char* argv[])
{
  if (argc < 5) {
    std::cerr << "do_timing <program> <start> <end> <stride> <trial>" << std::endl;
    return EXIT_FAILURE;
  }
  std::string exe = std::string("./") + argv[1];
  int start = atoi(argv[2]);
  int end = atoi(argv[3]);
  int stride = atoi(argv[4]);
  std::string trials = argv[5];

  for (int i = start; i < end; i += stride) {
    std::string cmd = exe + " " + int2str(i)
      + " " + int2str(i) + " " + trials;
    std::system(cmd.c_str());
  }
  return 0;
}
