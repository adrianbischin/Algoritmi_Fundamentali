/*
ADRIAN BISCHIN
group 30228
In primele trei grafice (construite pe baza varierii dimensiunii listelor, de trei ori: pentru 5 liste, 10 liste respectiv 100 de liste) se observa
ca functia obtinuta este liniara, deci numarul de operatii efectuate este influentat liniar de lungimea listelor.
In schimb, din ultimul grafic (construit pe baza varierii numarului de liste) reiese ca numarul de operatii efectuate este influentat logaritmic de
numarul de liste.
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler profiler("Interclasare_5_liste");

struct node
{
	int val;
	int index;
	int list;
};

void List_Int_String(int n, int a[],string s)
{
	cout << "\n"<<s;
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << ' ';
	}
}

int** Table_Allocation(int k, int m, int n)
{
	if (m * k == n)
	{
		int** v = (int**)calloc(k, sizeof(int*));
		if (!v)
		{
			cout << "\nEroare la alocarea listelor!!";
			exit(1);
		}
		for (int i = 0; i < k; i++)
		{
			*(v + i) = (int*)calloc(m, sizeof(int));
			if (!v[i])
			{
				cout << "\nEroare la alocarea listelor!!";
				exit(1);
			}
		}
		return v;
	}
	else
	{
		cout << "CONDITIA k*m=n NU ESTE INDEPLINITA!!";
		return NULL;
	}
}

node** Heap_Allocation(int k)
{
	node** heap = (node**)calloc(k, sizeof(node*));
	if (!heap)
	{
		cout << "\nEroare la alocarea heapului!!";
		exit(3);
	}
	for (int i = 0; i < k; i++)
	{
		*(heap + i) = (node*)calloc(1, sizeof(node));
		if (!heap[i])
		{
			cout << "\nEroare la alocarea listelor!!";
			exit(3);
		}
	}
	return heap;
}

void Table_Free(int k, int** v)
{
	for (int i = 0; i < k; i++)
	{
		free(*(v+i));
	}
	free(v);
	return;
}

void Heap_Free(int k, node** heap)
{
	for (int i = 0; i < k; i++)
	{
		free(*(heap+i));
	}
	free(heap);
}

void Min_Heapify_BotomUp(node** a, int i, int ka, int k, int m, int mode)
{
	//a[i] este frunza
	if (i > (ka / 2 - 1))//sau if( (2*i+1) > (ka-1) )
	{
		return;
	}
	//a[i] are un singur copil
	if ((2 * i + 2) > (ka - 1))
	{
		if (mode == 1)
		{
			profiler.countOperation("Operations", m, 1);
		}
		else
		{
			profiler.countOperation("Operations_k", k, 1);
		}
		if (a[i]->val > a[2 * i + 1]->val)
		{
			if (mode == 1)
			{
				profiler.countOperation("Operations", m, 3);
			}
			else
			{
				profiler.countOperation("Operations_k", k, 3);
			}
			swap(a[i], a[2 * i + 1]);
			return;
		}
	}
	//a[i] are doi copii
	else
	{
		int i_child_min;
		if (mode == 1)
		{
			profiler.countOperation("Operations", m, 2);
		}
		else
		{
			profiler.countOperation("Operations_k", k, 2);
		}
		if (a[2 * i + 1]->val < a[2 * i + 2]->val)
		{
			i_child_min = 2 * i + 1;
		}
		else
		{
			i_child_min = 2 * i + 2;
		}
		if (a[i]->val > a[i_child_min]->val)
		{
			if (mode == 1)
			{
				profiler.countOperation("Operations", m, 3);
			}
			else
			{
				profiler.countOperation("Operations_k", k, 3);
			}
			swap(a[i], a[i_child_min]);
			Min_Heapify_BotomUp(a, i_child_min, ka, k, m, mode);
		}
	}
}

void Build_Min_Heap_BottomUp(node** a, int k, int m, int mode)
{
	for (int i = k / 2 - 1; i >= 0; i--)
	{
		Min_Heapify_BotomUp(a, i, k, k, m, mode);
	}
}

void Interclasare(int k, int m, int mode)
{
	node** heap;
	int n = k*m, heapsize = k;
	int** v = Table_Allocation(k, m, n);
	if (!v)
	{
		cout << "\nEroare la alocarea listelor!!";
		exit(1);
	}
	else
	{
		heap = Heap_Allocation(k);
	}

	//alocare vector final sortat
	int* sorted = (int*)calloc(n, sizeof(int));
	if (!sorted)
	{
		cout << "\nEroare la alocarea vectorului sorted!!";
		exit(2);
	}
	
	//initializare liste
	for (int i = 0; i < k; i++)
	{
		FillRandomArray(*(v + i), m, 0, 50, false, 1);
	}
	/*afisare
	cout << "\nListele dupa initializare: ";
	for (int i = 0; i < k; i++)
	{
		cout << "\nLista " << i << ": ";
		for (int j = 0; j < m; j++)
		{
			cout << v[i][j]<<' ';
		}
	}*/
	//initializare heap
	for (int i = 0; i < k; i++)
	{
		heap[i]->val = v[i][0];
		heap[i]->index = 0;
		heap[i]->list = i;
	}
	
	Build_Min_Heap_BottomUp(heap, k, m, mode);
	/*afisare
	cout << "\nHeapul dupa Build Heap: ";
	for (int i = 0; i < k; i++)
	{
		cout << heap[i]->val << ' ';
	}*/

	//interclasarea propriu zisa
	for (int j = 0; j < n; j++)
	{
		if (mode == 1)
		{
			profiler.countOperation("Operations", m, 2);
		}
		else
		{
			profiler.countOperation("Operations_k", k, 2);
		}
		sorted[j] = (heap[0]->val);
		if ((heap[0]->index) < (m - 1))
		{
			if (mode == 1)
			{
				profiler.countOperation("Operations", m, 2);
			}
			else
			{
				profiler.countOperation("Operations_k", k, 2);
			}
			(heap[0]->index)++;
			heap[0]->val = v[heap[0]->list][heap[0]->index];
		}
		else
		{
			if (mode == 1)
			{
				profiler.countOperation("Operations", m, 2);
			}
			else
			{
				profiler.countOperation("Operations_k", k, 2);
			}
			heap[0] = heap[--heapsize];
			heap[heapsize] = NULL;
		}

		Min_Heapify_BotomUp(heap, 0, heapsize, k, m, mode);
	}
	//List_Int_String(n, sorted, "Vectorul sortat: ");

	//dealocare
	Heap_Free(k, heap);
	Table_Free(k, v);
}

int main()
{
	
	int k = 5;
	for (int i = 100; i <= 10000; i += 100)
	{
		cout << '\n' << i;
		Interclasare(k, i, 1);
	}
	profiler.reset("Interclasare_10_liste");
	k = 10;
	for (int i = 100; i <= 10000; i += 100)
	{
		cout << '\n' << i;
		Interclasare(k, i, 1);
	}
	profiler.reset("Interclasare_100_liste");
	k = 100;
	for (int i = 100; i <= 10000; i += 100)
	{
		cout << '\n' << i;
		Interclasare(k, i, 1);
	}
	
	profiler.reset("Interclasare dupa numarul de liste");
	for (int i = 10; i <= 500; i += 10)
	{
		Interclasare(i, 10000/i, 2);
	}
	profiler.showReport();
	
}