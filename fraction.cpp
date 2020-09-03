#ifndef FRACTION_H
#define FRACTION_H

#include <algorithm>

using namespace std;

using Integer = long long;

class Fraction {
	Integer numerator;
	Integer denominator;
public:
	Fraction(Integer n, Integer d);
public:
	Integer num();
	Integer den();
	double toreal();
public:
	Fraction negate();
	Fraction reciprocal();
	Fraction simplify();
};

Integer gcd(Integer a, Integer b) {
	if(a < b) swap(a, b);
	if(b == 0) return a;
	return gcd(b, a%b);
}

Integer lcm(Integer a, Integer b) {
	return(a * b) / gcd(a, b);
}

Fraction::Fraction(Integer n, Integer d) : numerator{ n }, denominator{ d } {}

Integer Fraction::num() {
	return numerator;
}

Integer Fraction::den() {
	return denominator;
}

double Fraction::toreal() {
	return static_cast<double>(numerator) / denominator;
}

Fraction Fraction::negate() {
	return Fraction{ -1 * numerator, denominator };
}

Fraction Fraction::reciprocal() {
	return Fraction{ denominator, numerator };	
}

Fraction Fraction::simplify() {
	return Fraction{numerator / gcd (numerator, denominator), denominator / gcd (numerator, denominator)};
}

bool operator == (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() == lhs.den() * rhs.num();
}

bool operator != (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() != lhs.den() * rhs.num();
}

bool operator < (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() < lhs.den() * rhs.num();
}

bool operator <= (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() <= lhs.den() * rhs.num();
}

bool operator > (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() > rhs.num() * lhs.den();
}

bool operator >= (Fraction lhs, Fraction rhs) {
	return lhs.num() * rhs.den() >= rhs.num() * lhs.den();
}

#endif

