#include "command.h"

#include <stdio.h>
#include <string.h>

CommandResult execute_command(StudentList *list, const char *csv_path, const char *line) {
    (void)list;
    (void)csv_path;

    if (line[0] == '\0') {
        return COMMAND_CONTINUE;
    }

    if (strcmp(line, "exit") == 0) {
        printf("Goodbye.\n");
        return COMMAND_EXIT;
    }

    printf("Unknown command or permission denied.\n");
    return COMMAND_ERROR;
}
