#include <iostream>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "FireflyExperiment.h"

#define NUM_FIREFLIES 10;
#define NUM_ITERATIONS 10;

using namespace std;

double fitnessFunction(double x, double y) {
    double x2 = x*x;
    double y2 = y*y;
    double sum = sqrt(x2 + y2);
    double f = sin(10.0*sum)/10.0*exp(-sum/100.0)/sum;
    if( f < 0 )
    	return 0;
    return f;
}

double euclideanDistance(Firefly* f1, Firefly* f2) {
	double x = f1->getX()-f2->getX();
	double y = f1->getY()-f2->getY();
    return sqrt(x*x+y*y);
}

Firefly::Firefly() {
	x = (float)rand()/((float)RAND_MAX/10)-5;
	y = (float)rand()/((float)RAND_MAX/10)-5;
}

string Firefly::str() {
	stringstream ss (stringstream::in | stringstream::out);
	ss << "{" << x << ", " << y << "} (" << intensity() << ")";
	return ss.str();
}

double Firefly::intensity() {
	return fitnessFunction(x, y);
}

void Firefly::move(Firefly* other) {
	double distance = euclideanDistance(this, other);
	double movement = other->intensity()/distance;
	x = (other->getX() - x)/distance;
	y = (other->getY() - y)/distance;
}

void print_fireflies(vector<Firefly*> fireflies) {
	for(vector<Firefly*>::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1)
		cout << (*f1)->str() << endl;
}

int main(int argc, char *argv[])
{
	srand(time(0));

	Firefly f = Firefly();
	cout << f.str() << endl;
	std::vector<Firefly*> fireflies;

	for ( int i = 0; i < 10; i++ )
		fireflies.push_back(new Firefly());

	print_fireflies(fireflies);

	for(int round = 0; round < 100; round++) {
		for(vector<Firefly*>::iterator f1 = fireflies.begin(); f1 != fireflies.end(); ++f1) {
			for(vector<Firefly*>::iterator f2 = fireflies.begin(); f2 != fireflies.end(); ++f2) {
				if (f1 != f2) {
					(*f1)->move(*f2);
				}
			}
		}
	}

	print_fireflies(fireflies);
	return 0;
}
