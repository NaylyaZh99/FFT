#include <iostream>
#include "headers/fourier.h"

void change(std::vector<Complex> &func, int shift) {
	std::vector<Complex> tmp;
	tmp.resize(func.size(), Complex(0, 0));
	for (int i = 1; i * shift < func.size() && i < func.size() / 2; i++) {
		tmp[i] = func[i * shift];
	}
	for (int i = func.size() / 2 + 1; i < func.size(); i++) {
		tmp[i] = tmp[i - func.size() / 2];
	}
	for (int i = 1; i < func.size(); i++) {
		func[i] = tmp[i];
	}
}

int main(int argc, char **argv){
	std::ifstream input(argv[1], std::ios::binary);
	std::ofstream output(argv[2]);
	std::istreambuf_iterator<char> start(input);
	std::istreambuf_iterator<char> end;
	std::vector<char> buf(start, end);
	for (int i = 0; i < 768; i++) {
		output << buf[i];
	}
	double sec = std::atof(argv[3]);
	int step = 44100 * sec, sizeOfPower2 = maxPowerOf2(step), index = 0, N, shift = std::atof(argv[4]);
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
		change(newFunc, shift);
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
