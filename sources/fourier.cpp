#include "../headers/fourier.h"

//~ using std::cout;
//~ using std::cin;
//~ using std::endl;
//~ using std::ostream;
//~ using std::istream;

std::vector<Complex> f2c(const std::vector<Complex> &f, int N) {
	std::vector<Complex> c;
	c.resize(N);
	for (int k = 0; k < N; k++) {
		Complex sum(0,0);
		for (int j = 0; j < N; j++) {
			sum = sum + (f[j] ^ ((-2 * M_PI * j * k) / N));
		}
		c[k] = sum / N;
	}
	return c;
}

std::vector<Complex> c2f(std::vector<Complex> c, int N) {
	std::vector<Complex> f;
	f.resize(N);
	for (int k = 0; k < N; k++) {
		Complex sum(0,0);
		for (int j = 0; j < N; j++) {
			sum = sum + (c[j] ^ ((2 * M_PI * j * k) / N));
		}
		f[k] = sum;
	}
	return f;
}

std::vector<Complex> parse(std::vector<char> buf) {
	std::vector<Complex> function;
	for (int i = 0; i < buf.size(); i += 4) {
		short value	= *(short *)(buf.data() + i);
		function.push_back(value);
	}
	return function;
}

//~ std::vector<Complex> parse(std::vector<char> buf) {
	//~ std::vector<Complex> vec;
	//~ for (int i = 44; i < buf.size(); i++) {
		//~ vec.push_back(Complex((double)buf[i]));
	//~ }
	//~ return vec;
//~ }


int maxPowerOf2(int n) {
	int q = 1;
	while (n > q) {
		q *= 2;
	}
	return q;
}

void fft(std::vector<Complex> &vec_f, bool invert) {
	int n = (int) vec_f.size();
	if (n == 1) {
		return;
	}
	std::vector<Complex> even_vec(n / 2), uneven_vec(n / 2);
	for (int i = 0, j = 0; i < n; i += 2, ++j) {
		even_vec[j] = vec_f[i];
		uneven_vec[j] = vec_f[i + 1];
	}
	fft(even_vec, invert);
	fft(uneven_vec, invert);
	double alpha = 2 * M_PI / n * (invert ? -1: 1);
	for (int i = 0; i < n / 2; ++i) {
		vec_f[i] = even_vec[i] + (uneven_vec[i] ^ (alpha * i));
		vec_f[n / 2 + i] = even_vec[i] - (uneven_vec[i] ^ (alpha * i));
		if (invert) {
			vec_f[i] = vec_f[i] / 2, vec_f[n / 2 + i] = vec_f[n / 2 + i] / 2;
		}
	}
}

ostream &operator<< (ostream &os, const Complex &z) {
	os << std::fixed << std::setprecision(3) << z._re << '+' << std::fixed << std::setprecision(3) << z._im << 'i';
	return os;
}

istream &operator>> (istream &is, Complex &z) {
	is >> z._re >> z._im;
	return is;
}

Complex::Complex() {
	_re = 0;
	_im = 0;
}

Complex::Complex(double re, double im) {
	_re = re;
	_im = im;
}

double Complex::re() const {
	return _re;
}

double Complex::amp() const {
	return sqrt(_re * _re + _im * _im);
}

double Complex::phase() const {
	return atan2(_im, _re);
}

const Complex& Complex::operator=(const Complex &z) {
	_re = z._re;
	_im = z._im;
	return *this;
}

Complex Complex::operator+(const Complex &z) const {
	Complex ans;
	ans._re = _re + z._re;
	ans._im = _im + z._im;
	return ans;
}

Complex Complex::operator-(const Complex &z) const {
	Complex ans;
	ans._re = _re - z._re;
	ans._im = _im - z._im;
	return ans;
}

Complex Complex::operator*(double x) const {
	Complex ans;
	ans._re = x * _re;
	ans._im = x * _im;
	return ans;
}

Complex Complex::operator/(double x) const {
	Complex ans;
	ans._re = _re / x;
	ans._im = _im / x;
	return ans;
}

Complex Complex::operator^(double alpha) const {
	Complex ans;
	ans._re = _re * cos(alpha) - _im * sin(alpha);
	ans._im = _im * cos(alpha) + _re * sin(alpha);
	return ans;
}
