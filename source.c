#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
 
const int arraySize = 30;
const int semestrSize = 8;
const int MAXMARK = 10;
const int MINMARK = 1;

struct students_t {
    char *name;
    int *marks;
    int marksSize;
    int **exams;
    int *examsSize;
    float average;  /*for exams*/
};

struct groups_t {
    struct students_t *student;
    int nextStudent;
};

void addStudent(struct groups_t *group, int *lastGroup);
int inputGroup(int *lastGroup);
char *inputName(struct students_t *student, int *nextStudent);
int inputMarks(int *marks);
void inputExams(int **exams, int *examsSize);
float searchAverage(int **exams, int *examsSize);

int printAll(struct groups_t *group, int lastGroup);

float searchMinAverage(struct groups_t *group, int lastGroup);
float searchMaxAverage(struct groups_t *group, int lastGroup);

void freeAll(struct groups_t *group);

int main(int argc, char *argv[])
{
    int i;
    int lastGroup = 0;
    float average;
    char command[arraySize];
    struct groups_t *group;
    group = (struct groups_t*)malloc(arraySize * sizeof(struct groups_t));
    for (i = 0; i < arraySize; i++) {
        group[i].nextStudent = 0;
        group[i].student = (struct students_t*)malloc(arraySize * sizeof(struct students_t));
    }

    while (1) {
        printf("\nSelect the action:\n");
        printf("    \\e - Add student\n    \\p - Print all\n    \\x - Search for average min\n");
        printf("    \\n - Search for average max\n    \\q - Exit\n");
        fgets(command, arraySize, stdin);
        if (command[0] != '\\') {
            printf("Wrong command!\n");
            continue;
        }
        switch (command[1]) {
        case 'e':
            addStudent(group, &lastGroup);
            break;
        case 'p':
            if (!(printAll(group, lastGroup))) {
                printf("Nothing is entered.\n");
            }
            break;
        case 'x':
            average = searchMinAverage(group, lastGroup);
            if (!average) {
                printf("Neither exam is not entered.\n");
                break;
            }
            printf("Min is %0.3f\n", average);
            break;
        case 'n':
            average = searchMaxAverage(group, lastGroup);
            if (!average) {
                printf("Neither exam is not entered.\n");
                break;
            }
            printf("Max is %0.3f\n", average);
            break;
        case 'q':
            freeAll(group);
            return 0;
        default:
            printf("Wrong command!\n");
        }
    }
    return 0;
}

void addStudent(struct groups_t *group, int *lastGroup)
{
    int groupNum, studentNum, semNum;
    groupNum = inputGroup(lastGroup);
    group[groupNum].student[group[groupNum].nextStudent].name = 
        inputName(group[groupNum].student, &(group[groupNum].nextStudent));
    studentNum = (group[groupNum].nextStudent)++;

    group[groupNum].student[studentNum].marks = (int *)malloc(arraySize * sizeof(int));
    printf("Enter marks: ");
    group[groupNum].student[studentNum].marksSize = inputMarks(group[groupNum].student[studentNum].marks);

    group[groupNum].student[studentNum].exams = (int **)malloc(semestrSize * sizeof(int*));
    group[groupNum].student[studentNum].examsSize = (int *)malloc(arraySize * sizeof(int));
    for (semNum = 0; semNum < semestrSize; semNum++) {
        group[groupNum].student[studentNum].exams[semNum] = (int *)malloc(arraySize * sizeof(int));
    }
    inputExams(group[groupNum].student[studentNum].exams, group[groupNum].student[studentNum].examsSize);

    group[groupNum].student[studentNum].average = 
        searchAverage(group[groupNum].student[studentNum].exams, group[groupNum].student[studentNum].examsSize);
}

int inputGroup(int *lastGroup)
{
    int groupNum;
    char groupStr[arraySize];
    do {
        printf("Enter group number: ");
        fgets(groupStr, arraySize, stdin);
        groupNum = atoi(groupStr);
        if (groupNum > arraySize || groupNum < 1) {
            printf("Wrong number.\n");
            continue;
        }
        break;
    } while (1);
    if (groupNum >= *lastGroup) {
    *lastGroup = groupNum;
    }
    return --groupNum;
}

char *inputName(struct students_t *student, int *nextStudent)
{
    student[*nextStudent].name = (char *)malloc(arraySize * sizeof(char));
    do {
        printf("Enter student's name: ");
        __fpurge(stdin);
        fgets(student[*nextStudent].name, arraySize, stdin);
        if (student[*nextStudent].name[0] == '\n') {
            continue;
        }
        break;
    } while (1);
    return student[*nextStudent].name;
}

int inputMarks(int *marks)
{
    int i, check = 1;
    char *marksStr, *buffer;
    buffer = (char *)malloc(arraySize * sizeof(char));
    marksStr = buffer;
    do {
        __fpurge(stdin);
        fgets(marksStr, arraySize, stdin);
        for (i = 0; ; i++) {
            while (marksStr[0] == ' ') {
                marksStr++;
            }
            if (!marksStr[1]) {
                check = 0;
                break;
            }
            marks[i] = strtol(marksStr, &marksStr, 10);
            if ((marks[i] < MINMARK) || (marks[i] > MAXMARK)) {
                printf("Marks are not valid, try again: ");
                break;
            }
        }
    } while (check);
    free(buffer);
    return i;
}

void inputExams(int **exams, int *examsSize)
{
    int semNum;
    printf("Enter exams:\n");
    for (semNum = 0; semNum < semestrSize; semNum++) {
        printf("    %d semestr: ", semNum + 1);
        examsSize[semNum] = inputMarks(exams[semNum]);
    }
}

float searchAverage(int **exams, int *examsSize)
{
    int i, j, count = 0, sum = 0;
    float result;
    for (i = 0; i < semestrSize; i++) {
        for (j = 0; j < examsSize[i]; j++) {
            sum += exams[i][j];
            count++;
        }
    }
    if (count == 0) {
        return 0;
    }
    result = (float)sum / (float)count;
    return result;
}

int printAll(struct groups_t *group, int lastGroup)
{
    int groupNum, studentNum, semNum, i, check = 0;
    for (groupNum = 0; groupNum < lastGroup; groupNum++) {
        printf("\nGroup #%d", groupNum + 1);
        for (studentNum = 0; studentNum < group[groupNum].nextStudent; studentNum++) {
            check = 1;
            printf("\n    %s", group[groupNum].student[studentNum].name);
            printf("        marks: ");
            for (i = 0; i < group[groupNum].student[studentNum].marksSize; i++) {
                printf("%d ", group[groupNum].student[studentNum].marks[i]);
            }
            printf("\n        exams:");
            for (semNum = 0; semNum < semestrSize; semNum++) {
                printf("\n            semestr #%d: ", semNum);
                for (i = 0; i < group[groupNum].student[studentNum].examsSize[semNum]; i++) {
                    printf("%d ", group[groupNum].student[studentNum].exams[semNum][i]);
                }
            }
        }
    }
    return check;
}

float searchMinAverage(struct groups_t *group, int lastGroup)
{
    int groupNum, studentNum;
    float tempMin = MAXMARK + 1;
    for (groupNum = 0; groupNum < lastGroup; groupNum++) {
        for (studentNum = 0; studentNum < group[groupNum].nextStudent; studentNum++) {
            if (group[groupNum].student[studentNum].average == 0) {
                continue;
            }
            if (group[groupNum].student[studentNum].average < tempMin) {
                tempMin = group[groupNum].student[studentNum].average;
            }
        }
    }
    if (tempMin == MAXMARK + 1) {
        return 0;
    }
    return tempMin;
}

float searchMaxAverage(struct groups_t *group, int lastGroup)
{
    int groupNum, studentNum;
    float tempMax = MINMARK - 1;
    for (groupNum = 0; groupNum < lastGroup; groupNum++) {
        for (studentNum = 0; studentNum < group[groupNum].nextStudent; studentNum++) {
            if (group[groupNum].student[studentNum].average == 0) {
                continue;
            }
            if (group[groupNum].student[studentNum].average > tempMax) {
                tempMax = group[groupNum].student[studentNum].average;
            }
        }
    }
    return tempMax;
}

void freeAll(struct groups_t *group)
{
    int groupNum, studentNum, semNum;
    for (groupNum = 0; groupNum < arraySize; groupNum++) {
        for (studentNum = 0; studentNum < group[groupNum].nextStudent; studentNum++) {
            free(group[groupNum].student[studentNum].name);
            free(group[groupNum].student[studentNum].marks);
            free(group[groupNum].student[studentNum].examsSize);
            for (semNum = 0; semNum < semestrSize; semNum++) {
                free(group[groupNum].student[studentNum].exams[semNum]);
            }
            free(group[groupNum].student[studentNum].exams);
        }
        free(group[groupNum].student);
    }
    free(group);
}
