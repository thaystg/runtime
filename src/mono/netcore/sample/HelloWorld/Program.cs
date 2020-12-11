// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Threading;
using System.Diagnostics;

namespace HelloWorld
{
    public class Thays
    {
        public long myLong;
        public int myInt;
        public char myLetter;
        public bool myBool;
        public string myString;
        public Thays(int c)
        {
            myLong = c + 10000;
            myInt = c + 10;
            myLetter = 'a';
            myBool = c == 50;
            myString = "thaysgrazia";
        }
    }

    internal class Program
    {
        private static void Main(string[] args)
        {
            bool isMono = typeof(object).Assembly.GetType("Mono.RuntimeStructs") != null;
            Console.WriteLine($"Hello World {(isMono ? "from Mono!" : "from CoreCLR!")}");
            Console.WriteLine(typeof(object).Assembly.FullName);
            Console.WriteLine(System.Reflection.Assembly.GetEntryAssembly ());
            Console.WriteLine(System.Runtime.InteropServices.RuntimeInformation.FrameworkDescription);
            Console.WriteLine("Waiting for debugger to attach");
            while (!Debugger.IsAttached)
            {
                Thread.Sleep(1000);
                Console.WriteLine("Waiting for debugger to attach");
            }
            AnotherFunctionCall(50);
            Console.WriteLine("Debugger attached");
        }
        private static void AnotherFunctionCall(int c)
        {
            long myLong = c + 10000;
            int myInt = c + 10;
            char myLetter = 'a';
            bool myBool = c == 50;
            string myString = "thays";
            Thays t = new Thays(c + 1);
            myString = myString + myLetter;
            Console.WriteLine(myLetter);
            Console.WriteLine(myBool);
            Console.WriteLine(myLong);
            Console.WriteLine(myInt);
            Console.WriteLine(myString);
            Console.WriteLine(t.ToString());
        }
    }
}
