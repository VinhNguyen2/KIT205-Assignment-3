/*
* Vinh Nguyen #470821
*/

#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

// update one step of the simulation
// G is the graph
// state is current state of individuals
// inf_rate is the infection rate
// rec_rate is the recovery rate
void step(Graph G, int* state, float inf_rate, float rec_rate);

// plot % infected (each '.' is 1%)
void plot(Graph G, int* state);

// vaccinate num individuals
void vaccinate(Graph G, int* state, int num);

int main() {
    // Build Social Network
    Graph G;

    //	G = watts_strogatz (10000, 10, 0.1);

    // You need to switch to the Barabasi&Albert model once implemented
    	 G = barabasi_albert(10000, 3);

    // Infection state  0 = susceptible, 1 = infected, 2 = recovered
    int* state = malloc(G.V * sizeof * state);

    /*******************************************************************/
    /*** Baseline ******************************************************/
    /*******************************************************************/

    // Initialise population
    for (int i = 0; i < G.V; i++) {
        state[i] = 0;
    }

    // Infect one individual
    state[0] = 1;

    // Run simulation for 50 time steps

    printf("No Vaccination\n");
    printf("     t|--------10--------20--------30--------40--------50--------60\n");
    for (int i = 0; i < 50; i++) {
        printf("B %4d|", i);
        plot(G, state);
        step(G, state, 0.2, 0.05);

    }
    printf("\n");

    /*******************************************************************/
    /*** Intervention **************************************************/
    /*******************************************************************/

    // Reinitialise
    for (int i = 0; i < G.V; i++) {
        state[i] = 0;
    }

    // Vaccinate 200 individuals
    vaccinate(G, state, 200);

    // Infect same individual
    state[0] = 1;
  
    // Run simulation for 50 time steps

    printf("With Vaccination\n");
    printf("     t|--------10--------20--------30--------40--------50--------60\n");
    for (int i = 0; i < 50; i++) {
        printf("V %4d|", i);
        plot(G, state);
        step(G, state, 0.2, 0.05);
    }
    printf("\n");
   
    /* the program is finished destroy a graph and free a state. */
    free(state);
    destroy_graph(G);
    return 0;
}


void vaccinate(Graph G, int* state, int num) {

    int* indegrees;

    /* Get the indegrees of the vertexs in the graph. */
    indegrees = get_indegree(G);

    for (int i = 0; i < num; i++) {

        int k = 0; // the variable is store index of highest indegrees
        int largest = indegrees[k];
        int j;
        for (j = 1; j < G.V; j++)
        {
            if (largest < indegrees[j]) {
                largest = indegrees[j];
                k = j;
            }
        }
        /* assig highest degree to 0 to make sure wont chose it again */
        indegrees[k] = 0;

        /*change state of vertex have highest indegree (Vaccinatethe important individals)*/
        state[k] = 2;
    }
    free(indegrees);
}


void step(Graph G, int* state, float inf_rate, float rec_rate) {
   
    /*create an array to store update state of node*/
    int* state_copy = malloc(G.V * sizeof(int));

    for (int vertex = 0; vertex < G.V; vertex++)
    {
        state_copy[vertex] = 0;
    }
    for (int vertex = 0; vertex < G.V; vertex++) {

        EdgeNodePtr current = G.edges[vertex].head;
        if (state[vertex] == 1 && current != NULL) {

            while (current != NULL) {

                if (state[current->edge.to_vertex] == 0 && (float)rand() / RAND_MAX < inf_rate) {

                    /* update state of susceptible neighbour if affected into array copy.*/
                    state_copy[current->edge.to_vertex] = 1;
                }
                current = current->next;
            }

            /*update state of individual effected to recovered */
            if ((float)rand() / RAND_MAX < rec_rate) {
                state[vertex] = 2;
            }
            free(current);
        }
    }
    for (int vertex = 0; vertex < G.V; vertex++) {
        if (state_copy[vertex] != 0) {
            state[vertex] = state_copy[vertex];
        }
    }
    free(state_copy);
}

void plot(Graph G, int* state) {
    int c = 0;

    for (int i = 0; i < G.V; i++) {
        if (state[i] == 1)
            c++;
    }

    int p = (int)((float)c / G.V * 100.0);

    for (int i = 0; i < p; i++) {
        printf(".");
    }
    printf("\n");
}
