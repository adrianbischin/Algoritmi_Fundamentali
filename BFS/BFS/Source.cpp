/*
ADRIAN BISCHIN
Group 30228
BFS: complexitatea algoritmului este O(|V| + |E|)
*/

#include <iostream>
#include <queue>
#include <iterator>
#include "Profiler.h"

using namespace std;

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
	Graph* parent;
	int sourceDistance;
	List* adj;
};

Profiler profiler("Edges variations");

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

void BFS(Graph* graph, int source, int graphSize, int functionParameter)
{
	profiler.countOperation("BFS Complexity", functionParameter, 4);
	graph[source].color = 1;
	graph[source].sourceDistance = 0;
	graph[source].parent = NULL;
	queue <Graph*> fifo;
	fifo.push(graph + source);
	while (!fifo.empty())
	{
		profiler.countOperation("BFS Complexity", functionParameter, 3);
		Graph* u = fifo.front();
		fifo.pop();
		for (List* v = u->adj; v != NULL; v = v -> next)
		{
			profiler.countOperation("BFS Complexity", functionParameter, 1);
			if (v->graphNode->color == 0)
			{
				profiler.countOperation("BFS Complexity", functionParameter, 4);
				v->graphNode->color = 1;
				v->graphNode->sourceDistance = u->sourceDistance + 1;
				v->graphNode->parent = u;
				fifo.push(v->graphNode);
			}
		}
		u->color = 2;
	}
}

void Pretty_Print(Graph* graph, int size, Graph* root)
{
	cout << '\n';
	for (int i = 1; i <= root->sourceDistance; i++)
	{
		cout << "  ";
	}
	cout << root->key;
	for (int i = 0; i < size; i++)
	{
		if ((graph + i)->parent == root)
		{
			Pretty_Print(graph, size, graph + i);
		}
	}
}

Graph* Generate_Graph(int size, int edges, bool print, bool read)
{
	Graph* graph = (Graph*)calloc(size, sizeof(Graph));
	if (graph == NULL)
	{
		cerr << "\nEroare la alocarea grafului !!";
		exit(3);
	}
	for (int i = 0; i < size; i++)
	{
		graph[i].key = i;
		graph[i].color = 0;
		graph[i].parent = NULL;
		graph[i].sourceDistance = -1;
		graph[i].adj = NULL;
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
	int count=0, u, v;
	if (print)
	{
		cout << "\nMuchiile grafului sunt:";
	}
	while (count < edges)
	{
		if (read)
		{
			cout << "\nMuchia " << count + 1 << ": (";
			cin >> u;
			cout << ", ";
			cin >> v;
			cout << ")";
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
			*(*(adjacent_matrix + v) + u) = 1;
			(graph + u)->adj = Insert_First((graph + u)->adj, graph + v);
			(graph + v)->adj = Insert_First((graph + v)->adj, graph + u);
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

int main()
{
	/*DEMO
	int size = 9, edges = 9;
	List* coverageTrees = NULL;
	Graph* graph = Generate_Graph(size, edges, 1, 0);
	for (int i = 0; i < size; i++)
	{
		if (graph[i].color == 0)
		{
			BFS(graph, i, size, 0);
			coverageTrees = Insert_First(coverageTrees, graph + i);
		}
	}
	while (coverageTrees)
	{
		Pretty_Print(graph, size, coverageTrees->graphNode);
		List* dn = coverageTrees;
		coverageTrees = coverageTrees->next;
		free(dn);
	}
	//*/

	//*EDGES VARIATION
	cout << "\nedges";
	int size = 100;
	for (int edges = 1000; edges <= 4500; edges += 100)
	{
		cout << "\n" << edges;
		List* coverageTrees = NULL;
		Graph* graph = Generate_Graph(size, edges, 0, 0);
		for (int i = 0; i < size; i++)
		{
			if (graph[i].color == 0)
			{
				BFS(graph, i, size, edges);
				coverageTrees = Insert_First(coverageTrees, graph + i);
			}
		}
		Free_List(coverageTrees);
	}
	//*/

	//*VERTEXES VARIATION
	cout << "\nsize";
	profiler.reset("Vertex variations");
	int edges = 4500;
	for (int size = 100; size <= 200; size += 10)
	{
		cout << "\n" << size;
		List* coverageTrees = NULL;
		Graph* graph = Generate_Graph(size, edges, 0, 0);
		for (int i = 0; i < size; i++)
		{
			if (graph[i].color == 0)
			{
				BFS(graph, i, size, size);
				coverageTrees = Insert_First(coverageTrees, graph + i);
			}
		}
		Free_List(coverageTrees);
	}
	//*/
	profiler.showReport();

	return 0;
}