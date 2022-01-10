#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <fstream>
using namespace std;
ofstream Euler("Euler.txt" );
ofstream RK("RK.txt");
ofstream Adams("Adams.txt");
ofstream ASolution("ASolution.txt");
ofstream Err("Err.txt");
const double PI = 3.141592653589793;
double a = 0;
double b = 1;
double p = 1;
double eps = 0.01;

double fi1(double x) 
{
	return sin(PI*x);
}

double f1(double x,double y)
{
	return (PI * cos(PI * x) + p * pow((y - fi1(x)), 3));	
}

double fi2(double x)
{
	return log(x + 1);
}

double f2(double x, double y)
{
	return (1 / (x + 1) + p * pow((y - fi2(x)), 3));
}

double u1(double x)
{
	return (sin(PI * x));
}

double u2(double x)
{
	return(log(x + 1));
}

double Euler1(double x, double y,double h) 
{
	return (y+h*f1(x, y));
}

double Euler2(double x, double y,double h)
{
	return (y+h*f2(x, y));
}

double RK1(double x, double y,double h)
{
	double k1 = h * f1(x, y);
	double k2 = h * f1(x + h / 4, y + k1 / 4);
	double k3 = h * f1(x + h / 2, y + k2 / 2);
	double k4 = h * f1(x + h, y + k1 - 2 * k2 + 2 * k3);
	return(y + (k1 + 4 * k3 + k4) / 6);
}

double RK2(double x, double y, double h)
{
	double k1 = h * f2(x, y);
	double k2 = h * f2(x + h / 4, y + k1 / 4);
	double k3 = h * f2(x + h / 2, y + k2 / 2);
	double k4 = h * f2(x + h, y + k1 - 2 * k2 + 2 * k3);
	return(y + (k1 + 4 * k3 + k4) / 6);
}

double body_Adams1 (double *x,double*y,double h, int i)
{
	double Y = y[i - 1];
	double H = 0.04166666666 * h;
	double F1 = 55 * f1(x[i - 1], y[i - 1]);
	double F2 = -59 * f1(x[i - 2], y[i - 2]);
	double F3 = 37 * f1(x[i - 3], y[i - 3]);
	double F4 = -9 * f1(x[i - 4], y[i - 4]);
	double u0 = Y + H * (F1 + F2 + F3 + F4);
	
	//double u0 = y[i - 1] + 1 / 24 * h * (55 * f1(x[i - 1], y[i - 1]) - 59 * f1(x[i - 2], y[i - 2]) + 37 * f1(x[i - 3], y[i - 3]) - 9 * f1(x[i - 4], y[i - 4]));
	double un; 
	double uk;
	uk = u0;
	un = u0 + 1;
	int count = 0;
	while (abs(un - uk) > eps)
	{
		count++;
		F1 = 9 * f1(x[i], uk);
		F2 = 19 * f1(x[i - 1], y[i - 1]);
		F3 = -5 * f1(x[i - 2], y[i - 2]);
		F4 = f1(x[i - 3], y[i - 3]);
		un = Y + H * (F1 + F2 + F3 + F4);
		//un= y[i - 1] +H* (9 * f1(x[i], uk) + 19 * f1(x[i - 1], y[i - 1]) -5 * f1(x[i - 2], y[i - 2]) + f1(x[i - 3], y[i - 3]));
		
		uk = un;
	}
	cout << "count = " << count << " i = " << i << endl;
	return un;
}

void Adams1(double* x, double* u, double h ,int N)
{
		for (int i = 1; i < 4; i++)
	{
		u[i] = RK1(x[i - 1], u[i - 1], h);
	}
	for (int i = 4; i < N; i++)
	{
		u[i]=body_Adams1(x, u, h, i);
	}
}

double body_Adams2(double* x, double* y, double h, int i)
{
	double H = 0.04166666666 * h;
	double u = y[i - 1] + H* (55 * f2(x[i - 1], y[i - 1]) - 59 * f2(x[i - 2], y[i - 2]) + 37 * f2(x[i - 3], y[i - 3]) - 9 * f2(x[i - 4], y[i - 4]));
	double un = u + 1;
	double temp = 0;
	double count = 0;
	while (abs(un - u) > eps)
	{
		u = y[i - 1] + H* (9 * f2(x[i], temp) + 19 * f2(x[i - 1], y[i - 1]) - 5 * f2(x[i - 2], y[i - 2]) + f2(x[i - 3], y[i - 3]));
		un = temp;
		count++;
		temp = u;
	}
	cout << "count = " << count << " i = " << i << endl;
	return u;
}

void Adams2(double* x, double* u, double h, int N)
{
	for (int i = 1; i < 4; i++)
	{
		u[i] = RK2(x[i - 1], u[i - 1], h);
	}
	for (int i = 4; i < N; i++)
	{
		u[i] = body_Adams2(x, u, h, i);
	}
}

double maxErr1(double* x, double* y, int N)
{
	double M = 0;
	double temp;
	for (int i = 0; i < N; i++)
	{
		temp = abs(y[i] - u1(x[i]));
		if (M < temp) { M = temp; }
	}
	return M;
}

double maxErr2(double* x, double* y, int N)
{
	double M = 0;
	double temp;
	for (int i = 0; i < N; i++)
	{
		temp = abs(y[i] - u2(x[i]));
		if (M < temp) { M = temp; }
	}
	return M;
}

void Gprint(double* X, double* YE, double* YR, double* YA, int N,int type)
{
	for (int i = 0; i < N; i++)
	{
		Euler << X[i] << " " << YE[i] << endl;
		RK << X[i] << " " << YR[i] << endl;
		Adams << X[i] << " " << YA[i] << endl;
	}
	double x = 0;
	if (type==0)
	{
		while (x <= 1)
		{
			ASolution << x << " " << u1(x)<<endl;
			x += 0.001;
		}
	}
	else
	{
		while (x <= 1)
		{
			ASolution << x << " " << u2(x)<<endl;
			x += 0.001;
		}
	}
}

void body(int N, int type)
{
	double* X = new double[N];
	double* YE = new double[N];
	double* YR = new double[N];
	double* YA = new double[N];
	double h = (b- a) / (N-1);
	for (int i = 0; i < N; i++) 
	{
		X[i] = a + h * i;
	}
	YE[0] = 0;
	YR[0] = 0;
	YA[0] = 0;
	if (type==0)
	{
		for (int i = 0; i < N-1; i++)
		{
			YE[i+1] = Euler1(X[i], YE[i], h);
			YR[i + 1] = RK1(X[i], YR[i], h);
		}
		Adams1(X, YA, h, N);
		Err << "type: " << type << ", N=" << N << endl;
		Err << "MaxErrEuler = " << maxErr1(X, YE, N) << endl;
		Err << "MaxErrRK = " << maxErr1(X, YR, N) << endl;
		Err << "MaxErrAdams = " << maxErr1(X, YA, N) << endl;
	}
	else
	{
		for (int i = 0; i < N-1; i++)
		{
			YE[i + 1] = Euler2(X[i], YE[i], h);
			YR[i + 1] = RK2(X[i], YR[i], h);
		}
		Adams2(X, YA, h, N);
		Err << "type: " << type << ", N=" << N << endl;
		Err << "MaxErrEuler = " << maxErr2(X, YE, N)<<endl;
		Err << "MaxErrRK = " << maxErr2(X, YR, N)<<endl;
		Err << "MaxErrAdams = " << maxErr2(X, YA, N)<<endl;
	}
	Gprint(X, YE, YR, YA, N, type);
	
}

/*int main()
{
	FILE* gpipe = _popen("C:\\gnuplot\\bin\\gnuplot -persist", "w");
	if (!gpipe)
		return -1;

	fprintf(gpipe, "set terminal win\n");
	fprintf(gpipe, "plot sin(x)\n");
	fprintf(gpipe, "pause -1 \"hit enter to exit\"\n");
	fprintf(gpipe, "exit\n");
	_pclose(gpipe);
	
	return 0;
}*/

int main()
{
	Euler.clear();
	RK.clear();
	Adams.clear();
	ASolution.clear();
	Err.clear();
	int N, type;
	cout << "0- fi=sin(pi*x), 1- fi=ln(x+1)" << endl;
	cin >> type;
	cout << "Enter N"<<endl;
	cin >> N;
	body(N, type);
	Err.close();
	Euler.close();
	RK.close();
	Adams.close();
	ASolution.close();
	return 0;
	
}