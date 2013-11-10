#include <iostream>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include "FireflyExperiment.h"

using namespace std;

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

Firefly::Firefly() {
	x = (float)rand()/((float)RAND_MAX/10);
	y = (float)rand()/((float)RAND_MAX/10);
}

string Firefly::str() {
	stringstream ss (stringstream::in | stringstream::out);
	ss << "{" << x << ", " << y << "}";
	return ss.str();
}

int main(int argc, char *argv[])
{
	srand(time(0));

	Firefly f = Firefly();
	cout << f.str() << endl;

	Firefly fireflies [10];
	// maybe use 5 fireflies for a toy example,
    // or a more complicated fitness function and more fireflies,
    // but something to kind of give a "proof-of-concept" for the firefly algorithm
    // before we code it up.
    return 0;
}
