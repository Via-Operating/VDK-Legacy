#include "stdint.h"
#include "stdio.h"
#include "../../../libkern/keyboard.h"
#include <string.h>
#include <stdlib.h>

#define MAX_HISTORY 100

char* command_history[MAX_HISTORY];
int history_count = 0;

void append_hist(const char* command) {
    if (history_count < MAX_HISTORY) {
        command_history[history_count++] = strdup(command);
    }
}

void hist() {
    printf("\nCommand History:\n");
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, command_history[i]);
    }
    printf("> ");
}

int history() {
    hist();

    for (int i = 0; i < history_count; i++) {
        free(command_history[i]);
    }

    return 0;
}
