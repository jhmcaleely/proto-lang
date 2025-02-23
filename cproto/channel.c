#include <stdio.h>
#include "pico/stdlib.h"

#include "common.h"

#include "channel.h"
#include "memory.h"
#include "vm.h"
#include "debug.h"

ObjChannel* newChannel() {
    ObjChannel* channel = ALLOCATE_OBJ(ObjChannel, OBJ_CHANNEL);
    channel->data = NIL_VAL;
    channel->present = false;
    channel->overflow = false;
    return channel;
}

bool makeChannelBuiltin(ObjRoutine* routine, int argCount, Value* args, Value* result) {
    if (argCount != 0) {
        runtimeError(routine, "Expected 0 arguments but got %d.", argCount);
        return false;
    }

    ObjChannel* channel = newChannel();

    *result = OBJ_VAL(channel);
    return true;
}

bool sendChannelBuiltin(ObjRoutine* routine, int argCount, Value* args, Value* result) {
    if (argCount != 2) {
        runtimeError(routine, "Expected 2 arguments, got %d.", argCount);
        return false;
    }
    if (!IS_CHANNEL(args[0])) {
        runtimeError(routine, "Argument must be a channel.");
        return false;
    }

    ObjChannel* channel = AS_CHANNEL(args[0]);
    while (channel->present) {
        // stall/block until space
        tight_loop_contents();
    }

    channel->data = args[1];
    channel->present = true;
    channel->overflow = false;
    *result = BOOL_VAL(channel->overflow);

    return true;
}

bool receiveChannelBuiltin(ObjRoutine* routine, int argCount, Value* args, Value* result) {
    if (argCount != 1) {
        runtimeError(routine, "Expected 1 arguments, got %d.", argCount);
        return false;
    }
    if (!IS_CHANNEL(args[0]) && !IS_ROUTINE(args[0])) {
        runtimeError(routine, "Argument must be a channel or a routine.");
        return false;
    }

    if (IS_CHANNEL(args[0])) {
        ObjChannel* channel = AS_CHANNEL(args[0]);
        while (!channel->present) {
            // stall/block until space
            tight_loop_contents();
        }

        *result = channel->data;
        channel->present = false;
        channel->overflow = false;

    } 
    else if (IS_ROUTINE(args[0])) {
        ObjRoutine* routine = AS_ROUTINE(args[0]);

        while (routine->state == EXEC_RUNNING) {
            tight_loop_contents();
        }
        
        if (routine->state == EXEC_CLOSED || routine->state == EXEC_SUSPENDED) {
            *result = *routine->stackTop;
        } 
        else {
            *result = NIL_VAL;
        }
    }
    
    return true;
}

bool shareChannelBuiltin(ObjRoutine* routine, int argCount, Value* args, Value* result) {
    if (argCount != 2) {
        runtimeError(routine, "Expected 2 arguments, got %d.", argCount);
        return false;
    }
    if (!IS_CHANNEL(args[0])) {
        runtimeError(routine, "Argument must be a channel.");
        return false;
    }

    ObjChannel* channel = AS_CHANNEL(args[0]);
    if (channel->present) {
        channel->overflow = true;
    }
    channel->data = args[1];
    channel->present = true;
    *result = BOOL_VAL(channel->overflow);

    return true;
}

bool peekChannelBuiltin(ObjRoutine* routine, int argCount, Value* args, Value* result) {
    if (argCount != 1) {
        runtimeError(routine, "Expected 1 arguments, got %d.", argCount);
        return false;
    }
    if (!IS_CHANNEL(args[0])) {
        runtimeError(routine, "Argument must be a channel.");
        return false;
    }

    ObjChannel* channel = AS_CHANNEL(args[0]);
    if (channel->present) {
        *result = channel->data;
    }
    else {
        *result = NIL_VAL;
    }

    return true;
}