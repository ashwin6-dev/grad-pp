#pragma once

#include "node.h"

class Input : public Node {
public:
    void accept(Visitor* visit) override;
};

std::shared_ptr<Input> make_input();