#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <vector>
#include "FFT.h"

// Tabla con twiddle factors precalculados

#include "W2.h"
// Tabla con bit-reverse precalculados
#include "BR2.h"

using namespace std;

#define M_PI 3.1415926535897932384626433832795028841971693993751

void ASSD::FFT(Complex * in, Complex * out, unsigned n)
{
	// Calcular cantidad de niveles
	unsigned y = (unsigned)floor(log2(n));
	
	if (out != in)
		memcpy(out, in, sizeof(Complex)*n);

	// Ordenar los datos bit-reversed
	for (unsigned i = 0; i < n; i++)
	{
		unsigned j = BR[y][i];
		if (j> i)
			swap(out[i], out[j]);
	}

	// Algoritmo de Cooley-Tukey con decimacion en el tiempo

	// Etapas
	for (unsigned TG = 2; TG <= n; TG *= 2)
	{ // TG: Tamaño del grupo
	  // NM: Numero de mariposas por grupo
	  // NG: Numero de grupos
		unsigned NM = TG / 2;
		unsigned NG = n / TG;
		// Grupos
		for (unsigned i = 0; i < n; i += TG)
		{
			// Mariposas
			for (unsigned j = i, k = 0; j < i + NM; j++, k += NG)
			{
				Complex temp = out[j + NM] * W[y][k];
				out[j + NM] = out[j] - temp;
				out[j] += temp;
			}
		}
		if (TG == n) break;
	}
}

void ASSD::DFT(Complex * in, Complex * out, unsigned n)
{
	out = (Complex*)memset(out, 0, sizeof(Complex)*n);
	for (unsigned k = 0; k < n; k++)
	{
		for (unsigned i = 0; i < n; i++)
		{
			out[k] = out[k] + in[i]* exp(Complex(0, -2 * M_PI/n * i * k));
		}
	}
}

void ASSD::IFFT(Complex * in, Complex * out, unsigned n)
{
	if (out != in)
		memcpy(out, in, sizeof(Complex)*n);
	for (unsigned i = 0; i < n; i++) out[i] = conj(out[i]);
	FFT(out,out,n);
	for (unsigned i = 0; i < n; i++) out[i] = conj(out[i])/Complex(n);
}
