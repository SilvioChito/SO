/*
 * Two pipes are used as a way to syncronyse two processes
 * IN this case no pause/sleep is needed
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int SincP(int fr, int fw);
int SincF(int fr, int fw);

int SincP(int fr, int fw){
  char cW='a',cR;
  int i=0;

  fprintf(stdout,"Father (PID: %d) woke up!!!\n",getpid());
  write(fw,&cW,1); //The communication starts when the father send the first message on the pipe

  while(read(fr,&cR,1)!=-1){
    i++;
    fprintf(stdout,"Father (PID: %d) woke up!!!\n",getpid());
    if(i==10){ //after 10 cycles, the two processes end
     cW='e';   
     write(fw,&cW,1);   
     return 0;
    }
    write(fw,&cW,1);
  }
  return 0;
}

int SincF(int fr, int fw){
  char cW='a',cR;
  
  while(read(fr,&cR,1)!=-1){
    fprintf(stdout,"Child (PID: %d) woke up!!!\n",getpid());
    if(cR=='e')
     return 0;
    write(fw,&cW,1);
  }
  return 0;
}


int main(int argc, char* argv[]){
  int fd1[2], fd2[2], status;

  if(pipe(fd1)==0){
    if(pipe(fd2)==0){
      if(fork()){
       close(fd1[1]); /*closes fd1[1] for writing
                        -> uses fd1[0] for reading*/
       close(fd2[0]); /*closes fd2[0] for writing
                        -> uses fd2[1] for reading*/
       SincP(fd1[0],fd2[1]);
       wait(&status);
       return 0;
      }
      else {
       close(fd1[0]); //the same thing done before, is done now
       close(fd2[1]);
       SincF(fd2[0],fd1[1]);
      }
    }
  }
}
