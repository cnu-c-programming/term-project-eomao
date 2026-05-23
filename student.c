#include "student.h"

#include <stdlib.h>

void init_student_list(StudentList *list) {
    list->head = NULL;
}

void clear_student_list(StudentList *list) {
    Student *cur = list->head;

    while (cur != NULL) {
        Student *next = cur->next;
        free(cur);
        cur = next;
    }

    list->head = NULL;
}

int count_students(const StudentList *list) {
    int count = 0;
    const Student *cur = list->head;

    while (cur != NULL) {
        count++;
        cur = cur->next;
    }

    return count;
}

Student *find_student(StudentList *list, int id) {
    Student *cur = list->head;

    while (cur != NULL) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }

    return NULL;
}

int add_student(StudentList *list, int id, const char *name, int score) {
    (void)list;
    (void)id;
    (void)name;
    (void)score;
    return 0;
}

int delete_student(StudentList *list, int id) {
    (void)list;
    (void)id;
    return 0;
}

int update_student(StudentList *list, int id, int score) {
    (void)list;
    (void)id;
    (void)score;
    return 0;
}

void sort_students_by_name(StudentList *list) {
    (void)list;
}

void sort_students_by_score(StudentList *list) {
    (void)list;
}
