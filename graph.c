#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

Graph watts_strogatz(int n, int k, float beta) {
	Graph G;
	G.V = n;
	G.edges = malloc(G.V * sizeof *G.edges);

	for (int i = 0; i < G.V; i++) {
		G.edges[i].head = NULL;
	}

	for (int u = 0; u < G.V; u++) {
		// add edges for neighbours
		for (int i = 1; i <= k / 2; i++) {
			int v = (u+i) % G.V;

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

void push(int* arr, int index, int value, int* size) {
	arr[index] = value;
	*size = *size + 1;
}

Graph barabasi_albert(int n, int v0) {
	Graph G;
	G.V = n;
	G.edges = malloc(G.V * sizeof * G.edges);
	int barabasi_array_size = n * v0 * 2;
	int* barabasi_array = malloc(barabasi_array_size * sizeof *barabasi_array);
	int current_arr_size = 0; //keeps track of how many of the array indices have been filled
	int random_index = 0; //random index to be chosen from barabasi_array
	int random_vertex = 0; //random vertex in the random_index of barabasi_array

	for (int i = 0; i < G.V; i++) {
		G.edges[i].head = NULL;
	}
	// Connecting the first v0 vertices with one another
	for (int u = 0; u < v0; u++) {
		for (int v = 0; v < v0; v++) {
			if (v != u) { //avoid connecting to the vertex u itself
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
				
				//storing vertex u in barabasi_array as many times as it connects to another vertex v
				push(barabasi_array, current_arr_size, u, &current_arr_size);
			}
		}
	}

	for (int u = v0; u < G.V; u++) {
		for (int i = 0; i < v0; i++) {
			//picking random vertex from available indices in barabasi_array
			random_index = rand() % current_arr_size;
			random_vertex = barabasi_array[random_index];

			//if the random index is the source vertex u itself, then randomise again
			if (random_vertex == u) {
				random_vertex = barabasi_array[(random_index+v0) % current_arr_size];
			}
				// add edges in both directions
				EdgeNodePtr uv;
				uv = malloc(sizeof * uv);
				uv->edge.to_vertex = random_vertex;
				uv->next = G.edges[u].head;
				G.edges[u].head = uv;

				EdgeNodePtr vu;
				vu = malloc(sizeof * vu);
				vu->edge.to_vertex = u;
				vu->next = G.edges[random_vertex].head;
				G.edges[random_vertex].head = vu;
				
				//storing vertices in barabasi_array as many times their degrees increase
				push(barabasi_array, current_arr_size, u, &current_arr_size);
				push(barabasi_array, current_arr_size, random_vertex, &current_arr_size);
		}
	}

	return G;
}

