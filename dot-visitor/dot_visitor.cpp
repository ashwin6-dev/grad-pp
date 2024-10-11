#include "headers/dot_visitor.h"

std::string DotVisitor::generate_variable_name() {
    return "var" + std::to_string(var_count++);
}

std::string DotVisitor::generate_node_name() {
    return "node" + std::to_string(node_count++);
}

void DotVisitor::visit(Variable* variable) {
    // Generate a unique variable name and store it in the map
    if (node_map.find(variable) == node_map.end()) {
        node_map[variable] = generate_variable_name();
    }

    if (reg_alloc.count(variable)) {
        dot_output << node_map[variable] << " [label=< <font color=\"red\">" << node_map[variable] << "[" << reg_alloc[variable] << "]" << "</font> >];\n";
    }else {
        dot_output << node_map[variable] << " [label=< <font color=\"red\">" << node_map[variable] << "</font> >];\n";
    }
}

void DotVisitor::visit(Const* constant) {
    // Generate a name for the constant and store it in the map
    std::string const_name = generate_node_name();
    node_map[constant] = const_name;

    if (reg_alloc.count(constant)) {
        dot_output << node_map[constant] 
        << " [label=< <font color=\"black\">Const[" 
        << constant->get_value() << "] [" << reg_alloc[constant] << "]" << "</font> >];\n";
    }else {
       // Add the constant to the DOT graph with black-colored text
        dot_output << const_name << " [label=< <font color=\"black\">Const[" 
               << constant->get_value() << "]</font> >];\n";
    }
}

void DotVisitor::visit(Input* input) {
    std::string input_name = generate_node_name();
    node_map[input] = input_name;

    if (reg_alloc.count(input)) {
        dot_output << input_name << " [label=< <font color=\"green\">Input[" << reg_alloc[input] << "]</font> >];\n";
    } else {
        dot_output << input_name << " [label=< <font color=\"green\">Input</font> >];\n";
    }
}

void DotVisitor::visit(Add* node) {
    std::string add_name = generate_node_name();
    node_map[node] = add_name;

    if (reg_alloc.count(node)) {
        dot_output << add_name << " [label=< <font color=\"black\">+[" << reg_alloc[node] << "]</font> >];\n";
    } else {
        dot_output << add_name << " [label=< <font color=\"black\">+</font> >];\n";
    }

    node->get_left()->accept(this);
    node->get_right()->accept(this);

    dot_output << add_name << " -- " << node_map[node->get_left().get()] << ";\n";
    dot_output << add_name << " -- " << node_map[node->get_right().get()] << ";\n";
}

void DotVisitor::visit(Subtract* node) {
    std::string sub_name = generate_node_name();
    node_map[node] = sub_name;

    if (reg_alloc.count(node)) {
        dot_output << sub_name << " [label=< <font color=\"black\">-[" << reg_alloc[node] << "]</font> >];\n";
    } else {
        dot_output << sub_name << " [label=< <font color=\"black\">-</font> >];\n";
    }

    node->get_left()->accept(this);
    node->get_right()->accept(this);

    dot_output << sub_name << " -- " << node_map[node->get_left().get()] << ";\n";
    dot_output << sub_name << " -- " << node_map[node->get_right().get()] << ";\n";
}

void DotVisitor::visit(Multiply* node) {
    std::string mul_name = generate_node_name();
    node_map[node] = mul_name;

    if (reg_alloc.count(node)) {
        dot_output << mul_name << " [label=< <font color=\"black\">*[" << reg_alloc[node] << "]</font> >];\n";
    } else {
        dot_output << mul_name << " [label=< <font color=\"black\">*</font> >];\n";
    }

    node->get_left()->accept(this);
    node->get_right()->accept(this);

    dot_output << mul_name << " -- " << node_map[node->get_left().get()] << ";\n";
    dot_output << mul_name << " -- " << node_map[node->get_right().get()] << ";\n";
}

void DotVisitor::visit(Divide* node) {
    std::string div_name = generate_node_name();
    node_map[node] = div_name;

    if (reg_alloc.count(node)) {
        dot_output << div_name << " [label=< <font color=\"black\">/[" << reg_alloc[node] << "]</font> >];\n";
    } else {
        dot_output << div_name << " [label=< <font color=\"black\">/</font> >];\n";
    }

    node->get_left()->accept(this);
    node->get_right()->accept(this);

    dot_output << div_name << " -- " << node_map[node->get_left().get()] << ";\n";
    dot_output << div_name << " -- " << node_map[node->get_right().get()] << ";\n";
}

// Get the final DOT representation
std::string DotVisitor::get_dot_representation(std::shared_ptr<Node> graph, std::unordered_map<Node*, std::string> var_names) {
    node_map = var_names;
    dot_output.str("");

    graph->accept(this);
    std::ostringstream final_output;
    final_output << "graph computation_graph {\n";
    final_output << "edge [dir=none];\nnode [shape=none];\n";
    final_output << dot_output.str();
    final_output << "}\n";
    return final_output.str();
}

std::string DotVisitor::get_dot_reg_representation(std::shared_ptr<Node> graph, std::unordered_map<Node*, std::string> var_names, std::unordered_map<Node*, int> ralloc) {
    node_map = var_names;
    reg_alloc = ralloc;
    dot_output.str("");

    graph->accept(this);
    std::ostringstream final_output;
    final_output << "graph computation_graph {\n";
    final_output << "edge [dir=none];\nnode [shape=none];\n";
    final_output << dot_output.str();
    final_output << "}\n";
    return final_output.str();
}

// Function to write DOT representation to a file
void write_dot_file(const std::string& filename, const std::string& dot_content) {
    std::ofstream file("./dot-files/" + filename);
    if (file.is_open()) {
        file << dot_content;
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

// Function to generate an image from a DOT file using Graphviz
void generate_image_from_dot(const std::string& dot_filename, const std::string& image_filename) {
    std::string command = "dot -Tpng ./dot-files/" + dot_filename + " -o " + "./graph-images/" + image_filename;
    std::system(command.c_str());  // Call system command to generate image
}