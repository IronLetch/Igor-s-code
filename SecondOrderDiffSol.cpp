#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;
ofstream RK("RK.txt");
ofstream Adams("Adams.txt");
ofstream ASolution("ASolution.txt");
ofstream Err("Err.txt");
const double PI = 3.141592653589793;
double a = 0;
double b = 1;

double fi(double x)
{
	return -6*(sin(3 * x) + 2 * cos(3 * x));
}
double* F(double*x)
{
	double* U = new double[3];
	U[0] = 1;
	U[1] = 4 * x[1] - 3 * x[2] + fi(x[0]);
	U[2] = x[1];
	return U;
}
double f(double x, double v, double u)
{
	return (fi(x) + 4 * v - 3 * u);
}
double g(double x, double v, double u)
{
	return v;
}
double u(double x)
{
	return (sin(3 * x));
}
void RungK(double** u, int N) 
{
	double h = (b - a)/(N-1);
	u[1][0] = 3;
	u[2][0] = 0;
	double* k1;
	double* k2;
	double* k3;
	double* k;
	double* temp;
	double* Y;
	k1 = new double[3];
	k2 = new double[3];
	k3 = new double[3];
	k = new double[3];
	temp = new double[3];
	Y = new double[3];
	for (int j = 0; j < N - 1; j++) {
		for (int i = 0; i < 3; i++)
		{
			Y[i] = u[i][j];
		}
		k1 = F(Y);
		for (int i = 0; i < 3; i++)
		{
			k1[i] = h * k1[i];
		}
		
		for (int i = 0; i < 3; i++)
		{
			temp[i] = Y[i]+0.5*k1[i];
		}
		k2 = F(temp);
		for (int i = 0; i < 3; i++)
		{
			k2[i] = h * k2[i];
		}
		for (int i = 0; i < 3; i++)
		{
			temp[i] = Y[i]-k1[i] + 2* k2[i];
		}
		k3 = F(temp);
		for (int i = 0; i < 3; i++)
		{
			k3[i] = h * k3[i];
		}
		
		for (int i = 1; i < 3; i++) 
		{
			u[i][j + 1] = u[i][j] + (k1[i] + 4 * k2[i] + k3[i]) / 6;

		}
	}
}
void Ad(double** u, int N)
{
	u[1][0] = 3;
	u[2][0] = 0;
	double h = (b - a) / (N - 1);
	double* k1;
	double* k2;
	double* k3;
	double* k;
	double* temp;
	double* Y;
	double a = 1.91666666666;
	double b = -1.33333333333;
	double c = 0.41666666666;
	k1 = new double[3];
	k2 = new double[3];
	k3 = new double[3];
	k = new double[3];
	temp = new double[3];
	Y = new double[3];
	cout << "a= "<<a << "b= " << b << "c= " << c;
	for (int j = 0; j < N-1; j++) {
		for (int i = 0; i < 3; i++)
		{
			Y[i] = u[i][j];
		}
		k1 = F(Y);
		for (int i = 0; i < 3; i++)
		{
			k1[i] = h * k1[i];
		}

		for (int i = 0; i < 3; i++)
		{
			temp[i] = Y[i] + 0.5 * k1[i];
		}
		k2 = F(temp);
		for (int i = 0; i < 3; i++)
		{
			k2[i] = h * k2[i];
		}
		for (int i = 0; i < 3; i++)
		{
			temp[i] = Y[i] - k1[i] + 2 * k2[i];
		}
		k3 = F(temp);
		for (int i = 0; i < 3; i++)
		{
			k3[i] = h * k3[i];
		}

		for (int i = 1; i < 3; i++)
		{
			u[i][j + 1] = u[i][j] + (k1[i] + 4 * k2[i] + k3[i]) / 6;
		}
	}
	for (int j = 3; j < N; j++) 
	{
		for (int i = 0; i < 3; i++)
		{
			k1[i] = u[i][j - 1];
			k2[i] = u[i][j - 2];
			k3[i] = u[i][j - 3];
		}
		temp = F(k1);
		k = F(k2);
		Y = F(k3);
		
		for (int i = 1; i < 3; i++)
		{
			temp[i] = temp[i] * h;
			k[i] = h * k[i];
			Y[i] = h * Y[i];
			u[i][j] = u[i][j - 1]+ a * temp[i] + b * k[i] + c * Y[i];
		}
	}
}


int main()
{
	RK.clear();
	Adams.clear();
	ASolution.clear();
	Err.clear();
	int N;
	cin >> N;
	double** uRK = new double* [3];
	for (int i = 0; i < 3; i++)
	{
		uRK[i] = new double[N];

	}
	double h = (b - a) / (N - 1);
	for (int i = 0; i < N; i++)
	{
		uRK[0][i] = a + h * i;
	}
	RungK(uRK, N);
	double** uA = new double* [3];
	for (int i = 0; i < 3; i++)
	{
		uA[i] = new double[N];

	}
	for (int i = 0; i < N; i++)
	{
		uA[0][i] = a + h * i;
	}
	Ad(uA, N);
	double M = 0, L=0;
	double temp1,temp2;
	for (int i = 0; i < N; i++)
	{
		temp1 = abs(uRK[2][i] - u(uRK[0][i]));
		temp2 = abs(uA[2][i] - u(uA[0][i]));
		if (M < temp1) { M = temp1; }
		if (L < temp2) { L = temp2; }

	}
	Err << M << "   Err in RK" << endl << L << "   Err In Adams";
	for (int i = 0; i < N; i++)
	{
		RK << uRK[0][i] << " " << uRK[2][i] << endl;
		Adams << uA[0][i] << " " << uA[2][i] << endl;
	}
	double x = 0;

	while (x <= 1)
	{
		ASolution << x << " " << u(x) << endl;
		x += 0.001;
	}
	delete[] uRK;
	delete[] uA;
	RK.close();
	Adams.close();
	ASolution.close();

	return 0;

}