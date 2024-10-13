#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <memory>
#include "../../print-visitor/headers/print_visitor.h"
#include "../../register-allocator/headers/register_allocator.h"
#include "../../grad-visitor/headers/grad_visitor.h"
#include "../../jit/headers/jit.h"
#include "../../eval-visitor/headers/eval_visitor.h"
#include "../../dot-visitor/headers/dot_visitor.h"

void deriv();