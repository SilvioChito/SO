#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define N 30

struct par{
 char strI[N];
 char strO[N];
};


void *SortFile(void *arg){
 int dim,i,j,tmp;
 int *vett;
 struct par* p = (struct par*) arg;
 FILE *fp, *fp2;

 fp = fopen(p->strI,"r");
 fp2 = fopen(p->strO,"w");

 fscanf(fp,"%d",&dim);
 vett = (int*) malloc(dim*sizeof(int));

 for(i=0; i<dim; i++)
  fscanf(fp,"%d",&vett[i]); //inserisco nel vettore tutti i dati in un vettore

 //BUBBLE SORT 
 for (i=0; i<dim-1; i++) {
  for (j=0; j<dim-1-i; j++) {
   if (vett[j] > vett[j+1]){
     tmp = vett[j];
     vett[j] = vett[j+1];
     vett[j+1] = tmp;
    }
  }
 }

 //adesso stampo il vettore sui file di uscita (in questo caso li genero ogni volta -> quelli in input li avevo già
 for(i=0; i<dim; i++)
  fprintf(fp2,"%d\n",vett[i]);

 fclose(fp);
 fclose(fp2);

 pthread_exit(NULL);
}


int main(int argc, char* argv[]){
 int i;
 int n = atoi(argv[1]);
 char tmp[N] = "";
 struct par p[n];
 pthread_t t[n];
 void *status; //per catturare lo stato di ritorno dei vari thread
 
 
 for(i=0; i<n; i++){ //genero tutti i file ingresso/uscita su cui lavoreranno i thread
  sprintf(tmp,"%s%d.txt",argv[2],i);
  strcpy(p[i].strI,tmp);
  sprintf(tmp,"%s%d.txt",argv[3],i);
  strcpy(p[i].strO,tmp);
 }
  
 for(i=0; i<n; i++)
  pthread_create(&t[i],NULL,SortFile,(void *) &p[i]);

 for(i=0; i<n; i++)
  pthread_join(t[i],&status);

 return 0;
}
