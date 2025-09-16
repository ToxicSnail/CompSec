#ifndef BOOLEQUATION_H
#define BOOLEQUATION_H

#include "boolinterval.h"
#include "IBranchingStrategy.h"  // Подключаем интерфейс

class BoolEquation
{
public:
	BoolEquation(BoolInterval **cnf, BoolInterval *root, int cnfSize, int count, BBV mask, IBranchingStrategy* strategy = nullptr);
	BoolEquation(BoolEquation &equation, IBranchingStrategy* strategy = nullptr);

	int CheckRules();
	void Simplify(int ixCol, char value);

	
	int ChooseColForBranching();	//вызовем стратегию

	int getCnfSize() const {return cnfSize;}
    int getCount() const {return count;}
    BoolInterval** getCnf() const {return cnf;}
    BoolInterval* getRoot() const {return root;}
	BBV& getMask() {return mask;}
    const BBV& getMask() const {return mask;}

	void setBranchingStrategy(IBranchingStrategy* strategy) 
	{
        branchingStrategy = strategy;
    }

private:
	BoolInterval **cnf;//множество интервалов
	int cnfSize; // Размер КНФ
	BoolInterval *root;//Корень уравнения
	int count; //количество дизъюнкций
	BBV mask; //маска для столбцов

	bool Rule1Row1(BoolInterval *interval);
	bool Rule2RowNull(BoolInterval *interval);
	void Rule3ColNull(BBV vector);
	bool Rule4Col0(BBV vector);
	bool Rule5Col1(BBV vector);

	IBranchingStrategy* branchingStrategy;
};

#endif // BOOLEQUATION_H
