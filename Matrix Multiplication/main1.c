#include <sys/times.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

double start, stop, used, mf;

volatile double *mem;
sem_t *sem;

void work ( int me )
{
    int i, n;
    n = 10000;
    for ( i = 0; i < n; i++ ) {
        sem_wait ( sem );
        *mem = *mem + 1;
        sem_post ( sem );
      
    }
}

double ftime(void);

double ftime (void)
{
    struct tms t;

    times ( &t );

    return (t.tms_utime + t.tms_stime) / 100.0;
}

int min(int a, int b)
{
  if (a <= b)
  {
    return a;
  }
  else
  {
    return b;
  }
}

void normalMultiplication (double **a, double **b, double **c, int n)
{
   int i, j, k;
   //setting values to 0
   for (i=0; i<n; i++)
   {
     for (j=0; j<n; j++)
     {
         c[i][j] = 0;
    }
}
    for (i=0; i<n; i++)
    {
       for (j=0; j<n; j++)
       {
         for (k=0; k<n; k++)
         {
           c[i][j]= c[i][j] + a[i][k] * b[k][j];
        }
     }
  }
}

void transposeMultiplication (double **a, double **b, double **c, int n)
{
   int i, j, k;
   //setting values to 0
   for (i=0; i<n; i++)
   {
     for (j=0; j<n; j++)
     {
         c[i][j] = 0;
    }
}
    for (i=0; i<n; i++)
    {
       for (j=0; j<n; j++)
       {
         for (k=0; k<n; k++)
         {
           c[i][j] += a[i][k] * b[j][k];
        }
     }
  }
}

void printMatrix(double**c, int n)
{
  int i, j;

  for (i = 0; i < n; i++)
  {
    for (j = 0; j < n; j++)
    {
      printf("%10.2f ", c[i][j]);
    }
  }
}

void blockMultiplication (double **a, double **b, double **c, int n)
{
  int i, j, k, l, m, o;
  int d = (n/2);
  //setting values to 0
  for (i=0; i<n; i++)
  {
    for (j=0; j<n; j++)
    {
        c[i][j] = 0;
   }
}
 

  for (l = 0; l < n; l += d)
  {
    for (m = 0; m < n; m += d)
    {
      for (o = 0; o < n; o += d)
      {
        for (i = l; i < min(l + d, n); i++)
        {
          for (j = m; j < min(m + d, n); j++)
          {
            for (k = o; k < min(o + d, n); k++)
            {
              c[i][j] += a[i][k] * b[k][j];
            }
          }
        }
      }
    }
  }
}

void forkedBlockMultiplication(double **a, double **b, double **c, int n){

  int d = (n/2);
  int pid, kids = 0, shmfd;
  int i, j, k, l,m,o;
  int row,col;
  shmfd = shm_open ( "/mekonneny_memory", O_RDWR | O_CREAT, 0666 );
    if ( shmfd < 0 ) {
        fprintf(stderr,"Could not create mekonneny_memory\n");
        exit(1);
    }
    ftruncate ( shmfd, sizeof(double) );
  mem = (double *)mmap ( NULL, (n)*sizeof(double), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0 );
  if ( mem < 0 ) {
      fprintf(stderr,"Could not map mekonneny_memory\n");
      exit(1);
  }
  close ( shmfd );
  shm_unlink ( "/mekonneny_memory" );

  sem = sem_open ( "mekonneny_sem", O_CREAT, 0666, 1 );
  if ( sem == NULL ) {
      fprintf(stderr,"Could not create mekonneny semaphore\n");
      exit(1);
  }
  sem_unlink ( "mekonneny_sem" );
  mem[0] = 0;
  for (l = 0; l < n; l+=d)
  {
    for (m = 0; m < n; m+=d)
    {
      for (o = 0; o < n; o+=d)
       {
         kids++;;
         pid = fork();
         if ( pid < 0 )
          {
            // fprintf(stderr,"fork failed at %d\n",i);
             exit(1);
           }
           else if ( pid > 0 ) {
           //  printf("parent: new child is %d\n",pid);
            }
            else {
             printf("child %d, parent is %d\n",i, getppid());
             for (i = l; i < min(l+d, n); i++)
             {
               for (j = m; j < min(m+d, n); j++)
               {
                 for (k = o; k < min(o+d, n); k++)
                 {
                   mem[i*n+j] = mem[i*n+j] + a[i][k] * b[k][j];
                  // printf("%10.2f\n",mem[i*n+j]);
                 }
               }
             }
             work(o);
             exit(0);
           }
         }
       }
       
     }
for (i = 0; i <kids; i++){
       wait(NULL);
   }
  }
int main (void)
{
   int i, j, n, d;
   double **a, **b, **c;

    printf ( "Enter the value of n: ");
    scanf ( "%d", &n);
     

   //Populate arrays....
     a= (double**)malloc(n*sizeof(double));
     b= (double**)malloc(n*sizeof(double));
     c= (double**)malloc(n*sizeof(double));

     for (i=0; i<n; i++)
     {
       a[i]= malloc(sizeof(double)*n);
       b[i]= malloc(sizeof(double)*n);
       c[i]= malloc(sizeof(double)*n);
      }

     for (i=0; i<n; i++)
     {
        for (j=0; j<n; j++)
         a[i][j]=rand()%10;
      }

     for (i=0; i<n; i++)
     {
        for (j=0; j<n; j++)
          b[i][j]=rand()%10;
      }
      printf("----------------------------------");
      
      printf("----------------------------------------");
      printf("Normal Matrix Multiplication \n");
      printf("\n");

     // printf("matrix: a\n");
     // printMatrix(a, n);

     // printf("matrix: b\n");
     // printMatrix(b, n);

      start = ftime();
      normalMultiplication (a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;

      //printf("matrix: c\n");
      //printMatrix(c, n);

      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n", mf);
      printf("----------------------------------");

      printf("----------------------------------------");
      printf("Transposed Matrix Multiplication \n");
      printf("\n");

     // printf("matrix: a\n");
     // printMatrix(a, n);
     

     // printf("transposed matrix: b\n");
     // printMatrix(b, n);

      start = ftime();
      transposeMultiplication (a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;

      //printf("matrix: c\n");
      //printMatrix(c, n);

      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n", mf);
      printf("----------------------------------");

      printf("----------------------------------------");
      printf("Blocked Matrix Multiplication \n");
      printf("\n");

     // printf("matrix: a\n");
     // printMatrix(a, n);

     // printf("matrix: b\n");
     // printMatrix(b, n);

      start = ftime();
      blockMultiplication (a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;

      //printf("matrix: c\n");
      //printMatrix(c, n);

      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n", mf);
      printf("----------------------------------");
    
      printf("----------------------------------------");
      printf("Fork Blocked Matrix Multiplication \n");
      printf("\n");

     // printf("matrix: a\n");
     // printMatrix(a, n);

     // printf("matrix: b\n");
     // printMatrix(b, n);

      
      start = ftime();
      forkedBlockMultiplication(a,b,c,n);
      stop = ftime();
      used = stop - start;
      mf = (n*n*n *2.0) / used / 1000000.0;

     // printf("matrix: c\n");
     // printMatrix(c, n);

      printf ("\n");
      printf ( "Elapsed time:   %10.2f \n", used);
      printf ( "DP MFLOPS:       %10.2f \n", mf);
      printf("----------------------------------");
      return (0);
}

