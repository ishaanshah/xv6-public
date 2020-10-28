#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf(2, "setPriority: invalid number of arguments\n");
    exit();
  }

  set_priority(atoi(argv[1]), atoi(argv[2]));
  exit();
}
