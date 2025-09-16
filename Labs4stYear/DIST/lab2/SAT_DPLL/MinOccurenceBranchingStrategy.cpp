// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "MinOccurenceBranchingStrategy.h"
#include "boolinterval.h"

int MinOccurenceBranchingStrategy::chooseColumn(BoolEquation& equation)
{
    std::vector<int> indexes;
    std::vector<int> values;
    bool rezInit = false;

    for (int i = 0; i < equation.getMask().getSize(); i++) 
    {
        if (equation.getMask()[i] == 0) 
        {
            indexes.push_back(i);
        }
    }

    for (int i = 0; i < equation.getCnfSize(); i++) 
    {
        BoolInterval* interval = equation.getCnf()[i];
        if (interval != nullptr) 
        {
            if (!rezInit) {
                for (int k : indexes) 
                {
                    char val = interval->getValue(k);
                    int pushValue = 0;
                    if (val == '-') 
                    {
                        pushValue = 1;
                    }
                    values.push_back(pushValue);
                }
                rezInit = true;
            } else {
                for (int idx = 0; idx < (int)indexes.size(); idx++) 
                {
                    if (interval->getValue(indexes[idx]) == '-') 
                    {
                        values[idx]++;
                    }
                }
            }
        }
    }

    int minElementIndex = std::min_element(values.begin(), values.end()) - values.begin();
    return indexes[minElementIndex];
}
