// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include <iostream>
#include <QFile>
#include <QTextStream>
#include <QStack>
#include <string>
#include <cstring>
#include <stack>
#include <ostream>
#include "NodeBoolTree.h"
#include "boolinterval.h"
#include "boolequation.h"
#include "BBV.h"

#include "Allocator.h"
#include <chrono>

#include "MinOccurenceBranchingStrategy.h"	// Подключаем нашу стратегию и интерфейс
#include "IBranchingStrategy.h"

int main(int argc, char *argv[])
{
	Allocator allocBoolInterval(64, 200);	//100
    Allocator allocBoolEquation(256, 100);	//50
    Allocator allocNodeBoolTree(80, 100);	//50
	Allocator allocCNF(1024, 1);	
	Allocator allocStrategy(64, 1);
	
	auto start = std::chrono::high_resolution_clock::now();

	QStringList full_file_list;
	QList<QStringList> Elements;
	std::string filepath;
	QStringList inputs;

	BoolInterval** CNF = nullptr; 
    int cnfSize = 0;
    BoolInterval* root = nullptr;
    BoolEquation* boolequation = nullptr; 
    NodeBoolTree* startNode = nullptr;
    IBranchingStrategy* strategy = nullptr;

	//std::cout << "Input file path...\n";
	//std::cin >> filepath;
	// Hardcode input
	//	filepath = "sat_ex_2.pla";
	//filepath = "Sat_ex11_3.pla";

	// filepath = "/home/toxicsnail/Code/GCC/DIST/DIST_lab2/SAT_DPLL/SatExamples/sat_new.pla";
	filepath = "/home/toxicsnail/Code/GCC/DIST/DIST_lab2/SAT_DPLL/SatExamples/Sat_ex14_3.pla";

	QFile file(QString::fromUtf8(filepath.c_str()));

	//считываем весь файл
	if ((file.exists()) && (file.open(QIODevice::ReadOnly))) {
		while (!file.atEnd()) {
			full_file_list << file.readLine().replace("\r\n", "");
		}

		int cnfSize = full_file_list.length();
		// BoolInterval **CNF = new BoolInterval*[cnfSize];
		void* memCNF = allocCNF.Allocate(sizeof(BoolInterval*) * cnfSize);
		BoolInterval** CNF = static_cast<BoolInterval**>(memCNF);

		int rangInterval = -1; // error

		if (cnfSize) {
			rangInterval = full_file_list[0].toUtf8().trimmed().length();
		}
		
		for (int i = 0; i < cnfSize; i++) {
			QString line = full_file_list[i].trimmed();

			//CNF[i] = new BoolInterval(line.toUtf8().data());
			// Выделяем память через allocBoolInterval
			void* mem = allocBoolInterval.Allocate(sizeof(BoolInterval));
			CNF[i] = new (mem) BoolInterval(line.toUtf8().data());
		}

		QString rootvec = "";
		QString rootdnc = "";

		//Строим интервал в которм все компоненты принимают значение '-',
		//который представляет собой корень уравнения, пока пустой.
		//В процессе поиска корня, компоненты интервала буду заменены на конкретные значения.

		for (int i = 0; i < rangInterval; i++) {
			rootvec += "0";
			rootdnc += "1";
		}

		QByteArray v = rootvec.toUtf8();

		BBV vec(v.data());
		QByteArray d = rootdnc.toUtf8();
		BBV dnc(d.data());

		// Создаем пустой корень уравнения;
		//BoolInterval *root = new BoolInterval(vec, dnc);
		void* memRoot = allocBoolInterval.Allocate(sizeof(BoolInterval));
   		BoolInterval* root = new (memRoot) BoolInterval(vec, dnc);


		//BoolEquation *boolequation = new BoolEquation(CNF, root, cnfSize, cnfSize, vec);

		// Алгоритм поиска корня. Работаем всегда с верхушкой стека.
		// Шаг 1. Правила выполняются? Нет - Ветвление Шаг 5. Да - Упрощаем Шаг 2.
		// Шаг 2. Строки закончились? Нет - Шаг1, Да - Корень найден? Да - Успех КОНЕЦ, Нет - Шаг 3.
		// Шаг 3. Кол-во узлов в стеке > 1? Нет - Корня нет КОНЕЦ, Да - Шаг 4.
		// Шаг 4. Текущий узел выталкиваем из стека, попадаем в новый узел. У нового узла lt rt отличны от NULL? Нет - Шаг 1. Да - Шаг 3.
		// Шаг 5. Выбор компоненты ветвления, создание двух новых узлов, добавление их в стек сначала с 1 потом с 0. Шаг 1.

		// Алгоритм CheckRules.
		// Цикл по строкам КНФ.
		// 1. Проверка правила 2. Выполнилось? Да - Корня нет, Нет - Идем дальше.
		// 2. Проверка правила 1. Выполнилось? Да - Упрощаем, Нет - Идем дальше.

		// Создаем стек под узлы булева дерева
		// QStack<NodeBoolTree> BoolTree;


		
		// IBranchingStrategy* strategy = new MinOccurenceBranchingStrategy();	// Создаём нашу стратегию
		void* memStrategy = allocStrategy.Allocate(sizeof(MinOccurenceBranchingStrategy));
        strategy = new (memStrategy) MinOccurenceBranchingStrategy();
		//BoolEquation *boolequation = new BoolEquation(CNF, root, cnfSize, cnfSize, vec, strategy);	// Создаём BoolEquation, передавая указатель на стратеги
		void* memEq = allocBoolEquation.Allocate(sizeof(BoolEquation));
		BoolEquation* boolequation = new (memEq) BoolEquation(CNF, root, cnfSize, cnfSize, vec, strategy);
	

		bool rootIsFinded = false;
		stack<NodeBoolTree *> BoolTree;

		//NodeBoolTree *startNode = new NodeBoolTree(boolequation);
		void* memNode = allocNodeBoolTree.Allocate(sizeof(NodeBoolTree));
		NodeBoolTree* startNode = new (memNode) NodeBoolTree(boolequation);
		
		BoolTree.push(startNode);

		do {
			NodeBoolTree *currentNode(BoolTree.top());

			if (currentNode->lt == nullptr &&
					currentNode->rt == nullptr) { // Если вернулись в обработанный узел
				BoolEquation *currentEquation = currentNode->eq;
				bool flag = true;

				// Цикл для упрощения по правилам.
				while (flag) {
					int a = currentEquation->CheckRules(); // Проверка выполнения правил

					switch (a) {
						case 0: { // Корня нет.
							BoolTree.pop();
							flag = false;
							break;
						}

						case 1: { // Правило выполнилось, корень найден или продолжаем упрощать.
							if (currentEquation->getCount() == 0 ||
									currentEquation->getMask().getWeight() ==
									currentEquation->getMask().getSize()) { // Если кончились строки или столбцы, корень найден.
								flag = false;
								rootIsFinded =
									true; // Полагаем, что корень найден, выполняем проверку корня

								for (int i = 0; i < cnfSize; i++) {

									if (!CNF[i]->isEqualComponent(*currentEquation->getRoot())) {
										rootIsFinded = false;//Корень не найден. Продолжаем искать дальше.
										BoolTree.pop();
										break;
									}
								}
							}

							break;
						}

						case 2: { // Правила не выполнились, ветвление.
							// Ветвление, создание новых узлов.

							int indexBranching = currentEquation->ChooseColForBranching();

							//BoolEquation *Equation0 = new BoolEquation(*currentEquation);
							//BoolEquation *Equation1 = new BoolEquation(*currentEquation);
							void* memEq0 = allocBoolEquation.Allocate(sizeof(BoolEquation));
                        	BoolEquation* Equation0 = new (memEq0) BoolEquation(*currentEquation, strategy);

                       		void* memEq1 = allocBoolEquation.Allocate(sizeof(BoolEquation));
                      		BoolEquation* Equation1 = new (memEq1) BoolEquation(*currentEquation, strategy);



							Equation0->Simplify(indexBranching, '0');
							Equation1->Simplify(indexBranching, '1');

							//NodeBoolTree *Node0 = new NodeBoolTree(Equation0);
							//NodeBoolTree *Node1 = new NodeBoolTree(Equation1);
							void* memNode0 = allocNodeBoolTree.Allocate(sizeof(NodeBoolTree));
                       		NodeBoolTree* Node0 = new (memNode0) NodeBoolTree(Equation0);

                        	void* memNode1 = allocNodeBoolTree.Allocate(sizeof(NodeBoolTree));
                        	NodeBoolTree* Node1 = new (memNode1) NodeBoolTree(Equation1);



							currentNode->lt = Node0;
							currentNode->rt = Node1;

							BoolTree.push(Node1);
							BoolTree.push(Node0);

							flag = false;
							break;
						}
					}
				}
			} else {
				BoolTree.pop();
			}

		} while (BoolTree.size() > 1 && !rootIsFinded);

		if (rootIsFinded) {
			cout << "Root is:\n ";
			BoolInterval *finded_root = BoolTree.top()->eq->getRoot();
			cout << string(*finded_root);
			cout << endl;
		} else {
			cout << "Root is not exists!";
		}

	} else {
		std::cout << "File does not exists.\n";
	}
	
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
    std::cout << "Time spent (with allocator): " << diff.count() << " s\n";


	if (startNode) 
	{
        std::stack<NodeBoolTree*> freeStack;
        freeStack.push(startNode);
        while (!freeStack.empty()) 
		{
            NodeBoolTree* node = freeStack.top();
            freeStack.pop();
            if (node->lt) freeStack.push(node->lt);
            if (node->rt) freeStack.push(node->rt);

            if (node->eq) 
			{
                node->eq->~BoolEquation();
                allocBoolEquation.Deallocate(node->eq);
                node->eq = nullptr;
            }
            node->~NodeBoolTree();
            allocNodeBoolTree.Deallocate(node);
        }
    }

    if (CNF) 
	{
        for (int i = 0; i < cnfSize; i++) 
		{
            if (CNF[i]) 
			{
                CNF[i]->~BoolInterval();
                allocBoolInterval.Deallocate(CNF[i]);
                CNF[i] = nullptr;
            }
        }
        allocCNF.Deallocate(CNF);
        CNF = nullptr;
    }

    if (root) 
	{
        root->~BoolInterval();
        allocBoolInterval.Deallocate(root);
        root = nullptr;
    }

    if (strategy) 
	{
        strategy->~IBranchingStrategy();
        allocStrategy.Deallocate(strategy);
        strategy = nullptr;
    }

    return 0;
}
