#include <iostream>
#include "headers/fourier.h"

void oberton(std::vector<Complex> &func, double coef) {
	double max = fabs(func[1]._re);
	for (int i = 2; i < (int)func.size(); i++) {
		max = fmax(max, fabs(func[i]._re));
	}
	for (int i = 1; i < (int)func.size() / 2; i++) {
		if (fabs(func[i]._re) < max * coef) {
			func[i] = Complex(0, 0);
		}
	}
	for (int i = func.size() / 2 + 1; i < (int)func.size(); ++i) {
		func[i] = func[i - func.size() / 2];
	}
}

int main(int argc, char **argv){
	std::ifstream input(argv[1], std::ios::binary);
	std::ofstream output(argv[2]);
	std::istreambuf_iterator<char> start(input);
	std::istreambuf_iterator<char> end;
	std::vector<char> buf(start, end);
	for (int i = 0; i < 36; i++) {
		output << buf[i];
	}
	double sec = std::atof(argv[3]), coef = std::atof(argv[4]);
	int step = 44100 * sec, sizeOfPower2 = maxPowerOf2(step), index = 0, N;
	std::vector<Complex> function = parse(buf), newFunc;
	newFunc.resize(sizeOfPower2);
	N = function.size();
	while (N > 0) {
		for (int i = 0; i < sizeOfPower2; i++) {
			newFunc[i] = Complex(0, 0);
		}
		for (int i = 0; i < step && step * index + i < function.size(); i++) {
			newFunc[i] = function[step * index + i];
		}
		fft(newFunc, false);
		oberton(newFunc, coef);
		fft(newFunc, true);
		for (int i = 0; i < step; i++) {	
			output << (char) newFunc[i]._re;
		}
		N -= step;
		++index;
	}
	output.close();
	return 0;
}
