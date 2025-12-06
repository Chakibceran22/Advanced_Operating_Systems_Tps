#ifndef HELPERS_H
#define HELPERS_H
#include <string.h>
#include <stdbool.h>

int findMainOperator(const char *exp);
void removeSpacesAndOuterParentheses(char *expr);

#endif // HELPERS_H