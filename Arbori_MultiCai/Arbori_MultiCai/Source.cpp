/*
ADRIAN BISCHIN
Group 30228
	Arbori multicai: 
	->R1: reprezentarea 1 este implementata cu un vector de node1 de dimensiunea numarului de noduri; fiecare element contine cheia nodului si indexul parintelui.
	->R2: reprezentarea 2 este implementata cu un vector de node2 de dimensiunea numarului de noduri; fiecare element contine cheia si o lista de copii
	->R3: reprezentarea 3 este implementata ca si arbore binar de noduri de tipul node3; fiecare element contine cheia, pointer la primul copil si pointer la primul frate
	==> Transformarea 1 este realizata in timp liniar, parcurgandu-se vectorul din reprezentarea 1 o data si efectuand pentru fiecare element urmatoarele operatii:
tree2[i].key = tree1[i].key, Add_First( tree2[tree1[i].parent], i ), unde add_first este de forma Add_First( destinatie, valoare )
	==> Transformarea 2 este realizata in timp liniar, parcurgandu-se vectorul din reprezentarea 2 o data si creand initial un vector de node3 pentru r3, cu specificarea
ca node3.firstChild si node3.firstBrother nu sunt indici din sir, ci pointer de tipul node3* la nodurile respective, astfel dupa crearea arborelui, putandu-se renunta la
sir, accesand arborele doar prin radacina. Pentru fiecare nod am efectuat urmatoarele operatii: tree3[i].key = tree2[i].key,
tree3[myNode].firstChild = &tree3[tree2[myNode].childs->key], tree3[tree2[myNode].childs->key].nextBrother = tree3+(tree2[myNode].childs->next->key).
	Deoarece la ambele treansformari am parcurs doar o data vectorul acestea sunt niste algoritmi liniari.
	Afisarile sunt facute recursiv, afisand intai cheia radacinii cu spatierea corespunzatoare nivelului in arbore, apoi apelandu-se recursiv pe totii copii, cu nivel+1.

*/

#include <iostream>
#include <list>
#include <iterator>

using namespace std;

struct node
{
	int key;
	node* next;
};

struct node1
{
	int key;
	int parentIndex;
};

struct node2
{
	int key;
	node* childs;
};

struct node3
{
	int key;
	node3* firstChild;
	node3* nextBrother;
};

node* Add_First(node* list, int key)
{
	node* newNode = (node*)calloc(1,sizeof(node));
	if (newNode == NULL)
	{
		cerr << "\nEroare la alocarea nodului nou !!";
		exit(0);
	}
	newNode->key = key;
	if (list == NULL)
	{
		list = newNode;
		newNode->next = NULL;
	}
	else
	{
		newNode->next = list;
		list = newNode;
	}
	return list;
}

node1* Build_Tree_0(int size)
{
	int ind=0;

	//daca nu se fac introduceri, se returneaza
	if (size < 1)
		return NULL;

	//se aloca sirul nodurilor
	node1* tree1 = (node1*)calloc(size, sizeof(node1));
	if (tree1 == NULL)
	{
		cerr << "\nNu s-a putut aloca vectorul arborelui.";
		exit(1);
	}

	//se introduc nodurile
	cout << "\nIntroduceti nodurile sub forma de pereche (cheie, indexParinte):";
et1:
	for (int i = 0; i < size; i++)
	{
		int parentIndex;
		tree1[i].key = i;
et2:
		cout << "\nPI[" << i << "].parentIndex = ";
		cin >> parentIndex;
		if (parentIndex >= -1 && parentIndex < size)
		{
			tree1[i].parentIndex = parentIndex;
			if (parentIndex == -1)
			{
				ind++;
			}
		}
		else
		{
			cout << "\nINDEXUL INTRODUS ESTE INVALID ! RESPECTATI INDICATIA !!";
			goto et2;
		}
			
	}
	if (ind == 0)
	{
		cout << "\nNU ATI INTRODUS RADACINA !! REINTRODUCETI TOATE NODURILE SUB FORMA DE PERECHE (cheie, indexParinte):";
		goto et1;
	}
	if (ind > 1)
	{
		cout << "\nATI INTRODUS MAI MULTE RADACINI !! REINTRODUCETI TOATE NODURILE SUB FORMA DE PERECHE (cheie, indexParinte):";
		goto et1;
	}
	return tree1;
}

node1* Build_Tree_1(int size)
{
	int index = 0;

	//daca nu se fac introduceri, se returneaza
	if (size < 1)
	return NULL;

	//se aloca sirul nodurilor
	node1* tree1 = (node1*)calloc(size, sizeof(node1));
	if (tree1 == NULL)
	{
		cerr << "\nNu s-a putut aloca vectorul arborelui.";
		exit(1);
	}

	//se introduce radacina
	cout << "\nIntroduceti cheia radacinii: ";
	cin >> tree1[0].key;
	tree1[0].parentIndex = -1;
	index++;

	//se introduc celelalte noduri
	cout << "\nRadacina a fost introdusa. Introduceti celelalte noduri sub forma de pereche (cheie, indexParinte):";
	for (int i=2; i <= size; i++)
	{
		cout << "\nIntroduceti datele pentru al " << index + 1 << "-lea nod:\n\tcheie: ";
		cin >> tree1[index].key;
	et1:
		cout << "\n\tindexParinte - din intervalul [0, " << index-1 <<"]: ";
		int parentIndex;
		cin >> parentIndex;
		if (parentIndex >= 0 && parentIndex < index)
		{
			tree1[index].parentIndex = parentIndex;
			index++;
		}
		else
		{
			cout << "\nINDEXUL INTRODUS ESTE INVALID ! RESPECTATI INDICATIA !!";
			goto et1;
		}
	}
	return tree1;
}

int Find_Root_Tree1(node1* tree1, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (tree1[i].parentIndex == -1)
			return i;
	}
	return -1;
}

void See_Tree1(node1 tree1[], int size, int nodeIndex, int level)
{
	cout << '\n';
	for (int i = 1; i <= level; i++)
	{
		cout << "  ";
	}
	cout << tree1[nodeIndex].key;
	for (int i = 0; i < size; i++)
	{
		if (tree1[i].parentIndex == nodeIndex)
		{
			See_Tree1(tree1, i, level + 1, size);
		}
	}
}

void See_Tree2(node2 tree2[], int nodeIndex, int level, int size)
{
	cout << '\n';
	for (int i = 1; i <= level; i++)
	{
		cout << "  ";
	}
	cout << tree2[nodeIndex].key;
	node* n = tree2[nodeIndex].childs;
	while(n != NULL)
	{
		See_Tree2(tree2, n->key, level + 1, size);
		n = n->next;
	}
}

void See_Tree3(node3* root, int level)
{
	cout << '\n';
	for (int i = 1; i <= level; i++)
	{
		cout << "  ";
	}
	cout << root->key;
	root = root->firstChild;
	while (root != NULL)
	{
		See_Tree3(root, level+1);
		root = root->nextBrother;
	}
}

node2* Transformation_1(node1* tree1, int size)
{
	node2* tree2 = (node2*)calloc(size, sizeof(node2));
	if (tree2 == NULL)
	{
		cerr << "\nNu s-a putut aloca vectorul arborelui.";
		exit(2);
	}
	for (int i = 0; i < size; i++)
	{
		tree2[i].key = tree1[i].key;
		tree2[tree1[i].parentIndex].childs = Add_First( tree2[tree1[i].parentIndex].childs, i );
	}
	return tree2;
}

void T_2(node2* tree2, node3* tree3, int myNode)
{
	tree3[myNode].key = tree2[myNode].key;
	node* n = tree2[myNode].childs;
	if (n == NULL)
	{
		tree3[myNode].firstChild = NULL;
	}
	else
	{
		tree3[myNode].firstChild = tree3 + (n->key);
	}
	while (n != NULL)
	{
		if (n->next == NULL)
		{
			tree3[n->key].nextBrother = NULL;
		}
		else
		{
			tree3[n->key].nextBrother = tree3+(n->next->key);
		}
		T_2(tree2, tree3, n->key);
		n = n->next;
	}
}

node3* Transformation_2(node2* tree2, int size, int root)
{
	node3* tree3 = (node3*)calloc(size,sizeof(node3));
	if (tree3 == NULL)
	{
		cerr << "\nNu s-a putut aloca vectorul arborelui.";
		exit(3);
	}
	tree3[root].nextBrother = NULL;
	T_2(tree2, tree3, root);
	return &tree3[root];
}

void See_R2(node2* tree2, int size)
{
	for (int i = 0; i < size; i++)
	{
		cout << "\n(" << tree2[i].key << "| ";
		node* n = tree2[i].childs;
		while (n)
		{
			cout << n->key << " ";
			n = n->next;
		}
		cout << ")";
	}
}

int main()
{
	int size, root;

	//se citeste numarul de noduri introduse
	cout << "\nCate noduri doriti sa aiba arborele? ";
	cin >> size;
	
	node1* tree1 = Build_Tree_0(size);
	root = Find_Root_Tree1(tree1, size);

	cout << "\n\nR1:\n";
	See_Tree1(tree1, root, 0, size);

	node2* tree2 = Transformation_1(tree1, size);
	cout << "\n\nR2:\n";
	See_Tree2(tree2, root, 0, size);

	node3* tree3 = Transformation_2(tree2, size, root);
	cout << "\n\nR3:\n";
	See_Tree3(tree3, 0);
}