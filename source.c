#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <string.h>

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
    float average;              /*for exams */
};

struct groups_t {
    struct students_t *student;
    int nextStudent;
};

void addStudent(struct groups_t *group, int *lastGroup);
int inputGroup(int *lastGroup);
char *inputName();
int inputMarks(int *marks);
int *inputExams(int **exams);
float searchAverage(int **exams, int *examsSize);

int printAll(struct groups_t *group, int lastGroup);

float searchMinAverageExam(struct groups_t *group, int lastGroup);
float searchMaxAverageExam(struct groups_t *group, int lastGroup);

void freeAll(struct groups_t *group);
void checkHelp(char **argv);

int main(int argc, char **argv)
{
    if (argc > 1) {
        checkHelp(argv);
    }
    int i;
    int lastGroup = 0;
    float average;
    char command[arraySize];
    struct groups_t *group;
    group = (struct groups_t *) malloc(arraySize * sizeof(struct groups_t));
    for (i = 0; i < arraySize; i++) {
        group[i].nextStudent = 0;
        group[i].student = (struct students_t *) malloc(arraySize * sizeof(struct students_t));
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
            average = searchMinAverageExam(group, lastGroup);
            if (!average) {
                printf("Neither exam is not entered.\n");
                break;
            }
            printf("Min is %0.3f\n", average);
            break;
        case 'n':
            average = searchMaxAverageExam(group, lastGroup);
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
    int grNum, studNum;
    grNum = inputGroup(lastGroup);
    group[grNum].student[group[grNum].nextStudent].name = inputName();
    studNum = (group[grNum].nextStudent)++;

    printf("Enter marks: ");
    group[grNum].student[studNum].marks = (int *) malloc(arraySize * sizeof(int));
    group[grNum].student[studNum].marksSize = inputMarks(group[grNum].student[studNum].marks);

    group[grNum].student[studNum].exams = (int **) malloc(semestrSize * sizeof(int *));
    group[grNum].student[studNum].examsSize = inputExams(group[grNum].student[studNum].exams);
    group[grNum].student[studNum].average = searchAverage(group[grNum].student[studNum].exams, group[grNum].student[studNum].examsSize);
}

int inputGroup(int *lastGroup)
{
    int grNum;
    char groupStr[arraySize];
    do {
        printf("Enter group number: ");
        fgets(groupStr, arraySize, stdin);
        grNum = atoi(groupStr);
        if (grNum > arraySize || grNum < 1) {
            printf("Wrong number.\n");
            continue;
        }
        break;
    } while (1);
    if (grNum >= *lastGroup) {
        *lastGroup = grNum;
    }
    return --grNum;
}

char *inputName()
{
    char *buffer;
    buffer = (char *) malloc(arraySize * sizeof(char));
    do {
        printf("Enter student's name: ");
        __fpurge(stdin);
        fgets(buffer, arraySize, stdin);
        if (buffer[0] == '\n') {
            continue;
        }
        break;
    } while (1);
    return buffer;
}

int inputMarks(int *marks)
{
    int i, check = 1;
    char *marksStr, *buffer;
    buffer = (char *) malloc(arraySize * sizeof(char));
    marksStr = buffer;
    do {
        __fpurge(stdin);
        fgets(marksStr, arraySize, stdin);
        for (i = 0;; i++) {
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

int *inputExams(int **exams)
{
    int semNum, *examsSize;
    examsSize = (int *) malloc(arraySize * sizeof(int));
    printf("Enter exams:\n");
    for (semNum = 0; semNum < semestrSize; semNum++) {
        printf("    %d semestr: ", semNum + 1);
        exams[semNum] = (int *) malloc(arraySize * sizeof(int));
        examsSize[semNum] = inputMarks(exams[semNum]);
    }
    return examsSize;
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
    result = (float) sum / (float) count;
    return result;
}

int printAll(struct groups_t *group, int lastGroup)
{
    int grNum, studNum, semNum, i, check = 0;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        printf("\nGroup #%d", grNum + 1);
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            check = 1;
            printf("\n    %s", group[grNum].student[studNum].name);
            printf("        marks: ");
            for (i = 0; i < group[grNum].student[studNum].marksSize; i++) {
                printf("%d ", group[grNum].student[studNum].marks[i]);
            }
            printf("\n        exams:");
            for (semNum = 0; semNum < semestrSize; semNum++) {
                printf("\n            semestr #%d: ", semNum);
                for (i = 0; i < group[grNum].student[studNum].examsSize[semNum]; i++) {
                    printf("%d ", group[grNum].student[studNum].exams[semNum][i]);
                }
            }
        }
    }
    return check;
}

float searchMinAverageExam(struct groups_t *group, int lastGroup)
{
    int grNum, studNum;
    float tempMin = MAXMARK + 1;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            if (group[grNum].student[studNum].average == 0) {
                continue;
            }
            if (group[grNum].student[studNum].average < tempMin) {
                tempMin = group[grNum].student[studNum].average;
            }
        }
    }
    if (tempMin == MAXMARK + 1) {
        return 0;
    }
    return tempMin;
}

float searchMaxAverageExam(struct groups_t *group, int lastGroup)
{
    int grNum, studNum;
    float tempMax = MINMARK - 1;
    for (grNum = 0; grNum < lastGroup; grNum++) {
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            if (group[grNum].student[studNum].average == 0) {
                continue;
            }
            if (group[grNum].student[studNum].average > tempMax) {
                tempMax = group[grNum].student[studNum].average;
            }
        }
    }
    return tempMax;
}

void freeAll(struct groups_t *group)
{
    int grNum, studNum, semNum;
    for (grNum = 0; grNum < arraySize; grNum++) {
        for (studNum = 0; studNum < group[grNum].nextStudent; studNum++) {
            free(group[grNum].student[studNum].name);
            free(group[grNum].student[studNum].marks);
            free(group[grNum].student[studNum].examsSize);
            for (semNum = 0; semNum < semestrSize; semNum++) {
                free(group[grNum].student[studNum].exams[semNum]);
            }
            free(group[grNum].student[studNum].exams);
        }
        free(group[grNum].student);
    }
    free(group);
}

void checkHelp(char **argv)
{
    if (!strcmp(argv[1], "-h")) {
        printf("================================================================================\n"
               "                                     MANUAL:\n\n"
               "Commands: \\e \\p \\x \\n \\q\n\n"
               "\\e - Structure input\n"
               "    1) Enter the group number. Number must be > 0 and < 30\n"
               "    2) Enter the name of the student. The name must not be an empty string.\n"
               "    3) Enter marks through spaces. Marks should be integers from 1 to 10. "
               "If marks are missing, enter an empty string.\n"
               "    4) Enter semester exams. Same as marks.\n\n"
               "\\p - Print whole structure.\n"
               "    Displays all entries.\n\n"
               "\\x - Searching for the maximum average of exam\n"
               "    Displays the maximum average of exams.\n\n"
               "\\n - Searching for the minimum average of exam\n"
               "    Displays the minimum average of exams.\n\n"
               "\\q - Exit\n"
               "    Close the program.\n"
               "================================================================================\n");
    }
    exit(0);
}