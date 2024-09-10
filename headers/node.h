#pragma once

#include <vector>
#include <memory>

class Visitor;

class Node {
public:
    virtual void accept(Visitor& visitor) = 0;
};