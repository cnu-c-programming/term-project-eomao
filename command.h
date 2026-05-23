#ifndef COMMAND_H
#define COMMAND_H

#include "student.h"

typedef enum {
    COMMAND_CONTINUE = 0,
    COMMAND_EXIT,
    COMMAND_ERROR
} CommandResult;

CommandResult execute_command(StudentList *list, const char *csv_path, const char *line);

#endif
