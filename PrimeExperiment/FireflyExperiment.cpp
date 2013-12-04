#include <iostream>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <limits>
#include "FireflyExperiment.h"

#define NUM_FIREFLIES 100
#define NUM_ITERATIONS 10000

using namespace std;

int bitSizeOf(unsigned long number)
{
    unsigned long mask = 1UL;
    int size = 0;
    while (mask < number)
    {
        mask <<= 1;
        size++;
    }
    return size;
}

double fitnessFunction(unsigned long number) {
    static long composite = 786011;
    unsigned long mod = composite % number;
    unsigned long dom = abs(number - mod);
    unsigned long min = mod < dom ? mod : dom;
    double exponent = ((double)min / (double)number)*((double)bitSizeOf(number)); // number of sqrt comp
    double result = ((double)number)*exp(-exponent);
    return result;
}

int hammingDistance(unsigned long a, unsigned long b)
{
    unsigned long X = a ^ b;
    unsigned long stop = sizeof(unsigned long) * 8U;
    int size = 0;
    for (unsigned long  i = 0; i < stop; i++)
    {
        size += (X & 1UL);
        X >>= 1UL;
    }
    return size;
}

Firefly::Firefly() : eng(), dist(0UL,10UL), number() {
    number = (unsigned long) rand()%((unsigned long)887);
}

string Firefly::str() {
	stringstream ss (stringstream::in | stringstream::out);
    ss << number << " " << rawFitness();
	return ss.str();
}
bool Firefly::operator == (const Firefly & other)
{
    return number == other.number;
}
bool Firefly::operator != (const Firefly & other)
{
    return number != other.number;
}

double Firefly::rawFitness() {
    return fitnessFunction(number);
}

double Firefly::intensity(Firefly* other) {
	double distance = (double)hammingDistance(this->number, other->number);
	return rawFitness() * exp(-distance);
}

void Firefly::move(Firefly* other) {
	int movement = 1;
    for (int i = 0; i < movement; i++)
    {
        unsigned long bitPosition = dist(eng);
        unsigned long mask = 1UL;
        mask <<= bitPosition;
        this->number = (this->number & ~mask) | (other->number & mask);
    }
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

	for ( int i = 0; i < NUM_FIREFLIES ; i++ )
		fireflies.push_back(new Firefly());

	print_fireflies(fireflies);

    vector<Firefly*>::iterator maxIntensityFly;
    double maxIntensity;
    int consensus = 0;
    int neededEqualities = ((NUM_FIREFLIES * NUM_FIREFLIES) - NUM_FIREFLIES) * 2 / 3;
	for(int round = 0; round < NUM_ITERATIONS ; round++) {
        consensus = 0;
		for(vector<Firefly*>::iterator f1 = fireflies.begin();
                f1 != fireflies.end(); ++f1) {
            maxIntensity = -numeric_limits<double>::infinity();
			for(vector<Firefly*>::iterator f2 = fireflies.begin(); f2 != fireflies.end(); ++f2) {
                if (f1 != f2) {
                    double currentIntensity = (*f2)->intensity(*f1);
                    if (currentIntensity > maxIntensity)
                    {
                        maxIntensity = currentIntensity;
                        maxIntensityFly = f2;
                    }
                    if (**f1 == **f2)
                    {
                        consensus++;
                    }
                }
            }
            (*f1)->move(*maxIntensityFly);
        }
        if (consensus > neededEqualities)
        {
            cout << "Consensus: " << consensus << endl;
            break;
        }
	}

	print_fireflies(fireflies);
	return 0;
}
