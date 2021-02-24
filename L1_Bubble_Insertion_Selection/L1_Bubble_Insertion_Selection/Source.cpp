/*
-> ADRIAN BISCHIN
-> #30228
-> Specificațiile problemei: Comparați metodele de sortare: BUBBLE_SORT, INSERTION_SORT, SELECTION_SORT
->Interpretare personală:
	Bubble O(n^2): metoda constă în parcurgerea vectorului pană când va fi sortat (de maxim n-1 ori, n fiind dimensiunea acestuia), la fiecare
parcurgere interschimbând elemente vecine dacă a[i]>a[i+1]; astfel, după fiecare parcurgere maximul din partea nesortată va ajunge pe
poziția lui finală, partea sortată crescând cu o unitate.
	Insertion O(n^2): vectorul este împărțit în două părți: prima este cea sortată iar a doua este partea nesortată, inițial partea sortată fiind
compusă din primul element. La fiecare pas (în total vor fi n-1 pași, n=dim. vect.) se alege primul element din partea nesortată și se
inserează (de unde provine și numele algoritmului) la locul potrivit în partea sortată, dimensiunea părții sortate crescând cu 1, iar cea
a părții nesortate scăzând cu 1.
	Selection O(n^2): asemenea lui insertion sort, vectorul este împărțit în două părți: prima este cea sortată iar a doua este partea nesortată,
inițial partea sortată fiind compusă din primul element; așadar cei doi algoritmi sunt asemănători/complementari. La fiecare pas (în total
vor fi n-1 pași, n=dim. vect.) se caută minimul din partea nesortată și se interchimbă cu primul element din partea nesortată (această
poziție reprezintă noua ultimă poziție a părții sortate), adică se mărește partea sortată și scade partea nesortată.
	Insertion și Selection sunt foarte asemănători ca și concept dar mai ales în practică. La Insertion efortul este depus în găsirea
locului potrivit al elementului ales, pe când la Selection efortul va fi depus în selectarea minimului din partea nesortată; în cazul mediu
statistic Insertion va depune un efort total similar cu selection, deci se va comporta asemănător. Bubble sort este total diferit: va face
tot n-1 pași (cel mult), însă la fiecare pas va face n-nrPasCurent comparații și între 0 și nrPasCurent atribuiri. De aici reiese ca este
mai ineficient decât ceilalți 2 algoritmi.
	Personal, eu aș folosi Insertion Sort. Din tabelul de analiză al cazului mediu statistic reiese de asemenea ca insertion ar fi cel mai
eficient.
*/

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "Profiler.h"
#define MAX_DIM 1000
#define INCREMENT 100

using namespace std;

//profiler declaration
Profiler profiler("Best_case");


//MY FUNCTIONS================================================================================================================================
void Clone_Vector(int n, int a[], int b[])
{
	for (int i = 0; i < n; i++)
	{
		a[i] = b[i];
	}
}

void list_int_string(int n, int a[])
{
	cout << "\n";
	for (int i = 0; i < n; i++)
	{
		cout << a[i] << ' ';
	}
}

void Bubble_Sort(int n, int v[])
{
	int ind = 1, count = 1;
	while (ind)
	{
		ind = 0;
		for (int i = 0; i < n - count; i++)
		{
			profiler.countOperation("Bubble_Comparations", n, 1);
			if (v[i] > v[i + 1])
			{
				profiler.countOperation("Bubble_Assignations", n, 3);
				swap(v[i], v[i + 1]);
				ind = 1;
			}
		}
	}
}

void Insertion_Sort(int n, int v[])
{
	for (int i = 1; i < n; i++)
	{
		int key = v[i];
		profiler.countOperation("Insertion_Assignations", n, 1);
		int j = i - 1;
		profiler.countOperation("Insertion_Comparations", n, 1);
		while ((j > 0)& (v[j] > key))
		{
			profiler.countOperation("Insertion_Assignations", n, 1);
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = key;
		profiler.countOperation("Insertion_Assignations", n, 1);
	}
}

void Selection_Sort(int n, int v[])
{
	for (int j = 0; j < n - 1; j++)
	{
		int imin = j;
		for (int i = j + 1; i < n; i++)
		{
			profiler.countOperation("Selection_Comparations", n, 1);
			if (v[i] < v[imin])
			{
				imin = i;
			}
		}
		if (v[imin] < v[j])
		{
			swap(v[j], v[imin]);
			profiler.countOperation("Selection_Assignations", n, 3);
		}
	}
}

//============================================================================================================================================
int main()
{
	/*TEST_BENCH_FOR_SORTING_ALGORITHMS
	int v[] = { 3,6,1,4,8,25,21,18,17,16,19,8,4 }, n = 13;

	list_int_string(n, v);
	Selection_Sort(n, v);
	list_int_string(n, v);
	*/

	//BEST====================================================================================================================================
	cout << "\n\n\n\n\n" << "---------------Calculating_BEST---------------";
	int v_best[MAX_DIM], v_best_clone[MAX_DIM];
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		cout << '\n' << "dim=" << dim;
		for (int i = 0; i < dim; i++)
		{
			v_best[i] = i;
		}
		//Bubble
		Clone_Vector(dim, v_best_clone, v_best);
		Bubble_Sort(dim, v_best);
		//Insertion
		Clone_Vector(dim, v_best_clone, v_best);
		Insertion_Sort(dim, v_best);
		//Selection
		Clone_Vector(dim, v_best_clone, v_best);
		Selection_Sort(dim, v_best);
	}
	profiler.addSeries("Bubble_Total", "Bubble_Comparations", "Bubble_Assignations");
	profiler.addSeries("Insertion_Total", "Insertion_Comparations", "Insertion_Assignations");
	profiler.addSeries("Selection_Total", "Selection_Comparations", "Selection_Assignations");

	profiler.createGroup("Best_Total_Comparations", "Bubble_Comparations", "Insertion_Comparations", "Selection_Comparations");
	profiler.createGroup("Best_Total_Assignations", "Bubble_Assignations", "Insertion_Assignations", "Selection_Assignations");
	profiler.createGroup("BEST_TOTAL", "Bubble_Total", "Insertion_Total", "Selection_Total");

	//AVERAGE=================================================================================================================================
	cout << "\n\n\n\n\n" << "---------------Calculating_AVERAGE---------------";
	profiler.reset("Average_case");
	int v_average[MAX_DIM], v_average_clone[MAX_DIM];

	//#1
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		cout << '\n' << "dim=" << dim;
		FillRandomArray(v_average, dim);
		//Bubble
		Clone_Vector(dim, v_average_clone, v_average);
		Bubble_Sort(dim, v_average_clone);
		//Insertion
		Clone_Vector(dim, v_average_clone, v_average);
		Insertion_Sort(dim, v_average_clone);
		//Selection
		Clone_Vector(dim, v_average_clone, v_average);
		Selection_Sort(dim, v_average_clone);
	}

	//#2
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		FillRandomArray(v_average, dim);
		//Bubble
		Clone_Vector(dim, v_average_clone, v_average);
		Bubble_Sort(dim, v_average_clone);
		//Insertion
		Clone_Vector(dim, v_average_clone, v_average);
		Insertion_Sort(dim, v_average_clone);
		//Selection
		Clone_Vector(dim, v_average_clone, v_average);
		Selection_Sort(dim, v_average_clone);
	}

	//#3
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		FillRandomArray(v_average, dim);
		//Bubble
		Clone_Vector(dim, v_average_clone, v_average);
		Bubble_Sort(dim, v_average_clone);
		//Insertion
		Clone_Vector(dim, v_average_clone, v_average);
		Insertion_Sort(dim, v_average_clone);
		//Selection
		Clone_Vector(dim, v_average_clone, v_average);
		Selection_Sort(dim, v_average_clone);
	}

	//#4
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		FillRandomArray(v_average, dim);
		//Bubble
		Clone_Vector(dim, v_average_clone, v_average);
		Bubble_Sort(dim, v_average_clone);
		//Insertion
		Clone_Vector(dim, v_average_clone, v_average);
		Insertion_Sort(dim, v_average_clone);
		//Selection
		Clone_Vector(dim, v_average_clone, v_average);
		Selection_Sort(dim, v_average_clone);
	}

	//#5
	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		FillRandomArray(v_average, dim);
		//Bubble
		Clone_Vector(dim, v_average_clone, v_average);
		Bubble_Sort(dim, v_average_clone);
		//Insertion
		Clone_Vector(dim, v_average_clone, v_average);
		Insertion_Sort(dim, v_average_clone);
		//Selection
		Clone_Vector(dim, v_average_clone, v_average);
		Selection_Sort(dim, v_average_clone);
	}
	profiler.addSeries("Bubble_Total", "Bubble_Comparations", "Bubble_Assignations");
	profiler.addSeries("Insertion_Total", "Insertion_Comparations", "Insertion_Assignations");
	profiler.addSeries("Selection_Total", "Selection_Comparations", "Selection_Assignations");

	profiler.createGroup("Average_Total_Comparations", "Bubble_Comparations", "Insertion_Comparations", "Selection_Comparations");
	profiler.createGroup("Average_Total_Assignations", "Bubble_Assignations", "Insertion_Assignations", "Selection_Assignations");
	profiler.createGroup("AVERAGE_TOTAL", "Bubble_Total", "Insertion_Total", "Selection_Total");

	//WORST===================================================================================================================================
	cout << "\n\n\n\n\n" << "---------------Calculating_WORST---------------";
	profiler.reset("Worst_case");
	int v_worst[MAX_DIM];

	for (int dim = 100; dim <= MAX_DIM; dim += INCREMENT)
	{
		cout << '\n' << "dim=" << dim;
		//Bubble
		for (int i = 0; i < dim; i++)
		{
			v_worst[i] = dim - i;
		}
		Bubble_Sort(dim, v_worst);
		//Insertion
		for (int i = 0; i < dim; i++)
		{
			v_worst[i] = dim - i;
		}
		Insertion_Sort(dim, v_worst);
		//Selection
		for (int i = 0; i < dim; i++)
		{
			v_worst[i] = (i + 1) % (dim);
		}
		Selection_Sort(dim, v_worst);
	}
	profiler.addSeries("Bubble_Total", "Bubble_Comparations", "Bubble_Assignations");
	profiler.addSeries("Insertion_Total", "Insertion_Comparations", "Insertion_Assignations");
	profiler.addSeries("Selection_Total", "Selection_Comparations", "Selection_Assignations");

	profiler.createGroup("Worst_Total_Comparations", "Bubble_Comparations", "Insertion_Comparations", "Selection_Comparations");
	profiler.createGroup("Worst_Total_Assignations", "Bubble_Assignations", "Insertion_Assignations", "Selection_Assignations");
	profiler.createGroup("WORST_TOTAL", "Bubble_Total", "Insertion_Total", "Selection_Total");

	profiler.showReport();

	return 0;
}