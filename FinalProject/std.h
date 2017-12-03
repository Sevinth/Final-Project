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

	bool operator==(Point2D v) {
		if (v.x == this->x && v.y == this->y)
			return true;
		else
			return false;
	}

	bool operator!=(Point2D v) {
		if (v.y != this->x && v.y != this -> y)
			return true;
		else
			return false;
	}

};


struct Velocity2D {
	double vx;
	double vy;

	Velocity2D(double _vx, double _vy) {
		vx = _vx;
		vy = _vy;
	}
	Velocity2D() {
		vx = 0;
		vy = 0;
	}

};


struct Particle {
	Point2D pos;
	Velocity2D vel;
	double mass;
	Particle(Point2D _pos, Velocity2D _vel, double _mass) {
		pos = _pos;
		vel = _vel;
		mass = _mass;
	}
	Particle(Point2D _pos, double _mass) {
		pos = _pos;
		vel = Velocity2D(0, 0);
		mass = _mass;
	}
	Particle() {
		pos = Point2D(0, 0);
		vel = Velocity2D(0, 0);
		mass = 1.0;
	}
};