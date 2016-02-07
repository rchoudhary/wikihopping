//
// Created by Rutvik Choudhary on 12/1/15.
//

#pragma once

#include <vector>
#include <unordered_map>
#include "Link.h"
#include "Node.h"

// Given a start and target `Link`, this function will return true if a path exists from the start to the target
// and false otherwise.
//
// As it is searching through Wikipedia it populates the graph (where the key is the `href` of the link and the
// value is the Node that represents the page (`see Node.h`)).
//
// This function uses the Breadth-First Search algorithm and so the first path found will be the shortest. This
// function also utilizes the maximum number of threads allowed by the hardware (determined with
// `std::thread::hardware_concurrency()`).
bool findPath(Link start, Link target, std::unordered_map<std::string, Node>* linkGraph);