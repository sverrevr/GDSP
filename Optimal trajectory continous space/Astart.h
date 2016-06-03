#pragma once

#include <vector>
#include "gameClasses.h"

bool aStar(Board* brett, std::vector<coordinate>* optimalPath);
void attachAndEval(Node* parent, Node* child);
void propagatePathImprovements(Node* parent, sortedNodeList* open);
void markPathNodes(Board* brett);
void getOptimalPath(Board* brett, std::vector<coordinate>* optimalPath);