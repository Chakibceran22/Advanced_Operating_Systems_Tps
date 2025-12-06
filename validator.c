
#include "validator.h"
#include <string.h>

bool validateExpression(const char *expression)
{
    if (expression == NULL || expression[0] == '\0')
    {
        return false; // for if the string is empty
    }
    bool isParenthesisEmpty = true;
    int parenthesisCount = 0;
    bool expectedOperand = true;
    size_t length = strlen(expression);
    for (size_t i = 0; i < length; i++)
    {
        char currentChar = expression[i];

        if (currentChar == ' ')
        {
            continue; // skip spaces
        }

        if (!(currentChar >= '0' && currentChar <= '9') &&
            !(currentChar >= 'a' && currentChar <= 'z') &&
            !(currentChar >= 'A' && currentChar <= 'Z') &&
            currentChar != '+' && currentChar != '-' &&
            currentChar != '*' && currentChar != '/' &&
            currentChar != '(' && currentChar != ')')
        {
            return false; // Invalid character found
        }

        if (currentChar == '(')
        {
            parenthesisCount++;
            size_t next = i + 1;
            while (next < length && expression[next] == ' ')
            {
                next++; // Skip spaces
            }

            if (next < length && expression[next] == ')')
            {
                return false; // Empty parentheses detected!
            }
            expectedOperand = true;
        }
        else if (currentChar == ')')
        {
            if (expectedOperand) // ADD THIS CHECK!
            {
                return false; // Operator before ')'
            }
            parenthesisCount--;
            if (parenthesisCount < 0)
            {
                return false;
            }
            expectedOperand = false;
        }
        else if (currentChar == '+' || currentChar == '-' ||
                 currentChar == '*' || currentChar == '/')
        {
            if (expectedOperand)
            {
                return false; // Operator found where operand was expected
            }

            expectedOperand = true;
        }
        else
        {
            if (!expectedOperand)
            {
                return false; // Operand found where operator was expected
            }
            expectedOperand = false;
        }
    }
    return (parenthesisCount == 0 && !expectedOperand);
}
