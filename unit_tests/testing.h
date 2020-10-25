#ifndef _TESTING_H
#define _TESTING_H

#include <iostream>
using namespace std;

#define ASSERT(X) \
    if(!(X)) { \
        cerr << __FILE__ << ":" << __LINE__ << " in " << __PRETTY_FUNCTION__ << ": " \
            << "Assertion Failed: " << #X << endl; \
        exit(-1); \
    }

#endif