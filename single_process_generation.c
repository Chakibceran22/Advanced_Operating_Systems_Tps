
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "validator.h"

int main()
{
    const char *testExpressions = "(( A + B) * (C - (D / E)))   ";

    printf("Expression: %s\n", testExpressions);
    if (validateExpression(testExpressions))
    {
        printf("The expression is valid.\n");
    }
    else
    {
        printf("The expression is invalid.\n");
    }

    return 0;
}