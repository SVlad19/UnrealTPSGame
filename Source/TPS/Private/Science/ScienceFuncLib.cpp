// My game copyright

#include "Science/ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogScience, All, All);

int32 UScienceFuncLib::Fibonacci(int32 Value)
{
    if (Value < 0)
    {
        UE_LOG(LogScience, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
    }

    return Value <= 1 ? Value : Fibonacci(Value - 1) + Fibonacci(Value - 2);
}

int32 UScienceFuncLib::Factorial(int32 Value)
{
    if (Value < 0)
    {
        return -1;
    }

    // if (Value == 0 || Value == 1)
    //{
    //     return 1;
    // }

    // return Factorial(Value - 1) * Value;

    int32 Fac = 1;
    for (int i = 1; i <= Value; ++i)
    {
        Fac *= i;
    }

    return Fac;
}