#include<Vector2D.h>
#include <math.h>
#include <assert.h>
#include <unordered_map>
#include <iostream>
#include <ttmath.h>
#include <ContinuedFractions.h>
//returnerar regular continued fraction för rooten ur n, på formen [a0|x1,x2,x3...] som sparat i listan
std::vector<int> ContinuedFractionOfIntegerRoot(int n);
Vector2D ContinuedFactorEvaluator(std::vector<int> Fractionen, int AntaletIterations);
std::vector<ttmath::Int<CFINTSIZE>> ContinuedFactorEvaluatorBigInt(std::vector<int> Fractionen, int AntaletIterations);