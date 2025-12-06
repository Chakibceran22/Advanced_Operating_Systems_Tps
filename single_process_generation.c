#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "validator.h"
#include "helpers.h"

int main() {
    char test[100] = "(( A + B) * (C - (D / E)))";
    
    if(validateExpression(test)) {
        removeOuterParentheses(test);
        printf("Cleaned expression: %s\n", test);
        
        int opIndex = findMainOperator(test);
        if(opIndex != -1) {
            printf("Main operator '%c' at index: %d\n", test[opIndex], opIndex);
        }
    }
    
    return 0;
}