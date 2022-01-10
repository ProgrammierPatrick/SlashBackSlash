#ifndef _TESTING_H
#define _TESTING_H

#include <iostream>
using namespace std;

int _assert_return = 0;

#ifdef _WIN32
#define _ASSERT_CERR_CURRENT_LOC __FILE__ << ":" << __LINE__ << " in " << __FUNCSIG__
#else
#define _ASSERT_CERR_CURRENT_LOC __FILE__ << ":" << __LINE__ << " in " << __PRETTY_FUNCTION__
#endif

#define ASSERT(X) \
    if(!(X)) { \
        cerr << _ASSERT_CERR_CURRENT_LOC << ":\n" \
            << "    Assertion Failed: " << #X << endl; \
        _assert_return = -1; \
    }

#define ASSERT_EQ(X,Y) \
    if((X) != (Y)) { \
        cerr << _ASSERT_CERR_CURRENT_LOC << ":\n" \
            << "    Assertion Failed: " << #X << " == " << #Y << '\n' \
            << "      " << #X << ": " << (X) << ",\n      " << #Y << ": " << (Y) << endl; \
        _assert_return = -1; \
    }

#define GET_ASSERT_RETURN (_assert_return)

#endif