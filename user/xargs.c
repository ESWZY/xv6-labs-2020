#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
  if(argc < 2) {
    printf("xargs: bad arguements\n");
    exit(-1);
  }
  
  // int i = 0;
  // for(i = 0; i < argc; i++)
  //   printf("%d %s\n", i, argv[i]);
  
  char cmdbuf[1006] = "";
  char* cmd_pointer[MAXARG];
  int i = 0;
  int j = argc - 1;
  char readchar;

  for(int k = 0; k < argc - 1; k++)
    cmd_pointer[k] = argv[k + 1];
  cmd_pointer[j++] = cmdbuf;

  while(1) {
    int readlen = read(0, &readchar, sizeof(char));
    
    // Incoming char
    if(readlen == sizeof(char) && readchar != '\n' && readchar != '\0') {
      // printf("%d: %c\n", i, readchar);
      cmdbuf[i] = readchar;
      if(readchar == ' ') {
        // cmdbuf[i] = '\0';
        cmd_pointer[j++] = cmdbuf + i + 1;
      }
      i++;
    } 

    // The end of one cmd.
    else if(strlen(cmdbuf)) {
      cmdbuf[i] = 0;
      if(fork() == 0) {
        // printf("cmd: |%s|\n", cmdbuf);
        
        for(int x = 0; x < strlen(cmdbuf); x++)
          if(cmdbuf[x] == ' ')
            cmdbuf[x] = '\0';

        cmd_pointer[j] = 0;
        // for(int x = 0; x < j; x++)
        //   printf("cmd_p: %s\n", cmd_pointer[x]);

        exec(argv[1], cmd_pointer);
      }
      else {
        wait(0);
        cmdbuf[0] = 0;
        i = 0;
        j = argc - 1;
        memset(cmd_pointer, 0, sizeof(cmd_pointer));
        memset(cmd_pointer, 0, sizeof(cmd_pointer));
        cmd_pointer[0] = cmdbuf;
        for(int k = 0; k < argc - 1; k++)
          cmd_pointer[k] = argv[k + 1];
        cmd_pointer[j++] = cmdbuf;
      }
    }
    
    if(readlen != sizeof(char) || readchar == '\0')
      break;
  }

  
  exit(0);
}

