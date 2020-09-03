#define _USE_MATH_DEFINES

#include "catch.hpp"

#include <vector> // provides standard library std::vector<T>
#include <cmath> // provides std::sqrt() - square root

const double Pi = M_PI; // use standard library definition of Pi
const double Tau = 2 * Pi;

// calculate cylinder volume
double cylinder_volume(double circumference, double height) {
	return (circumference * circumference * height) / (4 * Pi);
}

// calculate jug volume - assuming it is made of 4 cylinders
double ideal_jug_volume() {
	double coin1 = cylinder_volume(117.0, 25.0);
	double coin2 = cylinder_volume(117.0, 25.0);
	double coin3 = cylinder_volume(117.0, 25.0);
	double coin4 = cylinder_volume(117.0, 25.0);
	return coin1 + coin2 + coin3 + coin4;
}

// calculate the volume of the measured jug
double actual_jug_volume() {
	double coin1 = cylinder_volume(35.3, 1.8);
	double coin2 = cylinder_volume(34.2, 2.1);
	double coin3 = cylinder_volume(32.8, 2.5);
	double coin4 = cylinder_volume(30.7, 3.2);
	return coin1 + coin2 + coin3 + coin4;
}

// create a Cylinder entity to ensure height and circumference are handled together
struct Cylinder {
	double circumference;
	double height;
};

// create an ideal cylinder
const Cylinder Ideal{ 117.0, 25.0 };

// square a number
double pow2(double v) {
	return v * v;
}

// calculate difference in radii
double cylinder_radius_difference(const Cylinder& current, const Cylinder& next) {
	return (current.circumference / Tau) - (next.circumference / Tau);
}

// adjust the height using Pythagoras' theorem
Cylinder cylinder_adjust_height(const Cylinder& current, const Cylinder& next) {
	double delta_r = cylinder_radius_difference(current, next);
	return Cylinder{ current.circumference, std::sqrt(pow2(current.height) - pow2(delta_r)) };
}

// adjust the radius for wall thickness
double adjust_radius(double c) {
	return c / Tau - 0.5;
}

// adjust the circumference by using the average circumference for a cylinder
Cylinder cylinder_adjust_circumference(const Cylinder& current, const Cylinder& next) {
	double c = (current.circumference + next.circumference) / 2;
	return Cylinder{ c, current.height };
}

// calculate the cylinder volume after adjusting the height, circumference and the radius
double cylinder_volume_adjusted(const Cylinder& current, const Cylinder& next) {
	Cylinder c = cylinder_adjust_height(current, next);
	c = cylinder_adjust_circumference(c, next);
	return cylinder_volume(adjust_radius(c.circumference) * Tau, c.height);
}

// create 1D matrix of cylinders
using Cylinder_matrix = std::vector<Cylinder>;

const Cylinder_matrix Actual_jug = {
	{ 35.3, 1.8 },
	{ 34.2, 2.1 },
	{ 32.8, 2.5 },
	{ 30.7, 3.2 }, /* 3.7 - 0.5 for thickness of base */
	{ 28.7, 0 }
};

// calculate the volume of a jug
// the second parameter allows us to control how the adjustment is done (full or part)
double jug_volume(const Cylinder_matrix& cylinders) {
	double sum = 0.0;
	auto last = cylinders.size() - 1;			// determine the last cylinder from the size of the 1d matrix
	for (decltype(last) i = 0; i < last; ++i)	// decltype() ensures we use the same data type as for 'last' for: i < last
		sum += cylinder_volume_adjusted(cylinders[i], cylinders[i + 1]);
	return sum;
}

// slice a cylinder into count_slices slices by creating a 1D matrix of slices (Cylinders)
// we can then use jug_volume to calculate the volume of the sliced cylinder
Cylinder_matrix cylinder_sliced(const Cylinder& current, const Cylinder& next, int count_slices) {
	double r = current.circumference / Tau;
	double delta_r = cylinder_radius_difference(current, next) / count_slices;
	double h = current.height / count_slices;
	Cylinder_matrix slices{};
	for (int i = 0; i < count_slices; ++i)
		slices.push_back(Cylinder{ (r - (i * delta_r)) * Tau, h });
	slices.push_back(Cylinder{ next.circumference, 0.0 });
	return slices;
}

// calculate the jug volume using sliced cylinders
double jug_volume_sliced(const Cylinder_matrix& cylinders, int count_slices) {
	double sum = 0.0;
	auto last = cylinders.size() - 1;
	for (decltype(last) i = 0; i < last; ++i)
		sum += jug_volume(cylinder_sliced(cylinders[i], cylinders[i + 1], count_slices));
	return sum;
}

TEST_CASE("ideal_jug_volume & actual_jug_volume") {
	REQUIRE(Approx(4 * 27233.42) == ideal_jug_volume());
	REQUIRE(Approx(568.0) == actual_jug_volume());
}

TEST_CASE("cylinder_volume") {
	REQUIRE(0.0 == cylinder_volume(0.0, 0.0));
	REQUIRE(Approx(3.8197218) == cylinder_volume(4.0, 3.0));
	REQUIRE(Approx(27233.42) == cylinder_volume(117.0, 25.0));
}

TEST_CASE("cylinder radius difference") {
	REQUIRE(Approx(3.0) == cylinder_radius_difference(Cylinder{ 10 * Tau, 0.0 }, Cylinder{ 7 * Tau, 0.0 }));
}

TEST_CASE("cylinder_adjust_height") {
	REQUIRE(Approx(25.0) == cylinder_adjust_height(Ideal, Ideal).height);
	REQUIRE(Approx(117.0) == cylinder_adjust_height(Ideal, Ideal).circumference);
	REQUIRE(Approx(1.79147) == cylinder_adjust_height(Actual_jug[0], Actual_jug[1]).height);
	REQUIRE(Approx(35.3) == cylinder_adjust_height(Actual_jug[0], Actual_jug[1]).circumference);
}

TEST_CASE("jug_volume") {
	Cylinder_matrix cylinders{ 5, Ideal };
	cylinders.back().height = 0.0;
	REQUIRE(Approx(103162.141) == jug_volume(cylinders));
	REQUIRE(Approx(568.0) == jug_volume(Actual_jug));
}

TEST_CASE("cylinder_sliced - Ideal Cylinder") {
	Cylinder expected{ 117.0, 25.0 };
	Cylinder_matrix actual = cylinder_sliced(Ideal, Ideal, 1);
	REQUIRE(2 == actual.size());
	REQUIRE(Approx(expected.circumference) == actual[0].circumference);
	REQUIRE(Approx(expected.height) == actual[0].height);
	REQUIRE(Approx(expected.circumference) == actual[1].circumference);
	REQUIRE(0.0 == actual[1].height);
	REQUIRE(Approx(25790.535) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Ideal Jug - 1 slice") {
	Cylinder current{ 10.5 * Tau, 5.0 };
	Cylinder next{ 7.5 * Tau, 0.0 };
	Cylinder_matrix actual = cylinder_sliced(current, next, 1);
	REQUIRE(2 == actual.size());
	REQUIRE(Approx(current.circumference) == actual[0].circumference);
	REQUIRE(Approx(current.height) == actual[0].height);
	REQUIRE(Approx(next.circumference) == actual[1].circumference);
	REQUIRE(0.0 == actual[1].height);
	double expected = cylinder_volume_adjusted(current, next);
	REQUIRE(Approx(expected) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Ideal Jug - 100 slices") {
	int n_slices = 100;
	Cylinder current{ 10.5 * Tau, 5.0 };
	Cylinder next{ 7.5 * Tau, 0.0 };
	Cylinder_matrix actual =  cylinder_sliced(current, next, n_slices);
	REQUIRE(n_slices + 1 == actual.size());
	REQUIRE(Approx(3.0 / n_slices * Tau) == actual[n_slices - 2].circumference - actual[n_slices - 1].circumference);
	REQUIRE(actual[n_slices - 2].circumference > actual[n_slices - 1].circumference);
	REQUIRE(actual[n_slices - 1].circumference > actual[n_slices].circumference);
	REQUIRE(Approx(5.0 / n_slices) == actual[n_slices - 1].height);
	REQUIRE(actual[n_slices - 2].height == actual[n_slices - 1].height);
	double expected = cylinder_volume_adjusted(current, next);
	REQUIRE(expected == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Actual_jug[1] - 1 slice") {
	Cylinder_matrix actual = cylinder_sliced(Actual_jug[0], Actual_jug[1], 1);
	REQUIRE(2 == actual.size());
	REQUIRE(Approx(35.3) == actual[0].circumference);
	REQUIRE(Approx(1.8) == actual[0].height);
	REQUIRE(Approx(34.2) == actual[1].circumference);
	REQUIRE(0.0 == actual[1].height);
	double expected = cylinder_volume_adjusted(Actual_jug[0], Actual_jug[1]);
	REQUIRE(Approx(expected) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Actual_jug[1] - 100 slices") {
	Cylinder_matrix actual = cylinder_sliced(Actual_jug[0], Actual_jug[1], 100);
	REQUIRE(101 == actual.size());
	double expected = cylinder_volume_adjusted(Actual_jug[0], Actual_jug[1]);
	REQUIRE(Approx(568.0 / 4) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Actual_jug[2] - 100 slices") {
	Cylinder_matrix actual = cylinder_sliced(Actual_jug[1], Actual_jug[2], 100);
	double expected = cylinder_volume_adjusted(Actual_jug[1], Actual_jug[2]);
	REQUIRE(101 == actual.size());
	REQUIRE(Approx(568.0 / 4) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Actual_jug[3] - 100 slices") {
	Cylinder_matrix actual = cylinder_sliced(Actual_jug[2], Actual_jug[3], 100);
	double expected = cylinder_volume_adjusted(Actual_jug[2], Actual_jug[3]);
	REQUIRE(101 == actual.size());
	REQUIRE(Approx(568.0 / 4) == jug_volume(actual));
}

TEST_CASE("cylinder_sliced - Actual_jug[4] - 100 slices") {
	Cylinder_matrix actual = cylinder_sliced(Actual_jug[3], Actual_jug[4], 100);
	double expected = cylinder_volume_adjusted(Actual_jug[3], Actual_jug[4]);
	REQUIRE(101 == actual.size());
	REQUIRE(Approx(568.0 / 4) == jug_volume(actual));
}

TEST_CASE("jug_volume_sliced") {
	Cylinder_matrix cylinders{ 5, Ideal };
	cylinders.back().height = 0;
	REQUIRE(Approx(103162.141) == jug_volume_sliced(cylinders, 1));
	REQUIRE(Approx(103162.141) == jug_volume_sliced(cylinders, 100));
	REQUIRE(Approx(635.665) == jug_volume_sliced(Actual_jug, 1));
	REQUIRE(Approx(568.0) == jug_volume_sliced(Actual_jug, 100));
}

