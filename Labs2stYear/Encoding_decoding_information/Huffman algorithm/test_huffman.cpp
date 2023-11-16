#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
using namespace std;

class Uzel
{
public:
	int a;  /* - число*/
	char c;  /*с - символ*/
	Uzel *left, *right;  /*указатели на левого и правого "сына"*/

	Uzel() { left = right = NULL; }

	Uzel(Uzel* L, Uzel* R)  /*конструктор перегрузки*/
	{
		left = L;
		right = R;
		a = L->a + R->a;
	}
};


struct MyCompare
{
	bool operator()(const Uzel* l, const Uzel* r) const 
    {
      return l->a < r->a;
    }
};

// void print(Uzel* top, unsigned k=0)
// {
//   if (top != NULL)
//   {
//     print(top->left, k + 3);

//     for (unsigned i = 0; i <k; i++)
//       {
//         cout << " ";
//       }
//     if (top->c) cout<< top->a << " ("<<top->c<<")"<<endl; //если есть буква у узла, то выводить
//     else cout<<top->a << endl; //если нет
//     print(top->right,k+3);
//   }
// }


vector<bool> code;
map<char, vector<bool> > table;

void CreateTable(Uzel* top)  /*создаем пары 'символ-код'(ассоциации) (п.7-8)*/
{
	if (top->left != NULL)
	{
		code.push_back(0);
		CreateTable(top->left);
	}

	if (top->right != NULL)
	{
		code.push_back(1);
		CreateTable(top->right);
	}

	if (top->left == NULL && top->right == NULL) table[top->c] = code;

	code.pop_back();
}


int main(int argc, char* argv[])  /* считаем частоты символов*/
{
  string file;  /*открываем файл*/
  cout << "Enter name file : ";
  cin >> file;
  ifstream f;
  f.open(file);
    if (!f.is_open()) {
        cout << "don't open this file!" << endl;
        exit(1);
    }


	map<char, int> m;

	while (!f.eof())  /*пока не конец файла, считываем файл*/
	{
		char c = f.get();
		m[c]++;
	}

  // map<char, int>::iterator i;  // вывод ассоциированного массива 
  // for (i=m.begin(); i != m.end(); i++)
  //  cout << i->first << ":" << i->second << endl;


	list<Uzel*> t;  /* записываем начальные узлы в список list (п.3)*/
	for (map<char, int>::iterator itr = m.begin(); itr != m.end(); ++itr)
	{
		Uzel* p = new Uzel;
		p->c = itr->first;
		p->a = itr->second;
		t.push_back(p);
	}


	while (t.size() != 1)  /* создаем дерево (п.4-5-6)*/
	{
		t.sort(MyCompare()); 

		Uzel* SonL = t.front();
		t.pop_front();
		Uzel* SonR = t.front();
		t.pop_front();

		Uzel* parent = new Uzel(SonL, SonR);
		t.push_back(parent);

	}

	Uzel* top = t.front();   /*top - указатель на вершину дерева*/
  // print(top);
				

	CreateTable(top);  /*создаем пары 'символ-код'(ассоциации) (п.7-8)*/

	

	f.clear(); f.seekg(0); /*очищаем файл и перемещаем указатель снова в начало файла*/

	ofstream g("coded_text.txt", ios::out | ios::binary);

	int count = 0; char tmp = 0; /*Выводим коды в файл coded_text.txt*/
	while (!f.eof())  /*пока не конец файла, считываем файл и выводим байты*/ 
	{
		char c;
    c = f.get();
		vector<bool> x = table[c];
		for (int n = 0; n < x.size(); n++)
		{
			tmp = tmp | x[n] << (7 - count);  /*вектор преобразуем в байты, выводя в файл*/
			count++;
			if (count == 8) { count = 0;   g << tmp; tmp = 0; }
		}
	}
  cout << "File coded successfully" << endl;
  

	f.close();
	g.close();

	///// считывание из файла output.txt и преобразование обратно

	ifstream F("coded_text.txt", ios::in | ios::binary);
  ofstream G("decoded_text.txt", ios::out | ios::binary);

	Uzel* p = top;
	count = 0; char byte;
	byte = F.get();
	while (!F.eof())  /*проходимся по коду(байтам) и собираем наш изначальный файл*/
	{ 
		bool b = byte & (1 << (7 - count));
		if (b) p = p->right; else p = p->left;
		if (p->left == NULL && p->right == NULL) { G << p->c; p = top; }
		count++;
		if (count == 8) { count = 0; byte = F.get(); }
	}
  cout << "File decoded successfully" << endl;

	F.close(); G.close();

	return 0;
}
