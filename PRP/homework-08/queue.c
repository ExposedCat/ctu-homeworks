#include "queue.h"

#include <stdbool.h>

// Constants
#define NOT_ENOUGH_MEMORY_HINT "E: Not enough memory to (re)allocate!\n"
#define NOT_ENOUGH_MEMORY_CODE 1
#define QUEUE_EXTENTION_FACTOR 2
#define QUEUE_REDUCTION_FACTOR 3

// Shourctuts
#define Queue queue_t*
#define repeat(n) for (int i = 0; i < n; ++i)

// Helpers
/*
    Memory allocation helpers raise user-friendly errors on allocation errors so
   by using them we can be sure allocation will be safe (otherwise program will
   be interrupted)
*/
void* safe_alloc(size_t size) {
    void* memory = malloc(size);
    if (!memory) {
        fprintf(stderr, NOT_ENOUGH_MEMORY_HINT);
        exit(NOT_ENOUGH_MEMORY_CODE);
    }
    return memory;
}

void safe_realloc(void*** memory, size_t size) {
    *memory = realloc(*memory, size);
    if (!*memory) {
        fprintf(stderr, NOT_ENOUGH_MEMORY_HINT);
        exit(NOT_ENOUGH_MEMORY_CODE);
    }
}

// Services
Queue create_queue(int capacity) {
    Queue queue = safe_alloc(sizeof(queue_t));
    queue->head = -1;
    queue->tail = -1;
    queue->capacity = capacity;
    queue->used = 0;
    queue->body = safe_alloc(sizeof(void*) * capacity);
    return queue;
}

void shrink_queue_if_possible(Queue queue) {
    /*
        Algorithm: Shift elements from head and on to the nearest previous
       element to get rid of "holes" between actual first (zero) element and
       last filled element. After this, we can just "cut" extra elements at the
       end of the array by reallocating array memory from 0 to the last filled
       element.
    */
    // Check if reduction is possible with specified factor
    int reduce_by = queue->capacity / QUEUE_REDUCTION_FACTOR;
    int max_used = queue->capacity - reduce_by;
    bool can_shrink = queue->used <= max_used;
    if (!can_shrink) {
        return;
    }
    // Shift elements
    bool cycled = queue->tail < queue->head;
    int current = queue->head;
    int start_position = cycled ? queue->tail + 1 : 0;
    for (int i = start_position; i < queue->used; ++i, ++current) {
        queue->body[i] = queue->body[current];
    }
    // Update queue metadata
    queue->head = start_position;
    if (!cycled) {
        queue->body[queue->tail] = NULL;
        queue->tail = queue->used - 1;
    }
    queue->capacity -= reduce_by;
    // Reallocate memory for queue body (array of elements inside)
    safe_realloc(&queue->body, sizeof(void*) * queue->capacity);
}

void extend_queue(Queue queue) {
    queue->capacity *= QUEUE_EXTENTION_FACTOR;
    safe_realloc(&queue->body, sizeof(void*) * queue->capacity);
}

void delete_queue(Queue queue) {
    free(queue->body);
    free(queue);
}

// Never returns `false` because allocates additional elements if necessary
bool push_to_queue(Queue queue, void* data) {
    if (get_queue_size(queue) == queue->capacity) {
        extend_queue(queue);
    }
    // Adding first element (head is unset)
    if (queue->head == -1) {
        queue->head = 0;
        queue->tail = 0;
    } else {
        // Shift tail if there are already some elements
        queue->tail += 1;
    }
    // If position bounds out of queue, start from the beginning
    if (queue->tail == queue->capacity) {
        queue->tail = 0;
    }
    queue->body[queue->tail] = data;
    queue->used += 1;
    return true;
}

void* pop_from_queue(Queue queue) {
    if (get_queue_size(queue) == 0) {
        return NULL;
    }
    void* data = queue->body[queue->head];
    queue->body[queue->head] = NULL;
    queue->head += 1;
    // If position bounds out of queue, set head to the beginning
    if (queue->head == queue->capacity) {
        queue->head = 0;
    }
    queue->used -= 1;
    if (get_queue_size(queue) == 0) {
        // Reset head & tail if all elements were removed
        queue->head = -1;
        queue->tail = -1;
    } else {
        shrink_queue_if_possible(queue);
    }
    return data;
}

void* get_from_queue(Queue queue, int idx) {
    if (idx < 0 || idx >= get_queue_size(queue)) {
        return NULL;
    }
    int position = queue->head + idx;
    // If position bounds out of queue, start from the beginning
    if (position >= queue->capacity) {
        position -= queue->capacity;
    }
    return queue->body[position];
}

int get_queue_size(Queue queue) {  //
    return queue->used;
}
