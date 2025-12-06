#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include "validator.h"
#include "helpers.h"
#include <stdlib.h>
int *taskCounter;
int *memoryCounter;

void initSharedCounters()
{
    // Create shared memory for counters to share between processes
    taskCounter = mmap(NULL, sizeof(int),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    memoryCounter = mmap(NULL, sizeof(int),
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *taskCounter = 0;
    *memoryCounter = 0;
}

char *generates(char *expr, int parenTaskId)
{
    removeSpacesAndOuterParentheses(expr);
    int mainOpIndex = findMainOperator(expr);

    if (mainOpIndex == -1)
    {
        return strdup(expr);
    }

    int myTaskId = ++(*taskCounter); // Use pointer!
    char operator = expr[mainOpIndex];

    char leftExpr[100], rightExpr[100];
    strncpy(leftExpr, expr, mainOpIndex);
    leftExpr[mainOpIndex] = '\0';
    strcpy(rightExpr, expr + mainOpIndex + 1);

    int leftPipe[2], rightPipe[2];
    pipe(leftPipe);
    pipe(rightPipe);

    pid_t leftPid = fork();
    if (leftPid == 0)
    {
        close(leftPipe[0]);
        char *result = generates(leftExpr, myTaskId);
        write(leftPipe[1], result, strlen(result) + 1);
        close(leftPipe[1]);
        exit(0);
    }

    pid_t rightPid = fork();
    if (rightPid == 0)
    {
        close(rightPipe[0]);
        char *result = generates(rightExpr, myTaskId);
        write(rightPipe[1], result, strlen(result) + 1);
        close(rightPipe[1]);
        exit(0);
    }

    close(leftPipe[1]);
    close(rightPipe[1]);

    char leftResult[100], rightResult[100];
    read(leftPipe[0], leftResult, 100);
    read(rightPipe[0], rightResult, 100);

    close(leftPipe[0]);
    close(rightPipe[0]);

    waitpid(leftPid, NULL, 0);
    waitpid(rightPid, NULL, 0);

    char resultMemory[10];
    sprintf(resultMemory, "M%d", ++(*memoryCounter)); // Use pointer!
    printf("T%d : %s := %s %c %s\n",
           myTaskId, resultMemory, leftResult, operator, rightResult);

    if (parenTaskId != -1)
    {
        printf("T%d < T%d\n", myTaskId, parenTaskId);
    }

    return strdup(resultMemory);
}

int main()
{
    initSharedCounters(); // Initialize shared memory!

    char test[100] = "(((A+B)*C)-(((D-(F/G))*(H+(K*L)))/((M-N)*O)))";

    if (validateExpression(test))
    {
        removeSpacesAndOuterParentheses(test);
        char *result = generates(test, -1);
        printf("\nFinal result: %s\n", result);
        free(result);
    }

    return 0;
}