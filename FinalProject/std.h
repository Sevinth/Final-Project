#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <memory>
#include <vector>

//////////////////////////////////////////////////
struct Point2D {
	double x;
	double y;

	Point2D(double _x, double _y) {
		x = _x;
		y = _y;
	}
	Point2D() {
		x = 0;
		y = 0;
	}

};


struct Particle {
	Point2D pos;
	double mass;
	Particle(Point2D _pos, double _mass) {
		pos = _pos;
		mass = _mass;
	}
	Particle() {
		pos = Point2D(0, 0);
		mass = 1.0;
	}
};