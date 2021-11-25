#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char const *argv[]) {
  int p1[2];
  int p2[2];
  char buf = 'A';
  char b = 'B';
  char c = 'C';
  
  pipe(p1);
  pipe(p2);

  int pid = fork();

  if(pid == 0) {
    close(p1[1]);
    while(read(p1[0], &buf, sizeof(buf)) < 0)
      continue;
    int cpid = getpid();
    // printf("%d: received ping %c\n", cpid, buf);
    printf("%d: received ping\n", cpid);
    // write(0, atoi(cpid), sizeof(atoi(cpid)));
    // write(0, buf, sizeof(buf));
    write(p2[1], &c, sizeof(buf));
    close(p1[0]);
    exit(0);
  }
  else {
    close(p1[0]);
    write(p1[1], &b, sizeof(buf));
    wait((int*)0);
    while(read(p2[0], &buf, sizeof(buf)) < 0)
      continue;
    int cpid = getpid();
    // printf("%d: received pong %c\n", cpid, buf);
    printf("%d: received pong\n", cpid);
    // write(0, atoi(cpid), sizeof(atoi(cpid)));
    // write(0, buf2, sizeof(buf2));
    close(p2[0]);
    exit(0);
  }

  exit(0);
}
