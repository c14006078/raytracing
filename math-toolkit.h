#ifndef __RAY_MATH_TOOLKIT_H
#define __RAY_MATH_TOOLKIT_H

#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <emmintrin.h>

static inline
void normalize(double *v)
{
    double d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    assert(d != 0.0 && "Error calculating normal");

    v[0] /= d;
    v[1] /= d;
    v[2] /= d;
}

static inline
double length(const double *v)
{
    return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static inline
void add_vector(const double *a, const double *b, double *out)
{
    for (int i = 0; i < 3; i++)
        out[i] = a[i] + b[i];
}

static inline
void subtract_vector(const double *a, const double *b, double *out)
{
    for (int i = 0; i < 3; i++)
        out[i] = a[i] - b[i];
}

static inline
void multiply_vectors(const double *a, const double *b, double *out)
{
    for (int i = 0; i < 3; i++)
        out[i] = a[i] * b[i];
}

static inline
void multiply_vector(const double *a, double b, double *out)
{
    for (int i = 0; i < 3; i++)
        out[i] = a[i] * b;
}

static inline
void cross_product(const double *v1, const double *v2, double *out)
{
    out[0] = v1[1] * v2[2] - v1[2] * v2[1];
    out[1] = v1[2] * v2[0] - v1[0] * v2[2];
    out[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static inline
double dot_product(const double *v1, const double *v2)
{
    double * dp = (double *) malloc( 2 * sizeof( double));
    __m128d m128_v1 = _mm_set_pd( v1[0], v1[1]);
    __m128d m128_v2 = _mm_set_pd( v2[0], v2[1]);
    __m128d m128_d1 = _mm_set_pd( v1[2], 0);
    __m128d m128_d2 = _mm_set_pd( v2[2], 0);
    __m128d m128_mul_v1v2 = _mm_mul_pd( m128_v1, m128_v2);
    __m128d m128_mul_d1d2 = _mm_mul_pd( m128_d1, m128_d2);
    __m128d m128_add = _mm_add_pd( m128_mul_v1v2, m128_mul_d1d2);
    _mm_store_pd( dp, m128_add);

    return *dp + *(dp+1);
}

static inline
void scalar_triple_product(const double *u, const double *v, const double *w,
                           double *out)
{
    cross_product(v, w, out);
    multiply_vectors(u, out, out);
}

static inline
double scalar_triple(const double *u, const double *v, const double *w)
{
    double tmp[3];
    cross_product(w, u, tmp);
    return dot_product(v, tmp);
}

#endif
