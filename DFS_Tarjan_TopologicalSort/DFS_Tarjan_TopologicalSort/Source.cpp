/*
ADRIAN BISCHIN
Group 30228
DFS: complexitatea algoritmului DFS este O(|V| + |E|). Tarjan -> O(|V| + |E|).
Topological sort -> O(|V| + |E|) -> este un DFS cu o stiva in plus.
*/

#include <iostream>
#include <queue>
#include <stack>
#include <iterator>
#include <algorithm>
#include "Profiler.h"

using namespace std;

Profiler profiler("Edges variations");

struct Graph;

struct List
{
	Graph* graphNode;
	List* next;
};

struct Graph
{
	int key;
	int color;//0=white, 1=gray, 2=black
	List* adj;
	int discoveryTime;
	int finalTime;
	Graph* parent;
};

List* Insert_First(List* list, Graph* key)
{
	List* nn = (List*)calloc(1, sizeof(List));
	if (nn == NULL)
	{
		cerr << "\nEroare la alocarea unui nod in lista !!";
		exit(1);
	}
	nn->graphNode = key;
	nn->next = list;
	list = nn;
	return list;
}

void Free_List(List* list)
{
	while (list != NULL)
	{
		List* dn = list;
		list = list->next;
		free(dn);
	}
}

Graph* Generate_Graph(int size, int edges, bool print, bool read)
{
	Graph* graph = (Graph*)calloc(size, sizeof(Graph));
	if (graph == NULL)
	{
		cerr << "\nEroare la alocarea grafului !!";
		exit(1);
	}
	for (int i = 0; i < size; i++)
	{
		graph[i].key = i;
		graph[i].color = 0;
		graph[i].adj = NULL;
		graph[i].discoveryTime = -1;
		graph[i].finalTime = -1;
		graph[i].parent = NULL;
	}
	int** adjacent_matrix = (int**)calloc(size, sizeof(int*));
	if (adjacent_matrix == NULL)
	{
		cerr << "\nEroare la alocarea matricei de adiacenta !!";
		exit(2);
	}
	for (int i = 0; i < size; i++)
	{
		*(adjacent_matrix + i) = (int*)calloc(size, sizeof(int));
		if (*(adjacent_matrix + i) == NULL)
		{
			cerr << "\nEroare la alocarea matricei de adiacenta !!";
			exit(2);
		}
	}
	int count = 0, u, v;
	if (print)
	{
		cout << "\nMuchiile grafului sunt:";
	}
	while (count < edges)
	{
		if (read)
		{
			cout << "\nMuchia " << count + 1 << ":\nbaza=";
			cin >> u;
			cout << "varf=";
			cin >> v;
		}
		else
		{
			u = rand() % size;
			do
			{
				v = rand() % size;
			} while (v == u);
		}
		if (*(*(adjacent_matrix + u) + v) == 0)
		{
			count++;
			*(*(adjacent_matrix + u) + v) = 1;
			(graph + u)->adj = Insert_First((graph + u)->adj, graph + v);
			if (print)
			{
				cout << " (" << u << ", " << v << ")";
			}
		}
	}
	for (int i = 0; i < size; i++)
	{
		free(adjacent_matrix[i]);
	}
	free(adjacent_matrix);
	return graph;
}

void Free_Graph(Graph* graph, int size)
{
	for (int i = 0; i < size; i++)
	{
		Free_List((graph + i)->adj);
	}
	free(graph);
}

void See_Graph(Graph* graph, int size)
{
	cout << "\n\nNodurile grafului (key, finalTime):";
	for (int i = 0; i < size; i++)
	{
		cout << "\n(" << graph[i].key << ", " << graph[i].finalTime << ")";
	}
}

void Pretty_Print(Graph* graph, int size, Graph* root, int level)
{
	cout << '\n';
	for (int i = 1; i <= level; i++)
	{
		cout << "  ";
	}
	cout << root->key;
	for (int i = 0; i < size; i++)
	{
		if ((graph + i)->parent == root)
		{
			Pretty_Print(graph, size, graph + i, level + 1);
		}
	}
}

int myTime;

void DFS_Visit(Graph* graph, Graph* u, int functionParameter)
{
	profiler.countOperation("DFS Complexity", functionParameter, 6);//apel = push stiva -> operation, terminare apel = pop stiva -> operation
																	//=> 4(de mai jos) + 2 = 6
	myTime++;
	u->discoveryTime = myTime;//operation
	u->color = 1;//operation
	for (List* v = u->adj; v != NULL; v = v->next)
	{
		profiler.countOperation("DFS Complexity", functionParameter, 1);
		if (v->graphNode->color == 0)
		{
			profiler.countOperation("DFS Complexity", functionParameter, 1);
			v->graphNode->parent = u;
			DFS_Visit(graph, v->graphNode, functionParameter);
		}
	}
	u->color = 2;//operation
	myTime++;
	u->finalTime = myTime;//operation
}

void DFS(Graph* graph, int size, bool print, int functionParameter)
{
	myTime = 0;
	List* coverageTrees = NULL;
	for (int i = 0; i < size; i++)
	{
		if (graph[i].color == 0)
		{
			DFS_Visit(graph, graph + i, functionParameter);
			if (print)
			{
				coverageTrees = Insert_First(coverageTrees, graph + i);
			}
		}
	}
	if (print)
	{
		List* p = coverageTrees;
		while (coverageTrees)
		{
			List* dn = coverageTrees;
			Pretty_Print(graph, size, coverageTrees->graphNode, 0);
			coverageTrees = coverageTrees->next;
			free(dn);
		}
	}
}

int Partition(Graph* graph, int left, int right)
{
	int pivot_index = rand() % (right + 1 - left) + left;
	swap(graph[pivot_index], graph[right]);
	pivot_index = right;
	int border = left;	//border este indexul primului element din partea celor mai mari decat pivotul
	for (int i = left; i < right; i++)
	{
		if (graph[i].finalTime < graph[pivot_index].finalTime)
		{
			swap(graph[border], graph[i]);
			border++;
		}
	}
	swap(graph[border], graph[right]);	//dupa executarea acestui swap, border va fi indexul pivotului (asezat pe pozitia lui finala in sirul sortat)
	return border;
}

void QuickSort(Graph* graph, int left, int right)
{
	if (left < right)
	{
		int pivot_index = Partition(graph, left, right);
		QuickSort(graph, left, pivot_index - 1);
		QuickSort(graph, pivot_index + 1, right);
	}
	return;
}

void Topological_Sort(Graph* graph, int size, bool print, int functionParameter)
{
	QuickSort(graph, 0, size - 1);
}

int globalIndex = 0;

void Strongly_Connected(Graph* graph, stack <int>* lifo, int* index, int* lowlink, bool* onstack, int v)
{
	int w;
	index[v] = globalIndex;
	lowlink[v] = globalIndex;
	globalIndex++;
	lifo->push(v);
	onstack[v] = true;
	List* p = graph[v].adj;
	while (p)
	{
		w = p->graphNode->key;
		if (index[w] == -1)
		{
			Strongly_Connected(graph, lifo, index, lowlink, onstack, w);
			lowlink[v] = min(lowlink[v], lowlink[w]);
		}
		else
		{
			if (onstack[w])
			{
				lowlink[v] = min(lowlink[v], index[w]);
			}
		}
		p = p->next;
	}

	if (lowlink[v] == index[v])
	{
		cout << "\nComponenta tare conexa: ";
		do
		{
			w = lifo->top();
			lifo->pop();
			onstack[w] = false;
			cout << " " << w;
		} while (w != v);
	}
}

void Tarjan(Graph* graph, int size)
{
	int* index = (int*)calloc(size, sizeof(int));
	if (!index)
	{
		cerr << "\nEroare la alocarea vectorului index !!";
		exit(4);
	}
	int* lowlink = (int*)calloc(size, sizeof(int));
	if (!lowlink)
	{
		cerr << "\nEroare la alocarea vectorului lowlink !!";
		exit(5);
	}
	bool* onstack = (bool*)calloc(size, sizeof(bool));
	if (!onstack)
	{
		cerr << "\nEroare la alocarea vectorului onstack !!";
		exit(6);
	}
	stack <int> lifo;

	for (int i = 0; i < size; i++)
	{
		index[i] = -1;
		lowlink[i] = -1;
		onstack[i] = 0;
	}
	for (int i = 0; i < size; i++)
	{
		if (index[i] == -1)
		{
			Strongly_Connected(graph, &lifo, index, lowlink, onstack, i);
		}
	}
}

int main()
{
	/*DFS DEMO
	int size = 9, edges = 9;
	Graph* graph = Generate_Graph(size, edges, 1, 1);
	DFS(graph, size, 1, 0);
	//*/

	/*TOPOLOGICAL SORT DEMO
	int size = 8, edges = 6;
	Graph* graph = Generate_Graph(size, edges, true, false);
	DFS(graph, size, 1, 0);
	See_Graph(graph, size);
	Topological_Sort(graph, size, 1, 0);
	See_Graph(graph, size);
	//*/

	/*TARJAN ALGORITHM DEMO
	int size = 6, edges = 10;
	Graph* graph = Generate_Graph(size, edges, true, false);
	Tarjan(graph, size);
	//*/

	//*ANALISYS
	//EDGES VARIATION
	cout << "\nedges";
	int size = 100;
	for (int edges = 1000; edges <= 4500; edges += 100)
	{
		cout << "\n" << edges;
		Graph* graph = Generate_Graph(size, edges, false, false);
		DFS(graph, size, 0, edges);
	}

	//VERTEX VARIATION
	cout << "\nsize";
	profiler.reset("Vertex variations");
	int edges = 4500;
	for (int size = 100; size <= 200; size += 10)
	{
		cout << "\n" << size;
		Graph* graph = Generate_Graph(size, edges, false, false);
		DFS(graph, size, 0, size);
	}
	profiler.showReport();
	//*/

	return 0;
}