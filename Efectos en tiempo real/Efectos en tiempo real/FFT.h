#pragma once

#include <complex>

typedef std::complex<double> Complex;

namespace ASSD
{
	void FFT(Complex * in, Complex * out, unsigned n);
	void DFT(Complex * in, Complex * out, unsigned n);
	void IFFT(Complex * in, Complex * out, unsigned n);
}