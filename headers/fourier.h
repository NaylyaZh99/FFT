#ifndef __FOURIER_H__
#define __FOURIER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <cstdlib>

using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::istream;

class Complex {
	friend ostream &operator<< (ostream &, const Complex &);
	friend istream &operator>> (istream &, Complex &);
public:
	double _re, _im;
	Complex();
	Complex(double re, double im = 0);
	double re() const;
	double amp() const;
	double phase() const;
	const Complex& operator=(const Complex &z);
	Complex operator+(const Complex &z) const;
	Complex operator-(const Complex &z) const;
	Complex operator*(double x) const;
	Complex operator/(double x) const;
	Complex operator^(double alpha) const;
};

std::vector<Complex> f2c(const std::vector<Complex> &f, int N);
std::vector<Complex> c2f(std::vector<Complex> c, int N);
std::vector<Complex> parse(std::vector<char> buf);
int maxPowerOf2(int N);
void fft(std::vector<Complex> &vec_f, bool invert);

#endif
