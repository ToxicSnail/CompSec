#ifndef MINOCCURENCEBRANCHINGSTRATEGY_H
#define MINOCCURENCEBRANCHINGSTRATEGY_H

#include "IBranchingStrategy.h"
#include "boolequation.h"
#include <vector>
#include <algorithm>

class MinOccurenceBranchingStrategy : public IBranchingStrategy
{
public:
    // Переопределяем метод интерфейса 
    int chooseColumn(BoolEquation& equation) override;
};

#endif
