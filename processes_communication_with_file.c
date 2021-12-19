/*
 * Two children communicate 
 * by reading and writing a file
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <ctype.h> 
#include <string.h>
#define N 30

void SignalCatcher(int sig);
int Receiver(int f1);
int Sender(int f2,int f1);

void SignalCatcher(int sig){ //process wakes up when a SIGALRM arrives
 if(sig == SIGALRM)
  return;
}

int Receiver(pid_t f1){
 FILE* fp;
 int pid_brother;
 char str[N];

 while(1){
  pause();
  fp = fopen("test.txt","r");
  fscanf(fp,"%d %s", &pid_brother,str);
  fclose(fp);
  fflush(stdout);

  //UPPERCASE CONVERSION
  for(int i=0; i<strlen(str); i++)
   str[i] = toupper(str[i]);
  printf("RECEIVER (%d) WOKE UP! Message in capital letter -> %s\n",f1,str);
  kill(pid_brother,SIGALRM);
 }
 return 0;
}

int Sender(int f2, int f1){
 FILE* fp;
 char str[N];
 
 while(1){
  fprintf (stdout, "SENDER (%d) WOKE UP: ",f2);
  fflush (stdout);
  scanf ("%s", str);
  fp = fopen("test.txt","w");
  fprintf(fp,"%d %s",f2,str);
  fclose(fp);
  kill(f1,SIGALRM);
  pause();
 }
 return 0;
}




int main(int argc, char* argv[]){ 
  int f1,f2;
 int status;
 signal(SIGALRM, SignalCatcher); //initialize the Signal Handler
 
 if((f1=fork()) != 0){ //Inside the father
   if((f2=fork()) == 0){ //Inside the second child (F2) (Sender)
   Sender(getpid(), f1);
  }
  else{ //waiting for the two children
   printf("SENDER: %d\n",f2);
   printf("RECEIVER: %d\n",f1);
   wait(&status);
   wait(&status);
  }
 }
 else{ //Inside the first child (Receiver)
  Receiver(getpid());
 }

 return 0;
}

