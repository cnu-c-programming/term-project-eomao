#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

int load_students(const char *path, StudentList *list);
int save_students(const char *path, const StudentList *list);

#endif
