#include <stdio.h>
#include "process.h"

// Print the tree recursively
void print_tree(Process *process, int level) {
    // Print indentation using '\t' for each level
    for (int i = 0; i < level; i++) {
        printf("\t"); // Add tab for indentation
    }

    // Print process details
    printf("%s (PID: %d, User: %s)\n", process->command, process->pid, process->user);

    // Recursively print children
    for (int i = 0; i < process->child_count; i++) {
        print_tree(process->children[i], level + 1); // Increase level for children
    }
}
