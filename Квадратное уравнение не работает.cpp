#include <stdio.h>
#include  <math.h>

const double Pseudo0 = 1E-6;

int rootfinder (double a,double b,double c, double x1, double x2);


//-----------------------------------------------------------------------------


int main()
    {
    printf                           ("Hey there\n");
    printf ("Let's solve your quadratic equation\n");
    printf       ("Enter coefficients a, b and c\n");


    double a = 0, b = 0, c = 0;

    scanf ("%lg %lg %lg", &a, &b, &c);

    double x1 = 0, x2 = 0;
    int amount = rootfinder (a, b, c, &x1, &x2);

    switch (amount)
        {
        case  2: printf ("2 roots: %.2lg, and %.2lg\n", x1, x2);
                 break;
        case  1: printf ("Only 1 root: %.2lg", x1);
                 break;
        case  0: printf ("No roots");
                 break;
        case -1: printf ("Any number is a root");
                 break;
        }
    }


//-----------------------------------------------------------------------------


int rootfinder (double a, double b, double c, double *x1, double *x2)
    {
    double discr = b*b - 4*a*c;
    if ((a != 0) && (discr > Pseudo0))
        {
        *x1 = (-b + sqrt(discr))/2*a;
        *x2 = (-b - sqrt(discr))/2*a;
        return 2;
        }
        else if ((a != 0) && (discr <= Pseudo0) && (discr >= 0))
                {
                x1 = -b / (2*a);
                return 1;
                }
                if ((discr < 0) or ((b = 0) && (c != 0)))
                        {
                        return 0;
                        }
                        else if ((b != 0) && (c != 0))
                             {
                             x1 = - c / b;
                             return 1;
                             }
                             else return -1;
    }












