.assembly extern legacy library mscorlib {}
.assembly extern System.Runtime {}

.assembly 'Interpreter'{}

.class public auto ansi beforefieldinit InterpreterTest
    extends [System.Runtime]System.Object
{
    // Methods
    .method private hidebysig static
        int32 Main (
            string[] args
        ) cil managed
    {
        .custom instance void [System.Runtime]System.Runtime.CompilerServices.NullableContextAttribute::.ctor(uint8) = (
            01 00 01 00 00
        )
        .entrypoint

        // Method begins at RVA 0x2050
        // Code size 14 (0xe)
        .maxstack 1
        .locals init (
            [0] int32
        )

        IL_0000: nop
        IL_0001: call void InterpreterTest::RunInterpreterTests()
        IL_0006: nop
        IL_0007: ldc.i4.s 100
        IL_0009: stloc.0
        IL_000a: br.s IL_000c

        IL_000c: ldloc.0
        IL_000d: ret
    } // end of method InterpreterTest::Main

    .method public hidebysig static
        void RunInterpreterTests () cil managed noinlining
    {
        // Method begins at RVA 0x206c
        // Code size 10 (0xa)
        .maxstack 1

        IL_0000: nop
        .try
        {
            IL_0001: nop
            IL_0002: ldnull
            IL_0003: throw
        } // end .try
        catch [System.Runtime]System.Exception
        {
            IL_0004: pop
            IL_0005: nop
            IL_0006: nop
            IL_0007: leave IL_0009
        } // end handler

        IL_0009: ret
    } // end of method InterpreterTest::RunInterpreterTests

} // end of class InterpreterTest

