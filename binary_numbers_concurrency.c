/*
 * Printing binary numbers on n bit 
 * by using more processes (no recursion, but concurrency)
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

void Print(int [], int);

void Print(int vett[], int n){
 for(int i=0; i<n; i++)
  printf("%d ",vett[i]);
 printf("\n");
 return;
}



int main(int argc, char* argv[]){
 int n = atoi(argv[1]);
 int status, vett[n];
 
 for(int i=0; i<n; i++){
  if(fork()){
   if(!fork()){
    vett[i]=0;
    if(i==(n-1)){
     Print(vett,n);
     return 0;
    }
   }
   else {
    wait(&status);
    wait(&status);
    return 0;
   }
  }
  else{
   vett[i]=1;
   if(i==(n-1)){
    Print(vett,n);
    return 0;
   }
  }
 }
 return 0;
}
