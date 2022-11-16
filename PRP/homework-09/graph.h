#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdint.h>
#include <stdlib.h>

#define Graph graph_t*
#define Connection graph_connection*
#define Number uint32_t
#define string char*

typedef struct {
    Number from;
    Number to;
    Number cost;
} graph_connection;

typedef struct {
    Connection* connections;
    size_t capacity;
    size_t nodes_number;
} graph_t;

/* Allocate a new graph and return a reference to it. */
Graph allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(Graph* graph);

/* Load a graph from the text file. */
void load_txt(const string fname, Graph graph);
/* Load a graph from the binary file. */
void load_bin(const string fname, Graph graph);

/* Save the graph to the text file. */
void save_txt(const Graph const graph, const string fname);
/* Save the graph to the binary file. */
void save_bin(const Graph const graph, const string fname);

#endif  // __GRAPH_H__
