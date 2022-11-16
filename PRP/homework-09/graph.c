#include "graph.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define Graph graph_t*
#define Connection graph_connection*
#define File FILE*
#define Number uint32_t
#define string char*

#define forever while (true)

// Constants

const int INIT_GRAPH_CAPACITY = 1000;

const string error_hints[2] = {
    "Can't allocate memory",
    "Can't open file",
};

enum error_codes {
    CANT_ALLOCATE_MEMORY,
    CANT_OPEN_FILE,
};

// Helpers
void raise_error(int error_code) {
    fprintf(stderr, "E: %s [%d]", error_hints[error_code], error_code);
    exit(error_code);
}

void* safe_alloc(int count, size_t size) {
    void* memory = malloc(size * count);
    if (memory == NULL) {
        raise_error(CANT_ALLOCATE_MEMORY);
    }
    return memory;
}

void safe_realloc_connections(Graph graph) {
    Connection* connections = realloc(        //
        graph->connections,                   //
        sizeof(Connection) * graph->capacity  //
    );
    if (connections == NULL) {
        free_graph(&graph);
        raise_error(CANT_ALLOCATE_MEMORY);
    }
    graph->connections = connections;
}

File open_file(const string path, const string mode) {
    File file = fopen(path, mode);
    if (file == NULL) {
        raise_error(CANT_OPEN_FILE);
    }
    return file;
}

// Services
Graph allocate_graph() {
    Graph graph = safe_alloc(1, sizeof(graph_t));
    graph->capacity = INIT_GRAPH_CAPACITY;
    graph->nodes_number = 0;
    graph->connections = safe_alloc(graph->capacity, sizeof(Connection));
    for (size_t i = 0; i < graph->capacity; ++i) {
        graph->connections[i] = safe_alloc(1, sizeof(graph_connection));
    }
    return graph;
}

void extend_graph(Graph graph) {
    graph->capacity += INIT_GRAPH_CAPACITY;
    safe_realloc_connections(graph);
    size_t first = graph->capacity - INIT_GRAPH_CAPACITY;
    for (size_t i = first; i < graph->capacity; ++i) {
        graph->connections[i] = safe_alloc(1, sizeof(graph_connection));
    }
}

void free_graph(Graph* graph) {
    for (size_t i = 0; i < (*graph)->capacity; ++i) {
        free((*graph)->connections[i]);
    }
    free((*graph)->connections);
    free(*graph);
    *graph = NULL;
}

bool fscan(Number* number, File file, char end) {
    *number = 0;
    forever {
        char character = fgetc(file);
        if (character == EOF || character == end) {
            *number /= 10;
            return character == EOF;
        }
        // 0-char shift for correct type cast
        *number += character - '0';
        *number *= 10;
    }
}

void load_txt(const string fname, Graph graph) {
    File file = open_file(fname, "r");
    graph->nodes_number = 0;
    for (size_t i = 0;; ++i) {
        while (graph->capacity <= i) {
            extend_graph(graph);
        }
        int end = fscan(&graph->connections[i]->from, file, ' ');
        if (end) {
            break;
        }
        fscan(&graph->connections[i]->to, file, ' ');
        fscan(&graph->connections[i]->cost, file, '\n');
        graph->nodes_number++;
    }
    fclose(file);
}

void fprint(Number number, File file, char end) {
    if (number == 0) {
        fputc('0', file);
    } else {
        char buffer[10];
        int index = 0;
        while (number) {
            // 0-char shift for correct type cast
            buffer[index] = (number % 10) + '0';
            index += 1;
            number /= 10;
        }
        index -= 1;
        while (index >= 0) {
            fputc(buffer[index], file);
            index -= 1;
        }
    }
    fputc(end, file);
}

void save_txt(const Graph const graph, const string fname) {
    File file = open_file(fname, "w+");
    for (size_t i = 0; i < graph->nodes_number; ++i) {
        fprint(graph->connections[i]->from, file, ' ');
        fprint(graph->connections[i]->to, file, ' ');
        fprint(graph->connections[i]->cost, file, '\n');
    }
    fclose(file);
}

void save_bin(const Graph const graph, const string fname) {
    File file = open_file(fname, "w+b");
    for (size_t i = 0; i < graph->nodes_number; ++i) {
        fwrite(&graph->connections[i]->from, sizeof(uint32_t), 1, file);
        fwrite(&graph->connections[i]->to, sizeof(uint32_t), 1, file);
        fwrite(&graph->connections[i]->cost, sizeof(uint32_t), 1, file);
    }
    fclose(file);
}

void load_bin(const string fname, Graph graph) {
    File file = open_file(fname, "rb");
    graph->nodes_number = 0;
    for (size_t i = 0;; ++i) {
        while (graph->capacity <= i) {
            extend_graph(graph);
        }
        graph->nodes_number += 1;
        int success = fread(               //
            &graph->connections[i]->from,  //
            sizeof(uint32_t), 1,           //
            file                           //
        );
        if (!success) {
            graph->nodes_number -= 1;
            break;
        }
        fread(&graph->connections[i]->to, sizeof(uint32_t), 1, file);
        fread(&graph->connections[i]->cost, sizeof(uint32_t), 1, file);
    }
    fclose(file);
}
