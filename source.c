#include <stdio.h>
#include <malloc.h>
#include <stdio_ext.h>

struct info {
    char *name;
    int *estimate;
    int **exam;
    //int *averageExam;  //or average estimates?
    //int isModify;
};

struct group {
    struct info *student;
    int lastStudent;
};

void inputStruct(struct group*, int*);    //Need I write variables here?
int inputGroupNum(struct group*, int*);
int inputName(struct info*, int );
void inputEstimates(int*);
void inputExams(int**);

void printStruct(struct group*, int*);
void printStruct(struct group*, int*);

int minmaxAverageVal(struct group*, int, int);

int main()
{
    struct group *list;
    list = (struct group*)malloc(sizeof(struct group));
    list[0].lastStudent = 0;
    int groupMax = 0;
    char *action;
    int EXIT = 1;

    action = (char*)malloc(3 * sizeof(char));

    while (EXIT) {
        printf("    %d\n", groupMax);
        printf("Select the action:\n");
        printf("    \\e - Add student\n    \\p - Print all\n    \\x - Search for average min\n    \\n - Search for average max\n    \\q - Exit\n");
        fgets(action, 3, stdin);

        if (action[0] != '\\') {
            printf("Wrong command, try agan\n\n");
            continue;
        }

        switch (action[1]) {
            case 'e':
                inputStruct(list, &groupMax);
                break;
            case 'p':
                printStruct(list, &groupMax);
                break;
            case 'x':
                //printf("Group %d, average: %d", search(0), (list + search(0))->averageExam);
                break;
            case 'n':
                //printf("Group %d, average: %d", search(1), (list + search(1))->averageExam);
                break;
            case 'q':
                EXIT = 0;
                break;
            default:
                printf("Wrong command, try agan\n\n");
                break;
        }
        __fpurge(stdin);
    }

    return 0;
}

void inputStruct(struct group *list, int *groupMax)
{
    int groupNum, studentID;

    groupNum = inputGroupNum(list, groupMax);
    if (groupNum < 0) {
        return;
    }

    studentID = inputName(list[groupNum].student, list[groupNum].lastStudent);
    list[groupNum].lastStudent++;

    inputEstimates(list[groupNum].student[studentID].estimate);

    inputExams(list[groupNum].student[studentID].exam);
}

int inputGroupNum(struct group *list, int *groupMax)
{
    int groupNum;

    printf("Enter group:\n");
    scanf("%d", &groupNum);
    if (groupNum < 1) {
        printf("Error\n");
        return -1;
    }

    if (groupNum >= *groupMax) {
        list = (struct group*)realloc(list, groupNum * sizeof(struct group));
        for (; *groupMax <= groupNum; (*groupMax)++) {
            list[*groupMax].student = (struct info*)malloc(sizeof(struct info));
            list[*groupMax].lastStudent = 0;
        }
    }
    return --groupNum;
}

int inputName(struct info *student, int lastStudent)
{
    int studentID = lastStudent++;

    student = (struct info*)realloc(student, lastStudent * sizeof(struct info));
    student[studentID].name = (char*)malloc(25 * sizeof(char));

    printf("Enter name:\n");
    __fpurge(stdin);
    fgets(student[studentID].name, 25, stdin);

    return studentID;
}

void inputEstimates(int *estimate)
{
    int i, temp;
    estimate = (int*)malloc(sizeof(int));

    printf("Enter estimates, 0 to stop entry:\n");
    for(i = 1, temp = 1; temp; i++) {
        estimate = (int*)realloc(estimate, i * sizeof(int));
        scanf("%d", &temp);
        estimate[i -1] = temp;
    }
}

void inputExams(int **exam)
{
    int semNum, temp, i;
    exam = (int**)malloc(8 * sizeof(int*));

    printf("Enter exams, 0 to stop entry:\n");
    for (semNum = 0; semNum < 8; semNum++) {
        exam[semNum] = (int*)malloc(sizeof(int));

        printf("    %d semestr: ", semNum + 1);
        for (i = 1, temp = 1; temp; i++) {  // <7
            printf("               ");
            scanf("%d", &temp);
            exam[semNum][i - 1] = temp;
            exam[semNum] = (int*)realloc(exam[semNum], sizeof(int) * i); 
        }
    }
}


void printStruct(struct group *list, int *groupMax)
{
    int groupNum, studentID, semNum, i;

    for(groupNum = 0; groupNum < *groupMax; groupNum++){
        printf("\nGroup #%d: \n", groupNum + 1);

        for (studentID = 0; studentID < list[groupNum].lastStudent; studentID++) {
            printf("    %s\n", list[groupNum].student[studentID].name);

            printf("        estimates: ");
            for (i = 0; list[groupNum].student[studentID].estimate[i]; i++) {
                printf("%d ", list[groupNum].student[studentID].estimate[i]);
            }

            printf("\n        exams: ");
            for (semNum = 0; semNum < 8; semNum++) {
                printf("\n            %d semestr: ", semNum);
                for (i = 0; list[groupNum].student[studentID].exam[semNum][i]; ++i) {
                    printf("%d ", list[groupNum].student[studentID].exam[semNum][i]);
                }
            }
        }
    }
}

int minmaxAverageVal(struct group *list, int groupMax, int minmax)
{
    return 1;
}
