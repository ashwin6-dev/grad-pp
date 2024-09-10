#include "variable.h"

class Visitor {
public:
    virtual void visit(Variable* variable) = 0;
};