#ifndef NODEBOOLTREE_H
#define NODEBOOLTREE_H

#include "BBV.h"
#include "boolinterval.h"
#include "boolequation.h"

class NodeBoolTree
{
public:
    NodeBoolTree(BoolEquation *equation){this->eq = equation;}
    NodeBoolTree(const NodeBoolTree &node){
        this->eq = node.eq;
        this->lt = node.lt;
        this->rt = node.rt;}
    
	NodeBoolTree *lt = nullptr, *rt = nullptr;

	BoolEquation *eq;
};

#endif // NODEBOOLTREE_H
