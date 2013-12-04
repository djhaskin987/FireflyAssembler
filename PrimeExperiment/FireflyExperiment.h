/*
 * FireflyExperiment.h
 *
 *  Created on: Nov 9, 2013
 *      Author: kyle
 */

#ifndef FIREFLYEXPERIMENT_H_
#define FIREFLYEXPERIMENT_H_
#include <random>

class Firefly {
private:
    std::mt19937 eng;
    std::uniform_int_distribution<unsigned long> dist;
    unsigned long number;

public:
    bool operator == (const Firefly & other);
    bool operator != (const Firefly & other);
	Firefly();
	std::string str();
    double rawFitness();
	double intensity(Firefly* other);
	void move(Firefly* other);
};



#endif /* FIREFLYEXPERIMENT_H_ */
