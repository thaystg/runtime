# Interpreter Debugger Prototype

This document talks about my prototype for supporting managed debugging for the interpreter. The document will cover:

- Stack walking support (so that we can inspect variables in SOS)
- Debug info generation using frame pointer based variables
- Interpreted Tier label for debugger recognition
- Breakpoint support in MDbg - prototyping - work
- Inspection support in MDbg - prototyping - work
- Stepping support in MDbg - does not work

This is like a status report, but also wanted to spark some discussion on how to handling stepping and its dependency on frame pointer.

## Stackwalking support

The interpreter needed stack walking regardless for many purposes. To understand stack walking, we can start with understanding the data structures first.

We have a capital Frame on the thread static as `InterpreterFrame`, whenever start running interpreted code, we push this onto the capital Frame chain.

When interpreter code executes, internally, it uses a `InterpMethodContextFrame`, this is NOT a capital frame, and it does NOT contain the actual variables, it only has a linked list of these `InterpMethodContextFrame` so that we can call and return.

On an `InterpMethodContextFrame`, we have a `pStack` pointer that points to an array, that array is where the variables actually lives.

Jan Vorlicek implemented the stack walking support using these data structures. It works for handling exceptions (work in progress), GC reporting (work in progress) and "!clrstack -i -a" on SOS.

## Frame pointer based variables

To support "!clrstack -i -a", we need boundaries and vars. This is implemented in the `interpreter\compiler.cpp`. This is a compiler that converts the MSIL (together with some extra info that it queries for) in IR which `interpexec.cpp` interprets. The compilation process is augmented to report the MSIL to IR offset mapping, as well as the life time for the local variables and their offsets from `pStack`.

For stack walking purposes, the API reports a `CONTEXT` for each frame, but obviously `InterpMethodContextFrame` doesn't really have a `CONTEXT`, so we are free to report information through it. Right now, we have 

```
IP = pointer to the bytecode to be interpreted
SP = pointer to InterpMethodContextFrame
FP = pStack
```

The variables are stored on `pStack`, that's why the debug info reporting is frame pointer based. Otherwise they are the same as regular jitted code.

## Interpreted Tier label for debugger recognition

To make breakpoint works, we need to tell the debugger to "code" that the debugger reports is actually interpreter bytecode, and I chose to do it through the "Tier" in the `NativeCodeVersion`. In particular, I introduced interpreted tier to express the fact that the code is interpreter byte code.

The implicit nature of the code versioning design made that difficult. What I really wanted is that the code has just one version and it is interpreted, but it looks like I have to create a new version in order to do that. Would love to see if that could be changed.

## Breakpoint support in MDbg - prototyping - work

With the tiering label, I am able to react to the debugger's request to set a breakpoint. If the code is not translated yet, we will simply create a patch and the patch table just like it was not jitted yet. Then by the time the code is translated, the normal callback that would tell the debugger to convert the patch will now recognize the code is interpreter byte code, so it change the patch's type to an new patch type, and patch the interpreter opcode to a special debug opcode.

When execution reaches the debug opcode, the interpreter will raise a special exception to mimic a true breakpoint, but of course the break location is in native code. With the special code, we had a chance to patch the context so that it looks like what the stack walker would have produced (i.e. ip/sp/fp are all set to interpreter values) and then reuse the existing breakpoint logic.

It works, that will get recognized by the code manager as managed code, and will send the IPC message to tell the debugger we hit a breakpoint.

When the debugger send an IPC message to continue the process, we will find the patch, recognizing this is a special type of patch, and it will simply restore the opcode and resume the process from the exception handled.

# Inspection support in MDbg - prototyping - work

There is nothing special I need to do with this, just like it worked in SOS, it worked here as well.

# Stepping support in MDbg - does not work

I moved on and tried stepping, but then it appears we have a problem. The stepper really don't like our fake frame pointer.