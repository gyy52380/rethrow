#pragma once

//get length of static array
#define LEN(array) ((int)(sizeof(array)/sizeof(array[0])))

//execute a block of code only once
#define DO_ONCE_Name2(x, y) DO_ONCE_##x##_##y
#define DO_ONCE_Name(x, y) DO_ONCE_Name2(x, y)
#define DO_ONCE_Final(name) static bool name = false; if (!name && (name = true))
#define DO_ONCE DO_ONCE_Final(DO_ONCE_Name(_LINE,COUNTER_))