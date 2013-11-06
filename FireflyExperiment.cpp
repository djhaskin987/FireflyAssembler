#include <iostream>
#include <cstdlib>
#include <cmath>

double fitnessFunction(double x, double y)
{
    double x2 = x*x;
    double y2 = y*y;
    double sum = x2 + y2;
    double f = sin(10.0*sum)/10.0*exp(-sum/100.0)/sum;
    return f;
}

double euclideanDistance(double x, double y)
{
    return sqrt(x*x+y*y);
}

int main(int argc, char *argv[])
{
    // maybe use 5 fireflies for a toy example,
    // or a more complicated fitness function and more fireflies,
    // but something to kind of give a "proof-of-concept" for the firefly algorithm
    // before we code it up.
    return 0;
}
