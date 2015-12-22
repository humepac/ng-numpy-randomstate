#define __SIZEOF_INT128__ 16
#define RNG_TYPE pcg64_random_t
#include <stdint.h>

#include "../../src/common/binomial.h"
#include "../../src/common/entropy.h"
#include "../../src/pcg/pcg_variants.h"

typedef struct s_aug_state {
    pcg64_random_t *rng;
    binomial_t *binomial;

    int has_gauss, shift_zig_random_int, has_uint32;
    double gauss;
    uint32_t uinteger;
    uint64_t zig_random_int;
} aug_state;

inline uint32_t random_uint32(aug_state* state)
{
    if (state->has_uint32)
    {
        state->has_uint32 = 0;
        return state->uinteger;
    }
    state->has_uint32 = 1;
    uint64_t temp;
    temp = pcg64_random_r(state->rng);
    state->uinteger = (uint32_t)(temp >> 32);
    return (uint32_t)(temp & 0xFFFFFFFFLL);
}

inline uint64_t random_uint64(aug_state* state)
{
    return pcg64_random_r(state->rng);
}

inline void seed(aug_state* state, pcg128_t seed, pcg128_t inc)
{
    pcg64_srandom_r(state->rng, seed, inc);
}

inline void advance(aug_state* state, pcg128_t delta)
{
    pcg64_advance_r(state->rng, delta);
}

inline void entropy_init(aug_state* state)
{
    pcg128_t seeds[2];
    entropy_fill((void*) seeds, sizeof(seeds));
    seed(state, seeds[0], seeds[1]);
}

inline double random_double(aug_state* state)
{
    uint64_t rn;
    int32_t a, b;
    rn = random_uint64(state);
    a = rn >> 37;
    b = (rn & 0xFFFFFFFFLL) >> 6;
    return (a * 67108864.0 + b) / 9007199254740992.0;
}