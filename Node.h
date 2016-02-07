//
// Created by Rutvik Choudhary on 12/1/15.
//

#pragma once

#include "link.h"

// Simple struct used to represent an article and be used in a graph.
struct Node
{
    Link link;
    Node* parent;
    int distance;
};