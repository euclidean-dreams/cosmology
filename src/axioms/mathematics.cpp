#include "mathematics.h"

namespace cosmology {

class xorshiro {
/*  Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

// thanks friends! <3
public:
    static inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    static uint64_t next(void) {
        const uint64_t result = rotl(world_seed[0] + world_seed[3], 23) + world_seed[0];

        const uint64_t t = world_seed[1] << 17;

        world_seed[2] ^= world_seed[0];
        world_seed[3] ^= world_seed[1];
        world_seed[1] ^= world_seed[2];
        world_seed[0] ^= world_seed[3];

        world_seed[2] ^= t;

        world_seed[3] = rotl(world_seed[3], 45);

        return result;
    }

/* This is the jump function for the generator. It is equivalent
   to 2^128 calls to next(); it can be used to generate 2^128
   non-overlapping subsequences for parallel computations. */

    static void jump(void) {
        static const uint64_t JUMP[] = {0x180ec6d33cfd0aba, 0xd5a61266f0c9392c, 0xa9582618e03fc9aa, 0x39abdc4529b1661c};

        uint64_t s0 = 0;
        uint64_t s1 = 0;
        uint64_t s2 = 0;
        uint64_t s3 = 0;
        for (int i = 0; i < sizeof JUMP / sizeof *JUMP; i++)
            for (int b = 0; b < 64; b++) {
                if (JUMP[i] & UINT64_C(1) << b) {
                    s0 ^= world_seed[0];
                    s1 ^= world_seed[1];
                    s2 ^= world_seed[2];
                    s3 ^= world_seed[3];
                }
                next();
            }

        world_seed[0] = s0;
        world_seed[1] = s1;
        world_seed[2] = s2;
        world_seed[3] = s3;
    }

/* This is the long-jump function for the generator. It is equivalent to
   2^192 calls to next(); it can be used to generate 2^64 starting points,
   from each of which jump() will generate 2^64 non-overlapping
   subsequences for parallel distributed computations. */

    static void long_jump(void) {
        static const uint64_t LONG_JUMP[] = {0x76e15d3efefdcbbf, 0xc5004e441c522fb3, 0x77710069854ee241,
                                             0x39109bb02acbe635};

        uint64_t s0 = 0;
        uint64_t s1 = 0;
        uint64_t s2 = 0;
        uint64_t s3 = 0;
        for (int i = 0; i < sizeof LONG_JUMP / sizeof *LONG_JUMP; i++)
            for (int b = 0; b < 64; b++) {
                if (LONG_JUMP[i] & UINT64_C(1) << b) {
                    s0 ^= world_seed[0];
                    s1 ^= world_seed[1];
                    s2 ^= world_seed[2];
                    s3 ^= world_seed[3];
                }
                next();
            }

        world_seed[0] = s0;
        world_seed[1] = s1;
        world_seed[2] = s2;
        world_seed[3] = s3;
    }
};

int Randomizer::generate_number(int exclusive_max) {
    if (exclusive_max <= 0) {
        return 0;
    }
    return xorshiro::next() % exclusive_max;
}

int Randomizer::generate(int exclusive_max) {
    return generate_number(exclusive_max);
}

float Randomizer::generate_proportion() {
    return scast<float>(generate_number(10000)) / 9999;
}

float Randomizer::generate_sign() {
    auto comparable = generate_proportion() * 20 - 10;
    if (comparable > 0) {
        return 1;
    } else {
        return -1;
    }
}

}
