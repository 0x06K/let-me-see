#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// void show_processes();
// void show_threads(int pid);
// void kill_process(int pid);
// void inspect_process(int pid);
// void show_help();

int main() {
    char input[256];
    char command[32];
    int pid;

    printf("ProcessSpectre CLI Interface\n");
    printf("Type 'help' to see available commands.\n");

    while (1) {
        printf("\n> ");
        fgets(input, sizeof(input), stdin);

        // Remove newline
        input[strcspn(input, "\n")] = 0;

        if (sscanf(input, "%s %d", command, &pid) >= 1) {
            if (strcmp(command, "exit") == 0) {
                break;
            } else if (strcmp(command, "help") == 0) {
                // show_help();
            } else if (strcmp(command, "show") == 0) {
                if (strstr(input, "proc")) {
                    // show_processes();
                } else if (strstr(input, "threads")) {
                    // show_threads(pid);
                } else {
                    printf("Invalid 'show' command.\n");
                }
            } else if (strcmp(command, "kill") == 0) {
                // kill_process(pid);
            } else if (strcmp(command, "inspect") == 0) {
                // inspect_process(pid);
            } else {
                printf("Unknown command. Type 'help' for list.\n");
            }
        }
    }

    return 0;
}


