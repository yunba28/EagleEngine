#pragma once

#include <cassert>

#if _DEBUG
#define ensure(predicate,message) assert((message,predicate))
#else
#define ensure(predicate,message) (void)predicate
#endif
