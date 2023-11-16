#include <iostream> /*Подключаем библиотеки c++*/
#include <cmath>

using namespace std; /*Нужно чтобы искались* функции перед которыми std* например такой является cout, можно использовать конкретно using std::cout или endl, а можно чтобы искались среди всех std*/

class Complex /*Класс может определять переменные и константы для хранения состояния объекта и функции для определения поведения объекта.*/
{
private: /*список свойств и методов для использования внутри класса*/
	double Re, Im; // действительная и мнимая части

public: /*список методов доступных другим функциям и объектам программы*/
	Complex(double r = 0, double i = 0);
	~Complex();
	void setRe(double r);
	void setIm(double i);
	double getRe();
	double getIm();
	double mod(); 
	Complex operator+ (Complex&);
	Complex operator- (Complex&);
	Complex operator* (Complex&);
	Complex operator/ (Complex&);
	bool operator== (Complex&);
	bool operator!= (Complex&);
	friend ostream& operator<<(ostream& out, const Complex& c);
	friend istream& operator>>(istream& in, Complex& c);
};

Complex::Complex(double r, double i) /*конструктор */
{
	Re = r;
	Im = i;
}
Complex::~Complex() /*деструктор(работает после конструктора)*/
{
}

void Complex::setRe(double r) { Re = r; }
void Complex::setIm(double i) { Im = i; }

double Complex::getRe() { return Re; }
double Complex::getIm() { return Im; }


double Complex::mod()
{
	double temp;
	temp = sqrt(Re * Re + Im * Im);
	return temp;
}

Complex Complex::operator+(Complex& c) /*Операция сложения*/
{
	Complex temp;
	temp.Re = Re + c.Re;
	temp.Im = Im + c.Im;
	return temp;
}

Complex Complex::operator-(Complex& c) /*Операция вычитания*/
{
	Complex temp;
	temp.Re = Re - c.Re;
	temp.Im = Im - c.Im;
	return temp;
}

Complex Complex::operator*(Complex& c) /*Операция умножения*/
{
	Complex temp;
	temp.Re = Re * c.Re - Im * c.Im;
	temp.Im = Im * c.Re + Re * c.Im;
	return temp;
}
 
Complex Complex::operator/(Complex& c) /*Операция деления*/
{
	Complex temp;

	double r = c.Re * c.Re + c.Im * c.Im;
	temp.Re = (Re * c.Re + Im * c.Im) / r;
	temp.Im = (Im * c.Re - Re * c.Im) / r;
	return temp;
}

bool Complex::operator==(Complex& c) /*Операция сравнения*/
{
	if (Re != c.Re)
		return false;

	if (Im != c.Im)
		return false;

	return true;
}

bool Complex::operator!=(Complex& c) /*Операция "отрицательного" сравнения*/
{
	if (Re != c.Re)
		return true;

	if (Im != c.Im)
		return true;

	return false;

}

ostream& operator << (ostream& out, const Complex& c) /*перегрузка оператора « позволяет определить действия, которые будет выполнять оператор. Функция оператора может быть определена как член класса, либо вне класса.*/
{
	out << "(" << c.Re << ", " << c.Im << ")";
	return out;
}

istream& operator >> (istream& in, Complex& c) /*перегрузка оператора »*/
{
	in >> c.Re >> c.Im;
	return in;
}


int main(int argc, char const* argv[])
{
	Complex a;
	Complex b;

	cout << "Enter num a: ";
	cin >> a;
	cout << "Enter num b: ";
	cin >> b;

	
	cout << endl;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "Sum is " << a + b << endl;
	cout << "Sub is " << a - b << endl;
	cout << "Mul is " << a * b << endl;
	cout << "Div is " << a / b << endl;
	cout << "|a| = " << a.mod() << endl;
	cout << "|b| = " << b.mod() << endl;

	if (a == b)
		cout << "a = b\n";
	else
		cout << "a != b\n";


	return 0;
};
