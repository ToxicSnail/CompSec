#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int** create_matrix(int n);
void print_matrix(int** matrix, int n);

class Route
{
private:
	int* P;
	int n;
public:
	Route(int towns = 0);
	Route(const Route&);
	Route& operator=(const Route&);
	~Route();

	bool Next_route();
	int Route_price(int** matrix);

	friend ostream& operator<< (ostream& out, const Route& another);
};


Route::Route(int towns)
{
	n = towns;
	P = new int[n];
	// Тождественная перестановка
	for (int i = 0; i < n; i++)
		P[i] = i;
}

Route::Route(const Route& other_route)
{
	n = other_route.n;
	P = new int[n];
	for (int i = 0; i < n; i++)
		P[i] = other_route.P[i];
}

Route::~Route()
{
	if (P != nullptr)
	{
		delete(P);
		P = nullptr;
	}
}

bool Route::Next_route()
{
	int i = n - 2;
	if (i <= 0)
		return false;
	// Ищем пару (P[i] < P[i+1])
	for (i; i >= 0; i--)
	{
		if (i == 0)
			return false;
		if (P[i] < P[i + 1])
			break;
	}

	// Ищем максимальный j: P[i] < P[j]
	int j = n - 1;
	for (j; j > i; j--)
	{
		if (P[i] < P[j])
			break;
	}

	// Меняем местами
	int c;
	c = P[i];
	P[i] = P[j];
	P[j] = c;

	// Реверс
	int left = i + 1;
	int right = n - 1;

	while (left < right) {
		c = P[left];
		P[left] = P[right];
		P[right] = c;
		left++;
		right--;
		return true;
	}
}

int Route::Route_price(int** matrix)
{
	int price = 0;
	int i = 0;
	for (i; i < n - 1; i++)
		price += matrix[P[i]][P[i + 1]];
	price += matrix[P[i]][0];
	return price;
}


ostream& operator<< (ostream& out, const Route& another)
{
	for (int i = 0; i < another.n; i++)
		out << another.P[i] + 1 << "->";
	out << '1';
	return out;
}

Route& Route::operator= (const Route& other)
{
	if (this == &other)
		return *this;

	n = other.n;
	if (P != nullptr)
		delete(P);

	P = new int[other.n];
	for (int i = 0; i < n; i++)
		P[i] = other.P[i];

	return *this;
}
int main(int argc, char const* argv[])
{
    using namespace std;

    int towns;
    cout << "Count of towns: ";
    cin >> towns;

    if (towns <= 0)
    {
        cout << "There is no any towns\n";
        return 1;
    }


    cout << endl;
    Route way(towns);
    Route cmp_way(towns);
    // Генерируем случайную матрицу стоимости 
    int** matrix = create_matrix(towns);
    print_matrix(matrix, towns);

    // Генерируем следующие последовательности и вычисляем цены
    int price;
    int cmp_price = way.Route_price(matrix);
    do
    {
        price = way.Route_price(matrix);
        cout << "Price of the " << way << " route is " << price << endl;
        if (cmp_price > price)
        {
            cmp_price = price;
            cmp_way = way;
        }
    } while (way.Next_route());

    cout << "\nMinimal " << cmp_way << " " << cmp_price << endl;

    for (int i = 0; i < towns; i++)
        delete(matrix[i]);
    delete(matrix);

    return 0;
}

int** create_matrix(int n)
{
    srand(time(NULL));
    int** square_matrix = new int* [n];
    for (int i = 0; i < n; i++)
        square_matrix[i] = new int[n];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (i == j)
                square_matrix[i][j] = 0;
            else
                square_matrix[i][j] = rand() % 10 + 1;

        }
    }
    return square_matrix;
}

void print_matrix(int** matrix, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%2d ", matrix[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}
