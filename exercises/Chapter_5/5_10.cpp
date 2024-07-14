#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>
#include <string>

int eval_polish(int argc, char *argv[]) {
  if (argc < 1) {
    throw std::invalid_argument("There must be at least one argument.");
  }
  int first = std::strtol(argv[0], nullptr, 10);
  if (argc == 1) {
    return first;
  }
  if (argv[argc - 1][0] == '*') {
    return first * eval_polish(argc - 2, argv + 1);
  }
  if (argv[argc - 1][0] == '/') {
    return first / eval_polish(argc - 2, argv + 1);
  }
  if (argv[argc - 1][0] == '+') {
    return first + eval_polish(argc - 2, argv + 1);
  }
  if (argv[argc - 1][0] == '-') {
    return first - eval_polish(argc - 2, argv + 1);
  }

  std::string message = "The final arg must be either *, /, +, or -. Got ";
  for (int i = 0; i < argc; i++) {
    message += "\"";
    message += argv[i];
    message += "\"\t";
  }
  throw std::invalid_argument(message.c_str());
}

int main(int argc, char *argv[]) {
  printf("%d\n", eval_polish(argc - 1, argv + 1));
  return 0;
}
