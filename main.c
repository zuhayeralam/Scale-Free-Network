/*
* Watts-Strogatz and Banarasi-Albert Algorithm
* Zuhayer Alam
* 15 May 2020
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

	//G = watts_strogatz (10000, 10, 0.1);

	// You need to switch to the Barabasi&Albert model once implemented
	G = barabasi_albert(10000, 3);

	// Infection state  0 = susceptible, 1 = infected, 2 = recovered
	int* state = malloc(G.V * sizeof *state);

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

	return 0;
}


void vaccinate(Graph G, int* state, int num) {
	
	int degree_count = 0;
	int* degrees = malloc(G.V * sizeof *degrees);
	int* vertex_indices = malloc(G.V * sizeof *vertex_indices);
	int temp = 0; //variable for storing temporary value

	for (int i = 0; i < G.V; i++) {

		EdgeNodePtr current = G.edges[i].head;
		while (current != NULL)	{
			degree_count++;
			current = current->next;
		}
		degrees[i] = degree_count;
		vertex_indices[i] = i;
		degree_count = 0;
	}

	//Loop for sorting the degrees arrray in a descending order
	for (int i = 0; i < G.V; i++) {
		for (int j = 0; j < G.V; j++) {
			if (degrees[j] < degrees[i]) {
				
			    temp = degrees[i];         //Using temporary variable for storing last value
				degrees[i] = degrees[j];   //replacing value
				degrees[j] = temp;         //storing last value
				
				//syncing the vertex_indices array with the degrees array
				temp = vertex_indices[i];
				vertex_indices[i] = vertex_indices[j];
				vertex_indices[j] = temp;
			}
		}
	}

	//vaccinating the first 200 (or value of num) vertices with most degrees or connections
	for (int i = 0; i < num; i++) {
		temp = vertex_indices[i];
		state[temp] = 2;
	}
}


void step(Graph G, int* state, float inf_rate, float rec_rate) {
	
	for (int v = 0; v < G.V; v++) {
		//Look for an infected neighbour for every vertex
		EdgeNodePtr current = G.edges[v].head;
		while (current != NULL)	{
			if (state[v] == 0) { //only susceptible individuals can be infected
				
				//if a neighbour is infected, infect the current vertex based on infection rate
				if (state[current->edge.to_vertex] == 1) {
					if ((float)rand() / RAND_MAX < inf_rate) {
						state[v] = 1;
						current = NULL; //no need to look for infected neighbours anymore if vertex is already infected
					}
				}
				if (state[v] != 1 && current != NULL) {
					current = current->next;
				}
			}
			else if (state[v] != 0) {
				current = NULL; //if the vertex is not susceptible, stop processing this vertex
			}
		}

		//if the individual is infected they might recover based on recovery rate
		if (state[v] == 1) { 
			if ((float)rand() / RAND_MAX < rec_rate) {
				state[v] = 2;
			}
		}
	}
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
	printf("%i", p);// code added to see number of infections
	printf("\n");
}