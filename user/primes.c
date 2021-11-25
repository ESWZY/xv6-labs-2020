#include "kernel/types.h"
#include "user/user.h"

const uint INT_LEN = sizeof(int);

int filter(int pl[]) {
  int num_read_from_left = -9;
  int rcv_len_the_first_num = read(pl[0], &num_read_from_left, INT_LEN);
  if(rcv_len_the_first_num <= 0) {
    close(pl[0]);
    exit(0);
  }  

  printf("prime %d\n", num_read_from_left);

  int pr[2];
  pipe(pr);

  while(1) {
    // Read nums from left.
    int n = -9;
    int rcv_len = read(pl[0], &n, INT_LEN);

    if(rcv_len <= 0) {        // The end of this sieve loop.
      close(pl[0]);    // No more data from left.
      close(pr[1]);    // No more data to right.
      break;
    }
    else {              // Still have num(s).
      if(n % num_read_from_left == 0) {
        continue;
      }
      else {
        write(pr[1], &n, INT_LEN);
      }
    }
  }
  
  if(fork() == 0) {
    filter(pr);
  }
  else {
    wait((int*) 0);
  }
  close(pr[0]);
  close(pr[1]);
  exit(0);
}

int
main(int argc, char const *argv[]) {
  int p[2];
  pipe(p);

  int i = 999;
  for(i = 2; i <= 35; i++) {
    write(p[1], &i, INT_LEN);
  }
  close(p[1]);
  if(fork() == 0) {
    filter(p);
  }
  else {
    wait((int*) 0);
  }
  exit(0);

}
