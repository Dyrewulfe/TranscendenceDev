//	CCInteger.cpp
//
//	Implements CCInteger class

#include <windows.h>
#include "Kernel.h"
#include "KernelObjID.h"
#include "CodeChain.h"
#include <math.h>

static CObjectClass<CCInteger>g_Class(OBJID_CCINTEGER, NULL);