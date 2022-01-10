#include <iostream>
#include <fstream>

const long double pi = 3.141592;
const long double e = 2.71828182846;
using namespace std;
ofstream outr("outr.txt");
ofstream outt("outt.txt");
ofstream ansol("ansol.txt");
int N = 10;
long double* X;
long double** A;
long double* F;
long double* U;
long double a = 2, b = 3;

long double AnSol(long double x)
{
	return sin(x);
	//return pow(e,x);
}
long double K(long double x, long double s)
{
	return sin(x) * cos(s);
	//return s*x;
}
long double f(long double x)
{
	return  sin(x) + 0.25 * (cos(2 * a) - cos(2 * b)) * sin(x);
	//return x * ((b - 1) * pow(e, b) - (a - 1) * pow(e, a)) + pow(e, x);
}
void init()
{
	//inp >> N;
	X = new long double[N];
	F = new long double[N];
	U = new long double[N];
	A = new long double* [N];
	for (int i = 0; i < N; i++)
	{
		A[i] = new long double[N];
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			A[i][j] = 0;
		}
	}
	long double h = (b - a) / (N - 1);
	for (int i = 0; i < N; i++)
	{
		X[i] = a + h * i;
	}
	for (int i = 0; i < N; i++)
	{
		F[i] = f(X[i]);
	}

}
void rectangle()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			A[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N - 1; j++)
		{
			A[i][j] = K(X[i], X[j]) * (X[j + 1] - X[j]);

		}
		A[i][i]++;
	}

	return;
}
void tr()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			A[i][j] = 0;
		}
	}
	for (int i = 0; i < N; i++) {
		A[i][0] = 0.5 * (X[1] - a) * K(X[i], a);
		for (int j = 1; j < N - 1; j++)
		{
			A[i][j] = 0.5 * (X[j + 1] - X[j - 1]) * K(X[i], X[j]);
		}
		A[i][N - 1] = 0.5 * (b - X[N - 2]) * K(X[i], b);

	}
	for (int i = 0; i < N; i++)
	{
		A[i][i]++;
	}

}

void Gauss()
{
	for (int i = 0; i < N; i++)
	{
		U[i] = F[i];
	}
	for (int i = 0; i < N; i++)
	{

		long double max = 0;
		int Index = 0;
		for (int j = i; j < N; j++)
		{

			if (abs(A[j][i]) >= max)
			{
				max = abs(A[j][i]);
				Index = j;
			}

		}

		long double temp;
		for (int j = i; j < N; j++)
		{
			temp = A[i][j];
			A[i][j] = A[Index][j];
			A[Index][j] = temp;

		}
		temp = U[i];
		U[i] = U[Index];
		U[Index] = temp;
		for (int j = i + 1; j < N; j++)
		{
			if (A[j][i] != 0)
			{
				U[j] = U[j] - A[j][i] / A[i][i] * U[i];
				for (int k = N - 1; k > i - 1; k--)
				{
					A[j][k] = A[j][k] - A[j][i] / A[i][i] * A[i][k];
				}
			}
		}

	}
	for (int i = N - 2; i > -1; i--)
	{
		for (int j = i + 1; j < N - 1; j++)
		{
			U[i] -= U[j] * A[i][j];
		}
		U[i] /= A[i][i];
	}
}
int main()
{
	init();
	tr();
	Gauss();
	for (int i = 0; i < N; i++)
	{
		outt << X[i] << " " << U[i] << endl;
	}
	rectangle();
	Gauss();
	for (int i = 0; i < N; i++)
	{
		outr << X[i] << " " << U[i] << endl;
	}

	long double h = (b - a) / 1000;
	long double s = a;
	while (s <= b)
	{
		ansol << s << " " << AnSol(s) << endl;
		s += h;
	}
	return 0;
}