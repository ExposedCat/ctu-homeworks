#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

#define Graph graph_t*
#define Connection graph_connection*

void print_connection(Connection connection) {
    printf("{ from: %u, to: %u, cost: %u }",  //
           connection->from,                  //
           connection->to,                    //
           connection->cost                   //
    );
}

void print_graph(Graph graph) {
    printf("Capacity: %lu\n", graph->capacity);
    printf("Used: %lu\n", graph->nodes_number);
    printf("Connections:\n");
    for (size_t i = 0; i < graph->nodes_number; ++i) {
        print_connection(graph->connections[i]);
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    Graph graph = allocate_graph();
    if (argc > 1) {
        load_txt(argv[1], graph);
        printf("Read from %s [text]\n", argv[1]);
    }
    print_graph(graph);
    if (argc > 2) {
        save_bin(graph, argv[2]);
        printf("Wrote to %s [binary]\n", argv[2]);
    }
    if (argc > 2) {
        load_bin(argv[2], graph);
        printf("Read from %s [binary]\n", argv[2]);
    }
    print_graph(graph);
    if (argc > 3) {
        save_txt(graph, argv[3]);
        printf("Wrote to %s\n", argv[3]);
    }
    free_graph(&graph);
    return 0;
}
