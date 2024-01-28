#pragma once

#include <stdio.h>

#define UNROLL_FACTOR_TWO   2
#define UNROLL_FACTOR_FOUR  4
#define UNROLL_FACTOR_EIGHT 8


#define unroll_by_two( iterator, ... )  \
        {__VA_ARGS__;}                  \
        ++iterator;                     \
        {__VA_ARGS__;}                  \
        ++iterator;

#define unroll_by_four( iterator, ... ) \
        unroll_by_two( iterator, __VA_ARGS__ ) \
        unroll_by_two( iterator, __VA_ARGS__ )

#define unroll_by_eight( iterator, ... ) \
        unroll_by_four( iterator, __VA_ARGS__ )  \
        unroll_by_four( iterator, __VA_ARGS__ )



#define UNROLL_LOOP_by_UNROLL_FACTOR_TWO( iterator, ... ) \
        unroll_by_two( iterator, __VA_ARGS__ )

#define UNROLL_LOOP_by_UNROLL_FACTOR_FOUR( iterator, ... ) \
        unroll_by_four( iterator, __VA_ARGS__ )

#define UNROLL_LOOP_by_UNROLL_FACTOR_EIGHT( iterator, ... ) \
        unroll_by_eight( iterator, __VA_ARGS__ )



#define UNROLL_LOOP( factor, iterator, init, bound, ... )               \
        size_t rest = ( ( bound ) - ( init ) ) % factor;                \
        size_t last_idx = ( bound ) - rest;                             \
        for( size_t iterator = ( init ); iterator < ( bound ) - rest; ) \
        {                                                               \
                UNROLL_LOOP_by_##factor( iterator, __VA_ARGS__ )        \
        }                                                               \
        for( size_t iterator = last_idx; iterator < bound; ++iterator ) \
        {                                                               \
                __VA_ARGS__;                                            \
        }
