#pragma once

void Keybi_Tests_Run(void);

#define KEYBI_ASSERT(expr) \
    if (!(expr)) \
        Keybi_Test_Failed(__FILE__, __LINE__, #expr)

void Keybi_Test_Failed(char *file, int line, const char * expr);
