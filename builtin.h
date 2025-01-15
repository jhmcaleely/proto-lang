#ifndef clox_builtin_h
#define clox_builtin_h

#include "value.h"

bool makeIsrBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);
bool makeCoroBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);
bool makeMainBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);
bool resumeBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);
bool startBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);
bool makeChannelBuiltin(ObjThreadStack* thread, int argCount, Value* args, Value* result);

#endif