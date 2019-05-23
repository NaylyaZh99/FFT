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
	double _re, _im;
	friend ostream &operator<< (ostream &, const Complex &);
	friend istream &operator>> (istream &, Complex &);
public:
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
//std::vector<Complex> parse(std::string str);
std::vector<Complex> parse(std::vector<char> buf, int chunk_size);
void fft(std::vector<Complex> &vec_f, bool invert);

int main(void) {
	int chunk_start = 36;
	std::ifstream input("07070057.wav", std::ios::binary);
	std::istreambuf_iterator<char> start(input);
	std::istreambuf_iterator<char> end;
	std::vector<char> buf(start, end);
	
	std::string chunk_name(buf.begin() + chunk_start, buf.begin() + chunk_start + 4);
	chunk_start += 4;
	int chunk_size = *(int *)(buf.data() + chunk_start);
	chunk_start += 4;
	while (chunk_name != "data") {
		chunk_start += chunk_size;
		chunk_name = std::string(buf.begin() + chunk_start, buf.begin() + chunk_start + 4);
		chunk_start += 4;
		chunk_size = *(int *)(buf.data() + chunk_start);
		chunk_start += 4;
	}
	std::vector<Complex> function = parse(buf, chunk_size);
	int shift = 5000, size = 4410;
	std::vector<Complex> function_part(function.begin() + shift, function.begin() + shift + size);
	std::ofstream file_function;
	file_function.open("f.txt");
	std::ofstream file_spectr;
	file_spectr.open("s.txt");
	
	std::vector<Complex> spectr = f2c(function_part, size);
	
	//~ for (int i = 0; i < size; i++) {
		//~ double time = (i + shift) / 44100.0;
		//~ file_function << time << ' ' << function_part[i].re() << '\n';
	//~ }
	//~ for (int i = 0; i < size; i++) {
		//~ double frequency = 44100.0 / size * i;
		//~ file_spectr << frequency << ' ' << spectr[i].amp() << '\n';
	//~ }
	
	for (int i = 0; i < size; i++) {
		file_function << i + shift << ' ' << function_part[i].re() << '\n';
	}

	for (int i = 0; i < size; i++) {
		file_spectr << i << ' ' << spectr[i].amp() << '\n';
	}
	file_function.close();
	file_spectr.close();
	
	
		
	//~ Complex z1, z2;
	//~ double x = 2;
	//~ cin >> z1 >> z2;
	//~ z1 = z1 * x;
	//~ cout << z1 << endl;
	//~ z1 = z1 / 2;
	//~ cout << z1 << endl;
	//~ z1 = z1 + z2;
	//~ cout << z1 << endl;
	//~ int N;
	//~ std::vector<Complex> f, c;
	//~ cin >> N;
	//~ f.resize(N);
	//~ c.resize(N);
	//~ for (int i = 0; i < N; i++) {
		//~ cin >> 
	//~ }
	
	return 0;
}

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

std::vector<Complex> parse(std::vector<char> buf, int chunk_size) {
//std::vector<Complex> parse(std::string str) {
	std::vector<Complex> function;
	for (int i = 0; i < chunk_size; i += 4) {
		short value	= *(short *)(buf.data() + i);
		function.push_back(value);
	}
	return function;
	//~ int N, M;
	//~ std::vector<Complex> vec;
	//~ N = *(int *)(str.c_str() + 40);
	//~ M = *(short *)(str.c_str() + 32);
	//~ for (int i = 0; i < N; i++) {
		//~ vec.push_back(str[44 + i]);
	//~ }
	//~ return vec;
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
