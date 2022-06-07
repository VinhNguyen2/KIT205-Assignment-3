#pragma once
/*
* Vinh Nguyen #470821
*/
typedef struct edge {
	int to_vertex;
	int weight;  // NOTE: weights are not used in this assignment
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode *next;
} *EdgeNodePtr;

typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

typedef struct graph {
	int V;
	EdgeList *edges;
} Graph;

Graph create_graph(int vertices);

void add_edge(Graph G, int from_vertex, int to_vertex);


// builds a small world network using the Watts&Strogatz method
// n is the graph size
// k is the number of neighbours to connect
// beta is the probability of connecting to a random vertex
Graph watts_strogatz(int n, int k, float beta);

// builds a scale free network using the Barabase&Albert method
// n is the size of the graph
// v0 is the size of the initial, fully connected graph
Graph barabasi_albert(int n, int v0);

/*Gets the in degrees for each vertex*/
int* get_indegree(Graph self);

/*clean all memory use by Graph*/
void destroy_graph(Graph G);
