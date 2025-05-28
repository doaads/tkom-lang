#pragma once

#include "local_function.h"

void __stdout(ArgVector& args);
void __increment(ArgVector& args);
extern const std::vector<std::shared_ptr<Callable>> builtins;

