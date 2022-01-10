// Символьное дифференцирование

#include<stdlib.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;
ofstream out("out.txt");
class Expression {
public:
	virtual Expression* dif(char x) = 0;
	virtual void print() = 0;
};

class Number :public Expression
{
public:
	double e;

	Number(double n) : e(n) {}
	Expression* dif(char x)
	{


		return new Number(0);

	}
	void print()
	{
		out << e;
	}

};
Expression* Nl = new Number(0);

class Variable :public Expression
{
	char X;
public:
	Variable(char c) :X(c) {}
	Expression* dif(char x)
	{
		if (X == x)
			return new Number(1);
		else return new Number(0);
	}
	void print()
	{
		out << X;
	};
};

class Add :public Expression
{
	Expression* m1, * m2;
public:
	Add(Expression* L1, Expression* L2) : m1(L1), m2(L2) {}
	Expression* dif(char x)
	{
		return new Add(m1->dif(x), m2->dif(x));

	};
	void print() {
		m1->print();
		out << "+";
		m2->print();
	}
};

class Mul :public Expression
{
	Expression* e1;
	Expression* e2;
public:
	Mul(Expression* L1, Expression* L2) : e1(L1), e2(L2) {}
	Expression* dif(char x)
	{
		return new Add(new Mul(e1->dif(x), e2), new Mul(e1, e2->dif(x)));

	};
	void print() {
		e1->print();
		out << "*";
		e2->print();
	}

};

class Sub :public Expression
{
	Expression* m1, * m2;
public:
	Sub(Expression* L1, Expression* L2) : m1(L1), m2(L2) {}
	Expression* dif(char x)
	{
		return new Sub(m1->dif(x), m2->dif(x));

	};
	void print() {
		m1->print();
		out << "-";
		m2->print();
	}
};

class Res :public Expression
{
	Expression* e1;
	Expression* e2;
public:
	Res(Expression* L1, Expression* L2) : e1(L1), e2(L2) {}
	Expression* dif(char x)
	{
		return new Res(new Sub(new Mul(e1->dif(x), e2), new Mul(e1, e2->dif(x))), new Mul(e2, e2));

	};
	void print() {
		out << "(";
		e1->print();
		out << ")";
		out << "/";
		out << "(";
		e2->print();
		out << ")";

	}
};

class Pow :public Expression {
	Expression* arg;
	double p;
public:
	Pow(Expression* arg1, double p1) :arg(arg1), p(p1) {}
	Expression* dif(char x) {
		Expression* b = new Mul(new Number(p), new Pow(arg, p - 1));
		return new Mul(b, arg->dif(x));
	}
	void print() {
		out << "(";
		arg->print();
		out << ")";
		out << "^" << p;
	}
};

class Exp : public Expression {
	Expression* e;
public:
	Exp(Expression* e1) :e(e1) {}
	Expression* dif(char x)
	{
		return new Mul(e->dif(x), new Exp(e));
	}
	void print() {
		out << "e^";
		out << "(";
		e->print();
		out << ")";
	}
};
/*
Expression* read_expression(const char* Text) {
	int len = 0, brecketsOpChk = 0, brecketsClChk = 0, algChk = 0, i = 0, symbolsChk = 0, Abeg = -1, Aend = -1, Bbeg = -1, Bend = -1, expFirstChk = 0;
	char mid = ' ';
	while (Text[len] != '\0') {
		len++;
	}
	while (Text[i] != '\0') {
		if ((Text[i] == 'e') && (brecketsOpChk == 0)) {
			expFirstChk = 1;
		}
		if ((Text[i] >= 'a') && (Text[i] <= 'z'))
			symbolsChk = 1;
		if (Text[i] == '(') {
			brecketsOpChk++;
			if (brecketsOpChk == 2) {
				if (Abeg == -1)
					Abeg = i;
				else
					Bbeg = i;
			}
		}
		if (Text[i] == ')') {
			brecketsClChk++;
			if (brecketsClChk == (brecketsOpChk - 1)) {
				brecketsClChk = 0;
				brecketsOpChk = 1;
				if (Aend == -1) {
					Aend = i;
				}
				else
					Bend = i;
			}
		}
		if ((Aend != -1) && (Bbeg == -1)) {
			if (((Text[i] == '-') || (Text[i] == '+') || (Text[i] == '*') || (Text[i] == '/'))) {
				if (mid != ' ') {
					//exception
				}
				mid = Text[i];
			}
		}
		if (((Text[i] == '-') || (Text[i] == '+') || (Text[i] == '*') || (Text[i] == '/'))) {
			if (brecketsOpChk == 1) {
				mid = Text[i];
			}
			else
				algChk = 1;
		}
		i++;
	}
	i = 0;
	char* A, * B;
	A = new char[Aend - Abeg + 1];

	if ((mid == ' ') || (expFirstChk == 1)) {                   //if Text == (A), brekets - optional
		if ((algChk == 1) && (expFirstChk != 1)) {
			i = Abeg;
			while (i <= Aend) {
				A[i] = Text[i];                                 //what if (A) == ((B o C)), o - check under
				i++;
			}
			Expression* A_ = read_expression(A);
			return A_;
		}
		else {
			if (Abeg != -1) {
				i = Abeg;
				int y = 0;
				while (i <= Aend) {
					A[y] = Text[i];                             //what if (A) == ((A)) , where A == const, trgan, variable
					i++;
					y++;
				}
				Expression* A_ = read_expression(A);
				return A_;
			}
			if (expFirstChk == 1) {
				i = 4;
				int y = 0;
				while (i < (len - 1)) {                         //what if A == trigan(B)
					A[y] = Text[i];
					i++;
					y++;
				}
				Expression* A_ = read_expression(A);
				return new Exp(A_);

			}
			if (symbolsChk == 1) {
				return new Variable(Text[0]);
			}
			double ans = 0.0;
			int cur = 0;
			len--;
			while (Text[i] != '\0') {
				if ((Text[i] >= '0') && (Text[i] <= '9'))
					cur = Text[i] - '0';
				ans = ans + pow(10.0, (double)len) * cur;
				len--;
				i++;
			}
			return new Number(ans);
		}
	}


	Expression* A_, * B_;
	B = new char[Bend - Bbeg + 1];              // if Text == (A o B), breakets - required, o = {'+', '-', '*', '/'}
	if ((brecketsOpChk == 1) && (brecketsClChk == 1)) {
		i = 1;
		while (Text[i] != mid) {
			i++;
		}
		A = new char[i - 1];
		B = new char[len - i - 2];
		i = 1;
		int y = 0;
		while (Text[i] != mid) {
			A[y] = Text[i];
			i++;
			y++;
		}
		i++;
		y = 0;
		while (Text[i] != ')') {
			B[y] = Text[i];
			i++;
			y++;
		}
		A_ = read_expression(A);
		B_ = read_expression(B);
	}
	else {
		i = Abeg;
		int y = 0;
		while (i <= Aend) {
			A[y] = Text[i];
			y++;
			i++;
		}
		A_ = read_expression(A);
		i = Bbeg;
		y = 0;
		while (i <= Bend) {
			B[y] = Text[i];
			i++;
			y++;
		}
		B_ = read_expression(B);
	}
	if (mid == '+')
		return new Add(A_, B_);
	if (mid == '-')
		return new Sub(A_, B_);
	if (mid == '*')
		return new Mul(A_, B_);
	return new Res(A_, B_);

}
list<char> stack;
Expression* rd_expr(const char* Text) {
	int i = 0;
	char* txt;
	int len = 0;
	while (Text[len] != '\0') len++;
	txt = new char[len + 1];
	for (i = 0; i < len; i++) txt[i] = Text[i];
	return new Number(0);
}

int get_priority(const std::string& token) {
	if (token == "+") return 1;
	if (token == "-") return 1;
	if (token == "*") return 2;
	if (token == "/") return 2;
	if (token == "^") return 3;
	return 0;
}
*/
int main()
{
	char var;
	//cin >> per;
	var = 'x';
	//Expression* a = ("(e^(3*(e^x)))");
	Expression* a = new Exp(new Mul(new Number(3), new Exp(new Variable('x'))));
	a->print();
	out<<endl;
	Expression* da = a->dif(var);
	da->print();
	Expression* p = new  Mul(new Variable('x'), new Variable('y'));
	Expression* l = new Pow(p, 3);
	Expression* j = new Res(new Number(3), new Variable('x'));
	Expression* k = new Exp(new Mul(new Number(3), new Variable('x')));
	Expression* i = new Add(l, k);
	i->print();
	out << endl;
	Expression* di = i->dif(var);
	di->print();
	out << endl;
	Expression* d2i = di->dif(var);
	d2i->print();
	out << endl;
	Expression *d3i = d2i->dif(var);
	d3i->print();
	out << endl;
	system("pause");
	return 0;
}