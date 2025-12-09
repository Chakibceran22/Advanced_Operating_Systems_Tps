#include "helpers.h"

void removeSpacesAndOuterParentheses(char *expr) {
    // FIRST: Remove all spaces
    int i = 0, j = 0;
    while (expr[i]) {
        if (expr[i] != ' ') {
            expr[j++] = expr[i];
        }
        i++;
    }
    expr[j] = '\0';
    
    // THEN: Remove outer parentheses
    int len = strlen(expr);
    while (len > 2 && expr[0] == '(' && expr[len-1] == ')') {
        int level = 0;
        bool isOuter = true;
        
        for (int i = 0; i < len - 1; i++) {
            if (expr[i] == '(') level++;
            if (expr[i] == ')') level--;
            if (level == 0) {
                isOuter = false;
                break;
            }
        }
        
        if (isOuter) {
            memmove(expr, expr + 1, len - 2);
            expr[len - 2] = '\0';
            len = strlen(expr);
        } else {
            break;
        }
    }
}


int findMainOperator(const char *exp) {
    int parenthesisLevel = 0;
    int positionOperator = -1;
    
    for(size_t i = 0; i < strlen(exp); i++) {
        if(exp[i] == ' ') continue;  // Skip spaces!
        
        if(exp[i] == '(') {
            parenthesisLevel++;
        } else if(exp[i] == ')') {
            parenthesisLevel--;
        }
        else if( exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') {
            if(parenthesisLevel == 0) {
                positionOperator = i;
                break;
            }
        }
    }
    
    return positionOperator;
}
