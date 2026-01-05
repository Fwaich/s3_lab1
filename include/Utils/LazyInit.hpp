#pragma once
#include "Sequence.hpp"
#include "LazySequence.hpp"
#include "OperationParser.hpp"
#include "SharedPtr.hpp"

Shared_Ptr<Lazy_Sequence<int>> init_by_prod_func();
Shared_Ptr<Lazy_Sequence<int>> init_by_sequence();
Shared_Ptr<Lazy_Sequence<int>> init_new_lazy_seq();
