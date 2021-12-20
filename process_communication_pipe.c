/*
 * Two children communicate by using a pipe.
 * The communication ends when the Sender send the word "end" (THE TWO PROCESSES RETURN)
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

int Receiver(int fr);
int Sender(int fw);

int Receiver(int fr){
 char str[N];

 while(1){
  if((read(fr,str,N))==N){ //if I read 3o bytes (n. of bytes of the string)
   //UPPERCASE CONVERSION
   for(int i=0; i<strlen(str); i++)
    str[i] = toupper(str[i]);

   fflush(stdout);
   fprintf(stdout,"RECEIVER (%d) WOKE UP! Message in capital letter -> %s\n",getpid(),str);
  
   if(strcmp(str,"END")==0){
    printf("RECEIVER RETURNED SUCESSFULLY !!!\n");
    return 0;
   }
  }
 }
}


int Sender(int fw){
 char str[N];

 while(1){
  fprintf (stdout, "SENDER (%d) WOKE UP: ",getpid());
  fflush (stdout);
  scanf ("%s",str);
  //writing into the pipe
  write(fw,str,N); /*N is the max dimension of the string 
                   it should have been necessary to put the exact dim of the string, 
                   in case I would have used a malloc (and so without waste more memory)*/
  sleep(1);
  if(strcmp(str,"end")==0){
   printf("SENDER RETURNED SUCESSFULLY !!!\n");
   return 0;
  }
 }
 return 0;
}




int main(int argc, char* argv[]){ 
 int f1,f2,status,fd[2];

 if(pipe(fd) == 0){ //if pipe is successfull...
  if((f1=fork()) != 0){ //Inside the father
    if((f2=fork()) == 0){ //Inside the second child (F2) (Sender)
     close(fd[0]); //close the fd that I do not need
     Sender(fd[1]); //f[1] is file descriptor for writing
    }
    else{ //waiting for the two children
     printf("SENDER: %d\n",f2);
     printf("RECEIVER: %d\n",f1);
     wait(&status);
     wait(&status);
    }
  }
  else{ //Inside the first child (Receiver)
   close(fd[1]);
   Receiver(fd[0]); //f[0] is file descriptor for reading
  }
 }
 return 0;
}
