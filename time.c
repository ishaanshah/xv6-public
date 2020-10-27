#include "types.h"
#include "user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf(2, "time: invalid number of arguments\n");
    exit();
  }

  int pid;
  pid = fork();
  if (pid == 0) {
    exec(argv[1], argv);
    printf(2, "exec %s failed\n", argv[1]);
  } else {
    int wtime, rtime;
    int res = waitx(&wtime, &rtime);
    if (res < 0) {
      printf(2, "time failed\n");
      exit();
    }

    printf(1, "Wait Time: %d\n", wtime);
    printf(1, "Run Time: %d\n", rtime);
    exit();
  }
}
