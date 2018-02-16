//Yonathan Mekonnen
//mekonneny
//V00714698
//this file implements the "malloc", "free", "callac" and "realloc" functionalities.

#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <assignment2-support.h>



int main()
{

	char* n1 = (char*)malloc(20);
  showHeap();
  memory_leaks();
	char* n2 = (char*)malloc(30);
  showHeap();
  memory_leaks();
	char* n3 = (char*)malloc(50);
  showHeap();
  memory_leaks();
	char* n4 = (char*)malloc(60);
  showHeap();
  memory_leaks();
	char* n5 = (char*)malloc(70);
  showHeap();
  memory_leaks();
	char* n6 = (char*)malloc(80);
  showHeap();
  memory_leaks();
	char* n7 = (char*)malloc(100);
  showHeap();
  memory_leaks();
	char* n8 = (char*)malloc(120);
  showHeap();
  memory_leaks();
	char* n9 = (char*)malloc(2000);
  showHeap();
  memory_leaks();
	char* n10 = (char*)malloc(5000);
  showHeap();
  memory_leaks();

	free(n1);
	free(n2);
	free(n3);
	free(n4);
	free(n5);
  free(n6);
  free(n7);
  free(n8);
  free(n9);
  free(n10);


	n2 = (char*)calloc(200, sizeof(char));
  showHeap();
  memory_leaks();
	n4 = (char*)calloc(300, sizeof(char));
  showHeap();
  memory_leaks();
	n6 = (char*)calloc(500, sizeof(char));
  showHeap();
  memory_leaks();
	n8 = (char*)calloc(600, sizeof(char));
  showHeap();
  memory_leaks();
	n10 = (char*)calloc(700, sizeof(char));
  showHeap();
  memory_leaks();
  n2 = (char*)calloc(900, sizeof(char));
  showHeap();
  memory_leaks();
  n3 = (char*)calloc(1200, sizeof(char));
  showHeap();
  memory_leaks();
  n5 = (char*)calloc(1500, sizeof(char));
  showHeap();
  memory_leaks();
  n6 = (char*)calloc(1700, sizeof(char));
  showHeap();
  memory_leaks();
  n10 = (char*)calloc(1900, sizeof(char));
  showHeap();
  memory_leaks();

	free(n6);
	free(n7);
	free(n8);
	free(n9);
	free(n10);

  n1 = (char*)realloc(n1, 400);
  showHeap();
  memory_leaks();
	n3 = (char*)realloc(n3, 600);
  showHeap();
  memory_leaks();
	n5 = (char*)realloc(n5, 700);
  showHeap();
  memory_leaks();
	n7 = (char*)realloc(n7, 800);
  showHeap();
  memory_leaks();
	n9 = (char*)realloc(n9, 900);
  showHeap();
  memory_leaks();

	n1 = (char*)realloc(n2, 4000);
  showHeap();
  memory_leaks();
	n4 = (char*)realloc(n4, 5000);
  showHeap();
  memory_leaks();
	n7 = (char*)realloc(n6, 6000);
  showHeap();
  memory_leaks();
	n8 = (char*)realloc(n8, 7000);
  showHeap();
  memory_leaks();
	n9 = (char*)realloc(n10, 8000);
  showHeap();
  memory_leaks();








	return 0;
}
