//решение нелинейных уравненинй 2 итерационными методами 
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;
ofstream out("out.txt");
double intervals[3][2] = { { 0, 1 }, { -0.5, 1 }, { -0.5, 1 } };
double M[3] = { -1 - sin(1), 5, 3333333 };
double m[3] = { -1, 0, 1 / 5 };
double root[3] = { 0.7390851332151606416553120876738734040134, 0, 0 };

double eps = 0.000000000001;

void Relax(double a, double b, double x0, double t, double q, double(*f)(double), double root) {
	double initaproximation = abs(t * f(x0));
	double err = initaproximation;
	bool Chck = true;
	double x;
	unsigned int IterNum = 0;
	while (Chck && IterNum < 10000) {
		x = x0 - t * f(x0);
		x0 = x;
		err = (pow(q, IterNum + 1) * initaproximation) / (1 - q);
		out << "Номер итерации: " << IterNum + 1 << ", Приближение корня: " << x << " ,абсолютная погрешность: " << abs(x - root) << ", относительная погрешность : " << err << endl;

		Chck = (err > eps);
		IterNum++;
	}
}

void Sektant(double x0, double x1, double m, double(*f)(double), double root) {
	bool Chck = true;
	double IterNum = 0;
	double x;
	double err = 10000;
	while (Chck && IterNum < 10000) {
		x = x1 - (x1 - x0) / (f(x1) - f(x0)) * f(x1);
		err = abs(x - x1);
		x0 = x1;
		x1 = x;
		out << "Номер итерации: " << IterNum + 1 << ", Приближение корня: " << x << " ,абсолютная погрешность: " << abs(x - root) << ", относительная погрешность : " << err << endl;
		Chck = (err > eps);
		IterNum++;
	}
}
double f1(double x) {
	return (cos(x) - x);
}
double f2(double x) {
	return pow(x, 5);
}
double f3(double x) {
	return copysign(pow(abs(x), 0.2), x);
}
int main()
{
	setlocale(LC_ALL, "Russian");

	out << setprecision(18) << "Уравнение 1 метод Секущих :" << endl;
	double(*pt2Func)(double) = NULL;
	pt2Func = &f1;
	Sektant(1.5, 1, 1, pt2Func, root[0]);
	out << "Уравнение 1 метод Релаксации:" << endl;

	Relax(intervals[0][0], intervals[0][1], 1, 2 / (m[0] + M[0]), (M[0] - m[0]) / (m[0] + M[0]), f1, root[0]);
	out << "Уравнение 2 метод Секущих" << endl;
	double(*pt2Func2)(double) = NULL;
	pt2Func2 = &f2;
	Sektant(0.5, 0.25, 0, pt2Func2, root[1]);
	out << "Уравнение 2 метод релаксации:" << endl;
	double x0 = 0.25;
	double t = 0.3999999999999;
	double q = 0.9999999999996;
	double initaproximation = abs(t * f2(x0));
	double err = initaproximation;
	bool Chck = true;
	double x;
	unsigned int IterNum = 0;
	while (Chck && IterNum < 10000) {
		x = x0 - t * f2(x0);
		x0 = x;
		err = (pow(q, IterNum + 1) * initaproximation) / (1 - q);
		out << "Номер итерации: " << IterNum + 1 << ", Приближение корня: " << x << " ,абсолютная погрешность: " << abs(x - 0) << ", относительная погрешность : " << err << endl;

		Chck = (err > 0.0001);
		IterNum++;
	}

}
