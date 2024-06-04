#pragma once

#if defined(_DEBUG) || defined(DEBUG)
#define DEBUG_EXEC(statement) statement
#else
#define DEBUG_EXEC(statement)
#endif
