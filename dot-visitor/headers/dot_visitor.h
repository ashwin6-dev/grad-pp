#pragma once

#include "../../visitor/headers/visitor.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>

class DotVisitor : public Visitor {
public:
    DotVisitor() : var_count(0), node_count(0) {}

    void visit(Variable* variable) override;
    void visit(Const* constant) override;
    void visit(Input* input) override;
    void visit(Add* node) override;
    void visit(Subtract* node) override;
    void visit(Multiply* node) override;
    void visit(Divide* node) override;

    std::string get_dot_representation(std::shared_ptr<Node> graph, std::unordered_map<Node*, std::string> var_names);
    std::string get_dot_reg_representation(std::shared_ptr<Node> graph, std::unordered_map<Node*, std::string> var_names, std::unordered_map<Node*, int> ralloc);
private:
    std::unordered_map<Node*, std::string> node_map;
    std::unordered_map<Node*, int> reg_alloc;
    int var_count;
    int node_count;
    std::string generate_variable_name();
    std::string generate_node_name();

    std::ostringstream dot_output;
};

void write_dot_file(const std::string& filename, const std::string& dot_content);
void generate_image_from_dot(const std::string& dot_filename, const std::string& image_filename);