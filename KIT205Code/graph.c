/*
* Vinh Nguyen #470821
*/
#include <stdio.h>
#include <stdlib.h>
#include "graph.h"


Graph create_graph(int vertices) {
    /* create a new graph, set size of vertexes in the graph */
    Graph G;
    G.V = vertices;

    /* create an array of EdgeLists the side of v */
    G.edges = malloc(G.V * sizeof * G.edges);

    /* set each array the starting value of NULL */
    for (int v = 0; v < G.V; v++) {
        G.edges[v].head = NULL;
    }
    return G;
}

/*Adds an edges to the AdgeList */
void add_edge(Graph G, int from_vertex, int to_vertex)
{
    /*create a new edgeList node and assign it's value */
    EdgeNodePtr node = malloc(sizeof * node);
    node->edge.to_vertex = to_vertex;
    node->next = G.edges[from_vertex].head;
    G.edges[from_vertex].head = node;
}

/* builds a small world network using the Watts&Strogatz method
 * n is the graph size
 * k is the number of neighbours to connect
 * beta is the probability of connecting to a random vertex 
 */
Graph watts_strogatz(int n, int k, float beta) {

    Graph G;
    G.V = n;
    G.edges = malloc(G.V * sizeof * G.edges);

    for (int i = 0; i < G.V; i++) {
        G.edges[i].head = NULL;
    }

    for (int u = 0; u < G.V; u++) {
        // add edges for neighbours
        for (int i = 1; i <= k / 2; i++) {
            int v = (u + i) % G.V;

            // add edges to random vertices with probability beta
            if ((float)rand() / RAND_MAX < beta) {
                v = (int)(((float)rand() / RAND_MAX) * G.V);
            }

            // add edges in both directions
            EdgeNodePtr uv;
            uv = malloc(sizeof * uv);
            uv->edge.to_vertex = v;
            uv->next = G.edges[u].head;
            G.edges[u].head = uv;

            EdgeNodePtr vu;
            vu = malloc(sizeof * vu);
            vu->edge.to_vertex = u;
            vu->next = G.edges[v].head;
            G.edges[v].head = vu;
        }
    }

    return G;
}

/* builds a scale free network using the Barabase&Albert method
 * n is the size of the graph
 * v0 is the size of the initial, fully connected graph 
 */
Graph barabasi_albert(int n, int v0) {
    /* create a graph*/
    Graph G = create_graph(n);

     /*Add v0 vertex to Graph and connect every vertex in the graph
       to every other node in graph*/
    int i, j;
    for (i = 0; i < v0 + 1; i++) {
        for (j = i + 1; j < v0 + 1; j++) {

            /* add edges in both directions */
            add_edge(G, i, j);
            add_edge(G, j, i);
        }
    }

    /* degree keeps track of the actual degree of each vertex. */
    int* degree = malloc(n * sizeof(*degree));
    int* d = malloc(n * sizeof(*d));
    
    /* set degree of each vertex to 0 */
    for (i = 0; i < n; i++) {
        degree[i] = 0;
    }

    /* set degree of all vertex to v0 */
    for (i = 0; i < v0 + 1; i++) {
        degree[i] = v0;
    }

    /* total adges ( the total number of degrees) in the graph. */
    int total_edges = v0 * (v0 + 1);

    for (i = v0 + 1; i < n; i++) {

        /* Initializes the probability list */
        for (int k = 0; k < i; k++) {
            d[k] = degree[k];
         }
               
        int m = total_edges;

        /* For each new vertex, do vO rounds to select its target vertices in probability list.*/
        for (j = 0; j < v0; j++) {
            /*Generates a random index for the probability list*/
            int s = rand() % (m + 1);
            int v;
            for (v = 0; v < i; v++) {
                s -= d[v];
                if (s < 0) break;
            }
            
            /* add edges in both directions */
                add_edge(G, i, v);
                add_edge(G, v, i);
          
              /*Once a vertex is selected then remove it out off list
                so it wont be chosen in the next selection round */
                m -= d[v];
                d[v] = 0;
                degree[v]++;
        }
        /* increasing the total adges*/
        total_edges += 2 * v0;

        degree[i] = v0;
    }

    free(d);
    free(degree);
    return G;
}

/*Gets the in degrees for each vertex*/
int* get_indegree(Graph G)
{
    /* used to traverse through the vertex's edge lists.*/
    EdgeNodePtr current;    
    int* indegree = malloc(G.V * sizeof(int));

    /* set degree of each vertex to 0 */
    for (int i = 0; i < G.V; i++) {
        indegree[i] = 0;
    }

    /*Trevesal through whole graph*/
    for (int i = 0; i < G.V; i++) {
        current = G.edges[i].head;

        /* go through all the vertex's edgeList
	     * to find the to_Vertex variable and use that to increment
	     * the indegree array at the to_Vertex position.
         */
        while (current != NULL) {
            indegree[current->edge.to_vertex]++;
            current = current->next;
        }
    }
    return indegree;
}


/*clean all memory use by Graph*/
void destroy_graph(Graph G) {

    for (int i = 0; i < G.V; i++) {
        EdgeNodePtr current = G.edges[i].head;

        while (current != NULL) {
            EdgeNodePtr to_free = current;
            current = current->next;
            free(to_free);
        }
    }
    free(G.edges);
}


