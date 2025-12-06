#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "validator.h"
#include "helpers.h"

int taskCounter = 0;
int memoryCounter = 0;

char *generates( char *expr, int parenTaskId)
{

   
    removeSpacesAndOuterParentheses(expr);
    
    int mainOpIndex = findMainOperator(expr);
    
    if (mainOpIndex == -1) {
        return strdup(expr);
    }
    int myTaskId = ++taskCounter;

    char operator = expr[mainOpIndex];

    char leftExpr[100];
    char rightExpr[100];
    // we split the string into two substrings
    strncpy(leftExpr, expr, mainOpIndex);
    leftExpr[mainOpIndex] = '\0'; // Null terminate

    strcpy(rightExpr, expr + mainOpIndex + 1);

    char *leftResult = generates(leftExpr, myTaskId);
    char *rightResult = generates(rightExpr, myTaskId);

    char resultMemory[10];
    sprintf(resultMemory, "M%d", ++memoryCounter);
    printf("T%d : %s := %s %c %s\n",
           myTaskId, resultMemory, leftResult, operator, rightResult);

    // STEP 9: Precedence
    if (parenTaskId != -1)
    {
        printf("T%d < T%d\n", myTaskId, parenTaskId);
    }

    // STEP 10: Return
    return strdup(resultMemory);
}

int main()
{
    char test[100] = "(( A + B) * (C - (D / E))) ";

    if (validateExpression(test))
    {
        // cleaning the iput
        removeSpacesAndOuterParentheses(test);
        char *result = generates(test, -1);
    }

    return 0;
}