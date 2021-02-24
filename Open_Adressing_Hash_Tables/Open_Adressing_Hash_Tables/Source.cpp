/*
ADRIAN BISCHIN
Group 30228
Tabela de dispersie este de 10000 de elemente, cu factorii de umplere: 0.8, 0.85, 0.9, 0.95, 0.99. Pentru fiecare factor de
umplere am afisat numarul maxim si numarul mediu de accesari ale tabelei de catre functia de cautare a unei chei. Se observa ca
numarul accesarilor creste o data cu factorul de umplere. Totodata observam ca pentru factorul de umplere 0.8 avem rezultate
bune, astfel ca in majoritatea situatiilor nu este nevoie sa folosim un factor mai mic, economisind astfel memorie. Daca urcam
factorul de umplere peste 0.85 situatia se inrautateste din punct de vedere al timpului de executare, insa in situatiile in care
economia memoriei este critica poate ne va fi convenabil sa utilizam factori de peste 0.85 - 0.9.
*/

#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler profiler(" ");


void List_Int_String(int size, int array[], string s)
{
	cout << '\n' << s;
	for (int i = 0; i < size; i++)
	{
		cout << array[i] << ' ';
	}
}

void Initialize_Table(int size, int table[])
{
	for (int i = 0; i < size; i++)
	{
		table[i] = -1;
	}
}

int Hash_Function(int key, int hashSize, int ithTry)
{
	return ((key % hashSize) + ithTry + 2 * ithTry * ithTry) % hashSize;
}

int Find_Key(int key, int hashSize, int table[], int* tryCounter)
{
	for (int i = 0; i < hashSize; i++)
	{
		int index = Hash_Function(key, hashSize, i);
		(*tryCounter)++;
		if (table[index] == key)
		{
			return index;
		}
		else
		{
			if (table[index] == -1)
				return -1;
		}
	}
	return -1;
}

void Insert_Key(int key, int hashSize, int table[])
{
	int i;
	for (i = 0; i < hashSize; i++)
	{
		int index = Hash_Function(key, hashSize, i);
		if (table[index] == -1)
		{
			table[index] = key;
			break;
		}
	}
	if (i == hashSize)
	{
		cout << "\nYOUR HASH TABLE IS FULL !!";
	}
}

void Create_Hash_Table(int numberOfElements, int values[], int hashSize, int table[])
{
	for (int i = 0; i < numberOfElements; i++)
	{
		Insert_Key(values[i], hashSize, table);
	}
}

void Analysis(int hashSize, int hashTable[], int fillFactor, int cauta, bool detailedPrints, bool finalPrints)
{
	//cauta este numarul valorilor cautate si gasite/negaite (gasite=negasite), deci jumatate din numarul valorilor cautate
	int *values, *searched, *indexes, counter, maxSearchesFound = 0, maxSearchesNotFound = 0;
	double meanSearchesFound = 0, meanSearchesNotFound = 0;

	//se initializeaza valorile tabelei cu -1
	Initialize_Table(hashSize, hashTable);
	//se afiseaza tabela initializata
	if (detailedPrints == true)
	{
		List_Int_String(hashSize, hashTable, "Tabela initializata: ");

	}

	//se calculeaza cate numere trebuie introduse pentru a umple fillFactor% din tabela
	int numberOfElements = (int)((fillFactor / 100.0) * hashSize);
	//se afiseaza numarul calculat
	if (detailedPrints == true)
	{
		cout << '\n' << "Numarul de elemente introduse: " << numberOfElements;

	}
	
	//se aloca vectorul valorilor care se vor introduce in tabela
	values = (int*)calloc(numberOfElements, sizeof(int));

	//se genereaza numerele care vor fi inserate
	FillRandomArray(values, numberOfElements, 10, 1000000, true, 0);
	//se afiseaza numerele care vor fi inserate
	if (detailedPrints == true)
	{
		List_Int_String(numberOfElements, values, "Valorile de introdus: ");

	}

	//se insereaza numerele generate
	Create_Hash_Table(numberOfElements, values, hashSize, hashTable);
	//se afiseaza tabela dupa insertie
	if (detailedPrints == true)
	{
		List_Int_String(hashSize, hashTable, "Tabela creata: ");

	}

	//se aloca vectorul indecsilor vectorului values (values - vectorul valorilor introduse in tabela) de la care se vor lua valorile
	//cu care se va forma vectorul valorilor cautate in tabela
	indexes = (int*)calloc(cauta, sizeof(int));

	//se genereaza pozitiile vectorului values de la care se vor lua valorile cu care se va completa vectorul valorilor cautate
	FillRandomArray(indexes, cauta, 0, numberOfElements-1, true, 0);

	//se aloca vectorul valorilor care se vor cauta in tabela
	searched = (int*)calloc(2 * cauta, sizeof(int));

	//se creaza vectorul cu elementele cautate: prima jumatate va contine elemente care vor fi gasite in tabela (luate random din vectorul valorilor
	//inserate - 6 linii mai sus),
	//iar a doua jumatate va contine elemente care nu vor fi gasite in tabela (elemente >50, tabela continand doar elemente <=50)
	for (int i = 0; i < cauta; i++)
	{
		searched[i] = values[indexes[i]];
	}
	FillRandomArray(searched + cauta, cauta, 1000001, 2000000, true, 0);
	//se afiseaza vectorul valorilor cautate
	if (detailedPrints == true)
	{
		List_Int_String(2 * cauta, searched, "Numerele cautate: ");
	}

	//se cauta valorile si se calculeaza nr. max si nr. mediu de cautari - pentru valorile gasite
	for (int i = 0; i < cauta; i++)
	{
		counter = 0;
		Find_Key(searched[i], hashSize, hashTable, &counter);
		if (counter > maxSearchesFound)
		{
			maxSearchesFound = counter;
		}
		meanSearchesFound += counter;
	}
	meanSearchesFound /= cauta;
	//se cauta valorile si se calculeaza nr. max si nr. mediu de cautari - pentru valorile negasite
	for (int i = cauta; i < 2 * cauta; i++)
	{
		counter = 0;
		Find_Key(searched[i], hashSize, hashTable, &counter);
		if (counter > maxSearchesNotFound)
		{
			maxSearchesNotFound = counter;
		}
		meanSearchesNotFound += counter;
	}
	meanSearchesNotFound /= cauta;

	//se dealoca vectorul indecsilor
	free(indexes);

	//se dealoca vectorul valorilor introduse in tabela
	free(values);

	//se dealoca vectorul valorilor cautate in tabela
	free(searched);

	//se afiseaza factorul de umplere si valorile calculate pentru fiecare caz in parte
	if (finalPrints == true)
	{
		cout << "\n\nFACTOR DE UMPLERE: " << fillFactor << "%  => " << numberOfElements << " valori din totalul de " << hashSize << " locatii ale tabelei.";

		cout << "\n\tPentru numerele gasite: ";
		cout << "\n\t\tNumarul maxim de cautari: " << maxSearchesFound;
		cout << "\n\t\tNumarul mediu de cautari: " << meanSearchesFound;

		cout << "\n\tPentru numerele negasite: ";
		cout << "\n\t\tNumarul maxim de cautari: " << maxSearchesNotFound;
		cout << "\n\t\tNumarul mediu de cautari: " << meanSearchesNotFound;
	}
}

int main()
{
	//.
	int hashSize=10007, *hashTable;
	hashTable = (int*)calloc(hashSize,sizeof(int));
	
	Analysis(hashSize, hashTable, 80, 1500, false, true);
	Analysis(hashSize, hashTable, 85, 1500, false, true);
	Analysis(hashSize, hashTable, 90, 1500, false, true);
	Analysis(hashSize, hashTable, 95, 1500, false, true);
	Analysis(hashSize, hashTable, 99, 1500, false, true);
	//*/


	/*TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH    TEST BENCH
	int hashSize=13, *hashTable;
	hashTable = (int*)calloc(hashSize,sizeof(int));
	
	Analysis(hashSize, hashTable, 80, 5, true, true);
	//*/
}