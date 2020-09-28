#pragma once

//#define _GLIBCXX_DEBUG 1
#include "node.h"
#include "date_parser.h"

#include <memory>
#include <iostream>

using namespace std;

shared_ptr<Node> ParseCondition(istream& is);

void TestParseCondition();
