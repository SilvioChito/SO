/*
 *  
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>

void child(int fd, int j);

int main(int argc, char* argv[]){
  int fd, n, x;
  int i,j; //counters

  fd = creat (argv[1], 0666);

  fd = open(argv[1], O_RDWR);
  if(fd == -1){
    printf("Error while opening the file...");
    return 1;
  }

  srand(time(NULL)); //initialize the random number generator

  //now I write 4 random numbers on the binary file
  n=10;
  for(int i=0; i<n; i++){
    x=random()%20;
    printf("%d\n",x);
    write(fd, &x, sizeof(int));
  }

  for (i=0; i<n-1; i++) {
   for (j=0; j<n-1-i; j++) {
    if(fork()==0){ //siamo nel figlio
      child(fd,j);
      return 0;
    }
    else{
	wait((int* ) 0);
    }
   }
  }

  lseek(fd,0,SEEK_SET); 
  printf("\n");
  while(read(fd,&x,sizeof(int)) == sizeof(int)){
    printf("%d\n",x);
  }
  close(fd);
  return 0;
}

void child (int fd,int j){
  int v1, v2, c1, c2;

  lseek (fd, j*sizeof (int), SEEK_SET);
  c1 = read (fd, &v1, sizeof (int));
  c2 = read (fd, &v2, sizeof (int));
  if (c1<=0 || c2<=0) {
    fprintf (stderr, "Error reading file\n");
    exit (-1);
  }
  if (v1 > v2) {
    lseek (fd, j*sizeof (int), SEEK_SET);
    c1 = write (fd, &v2, sizeof (int));
    c2 = write (fd, &v1, sizeof (int));
  }

  return;
}
