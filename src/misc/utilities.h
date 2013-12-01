//
//  utilities.h
//  hfsinspect
//
//  Created by Adam Knight on 11/26/13.
//  Copyright (c) 2013 Adam Knight. All rights reserved.
//

#ifndef hfsinspect_utilities_h
#define hfsinspect_utilities_h

#include <sys/types.h>

ssize_t fpread                  (FILE* f, void* buf, size_t nbytes, off_t offset);

#ifndef cmp
#define cmp(a, b) ((a) > (b) ? 1 : ((a) < (b) ? -1 : 0))
#endif

static inline int icmp(const void * a, const void * b)
{
    int64_t A = *(int64_t*)a;
    int64_t B = *(int64_t*)b;
    return cmp(A,B);
}

#endif