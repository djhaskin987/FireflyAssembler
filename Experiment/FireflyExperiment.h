/*
 * FireflyExperiment.h
 *
 *  Created on: Nov 9, 2013
 *      Author: kyle
 */

#ifndef FIREFLYEXPERIMENT_H_
#define FIREFLYEXPERIMENT_H_

class Firefly {
private:
	double x;
	double y;

public:
	Firefly();
	std::string str();
	double intensity();
	void move(Firefly* other);

	double getX() const {
		return x;
	}

	void setX(double x) {
		this->x = x;
	}

	double getY() const {
		return y;
	}

	void setY(double y) {
		this->y = y;
	}
};



#endif /* FIREFLYEXPERIMENT_H_ */
