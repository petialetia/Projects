#include <stdio.h>
#include <math.h>
#include <assert.h>

//-----------------------------------------------------------------------------


const double Pseudo0 = 1E-6;
enum NRoots
    {
    InfinitRoots = -1,
    NoRoots,
    OneRoot,
    TwoRoots
    };



/*!

Finds roots, returns number of them
\param[in] a Coefficient a
\param[in] b Coefficient b
\param[in] c Coefficient c
\param[out] x1 First root
\param[out] x2 Second root
*/

NRoots findroots (double a, double b, double c, double* x1, double* x2);

int IsZero (double value);
void test_findroots (double* x1, double* x2);


//-----------------------------------------------------------------------------


int main()
    {
    printf                           ("Hey there\n");
    printf ("Let's solve your quadratic equation\n");
    printf       ("Enter coefficients a, b and c\n");


    double a = 0,
           b = 0,
           c = 0;


    scanf ("%lg %lg %lg", &a, &b, &c);


    double x1 = 0,
           x2 = 0;


    NRoots amount = findroots (a, b, c, &x1, &x2);

    switch (amount)
        {
        case  TwoRoots: printf ("2 roots: %.3lg and %.3lg\n", x1, x2);
                 break;
        case  OneRoot: printf           ("Only 1 root: %.3lg\n", x1);
                 break;
        case  NoRoots: printf                         ("No roots\n");
                 break;
        case  InfinitRoots: printf             ("Any number is a root\n");
                 break;
        }

    test_findroots (&x1, &x2);

    }


//-----------------------------------------------------------------------------


NRoots findroots (double a, double b, double c, double* x1, double* x2)
    {


    assert (x1 != nullptr);
    assert (x2 != nullptr);
    assert (isfinite(a));
    assert (isfinite(b));
    assert (isfinite(c));


    if (IsZero (a))
       {
       if (IsZero (b))
          {
          if (IsZero (c))
             return InfinitRoots;
          else
             return NoRoots;
          }
       else
          {
          *x1 = -c/b;
          return OneRoot;
          }
       }
    else
       {


       double discr = b*b - 4*a*c;


       if (discr > Pseudo0)
          {
          *x1 = (-b + sqrt(discr))/(2*a);
          *x2 = (-b - sqrt(discr))/(2*a);
          return TwoRoots;
          }
       else
          {
          if (IsZero (discr))
             {
             *x1 = -b/(2*a);
             return OneRoot;
             }
          else
             {
             return NoRoots;
             }
          }
       }
    }


//-----------------------------------------------------------------------------


int IsZero (double value)
    {
    if (fabs(value) > Pseudo0)
       {
       return 0;
       }
    else
       return 1;
    }


//-----------------------------------------------------------------------------


void test_findroots (double* x1, double* x2)
        {
        #define test(arg1, arg2)                                                                                                      \
                if (arg1 == arg2)                                                                                                     \
                   {                                                                                                                  \
                   printf ("Test #%d is OK\n", num);                                                                                  \
                   }                                                                                                                  \
                else                                                                                                                  \
                   {                                                                                                                  \
                   printf ("Test #%d FAILED, findroots (%.3lg, %.3lg, %.3lg, x1, x2) = %d, shoud be %d\n", num, at, bt, ct, res, exp);\
                   }
        double at = 0,
               bt = 0,
               ct = 0;
           int num = 1,
               res = findroots (at, bt, ct, x1, x2),
               exp = -1;

        test(res,exp)

            at = 1; bt = 0; ct = -4; num = 2; exp = 2;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 1; bt = 2; ct = 1; num = 3; exp = 1;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 1; bt = 2; ct = 2; num = 4; exp = 0;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 0; bt = 0; ct = 1; num = 5; exp = 0;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 0; bt = 2; ct = 1; num = 6; exp = 1;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 0.1; bt = 0.02; ct = -0.08; num = 7; exp = 2;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = 0.1; bt = 0.02; ct = -0.008; num = 8; exp = 2;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

            at = -1; bt = -2; ct = -8; num = 9; exp = 0;
            res = findroots (at, bt, ct, x1, x2);

        test(res,exp)

        #undef test

        }


//-----------------------------------------------------------------------------
