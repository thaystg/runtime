// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Runtime.ExceptionServices;

public static class RuntimeExceptionFilterPerf
{
    private const int Success = 100;
    private const int NotConfigured = 101;
    private const int DefaultIterations = 20_000;
    private const int DefaultSamples = 7;

    private static int s_firstChanceNotifications;

    public static int Main(string[] args)
    {
        bool allowUnattached = args.Length > 0 &&
            args[0].Equals("--allow-unattached", StringComparison.Ordinal);
        int scenarioArgument = allowUnattached ? 1 : 0;

        if (!Debugger.IsAttached && !allowUnattached)
        {
            Console.Error.WriteLine(
                "Attach a managed debugger before running this benchmark.");
            return NotConfigured;
        }

        if (args.Length <= scenarioArgument ||
            !TryParseScenario(args[scenarioArgument], out Scenario scenario))
        {
            Console.Error.WriteLine(
                "Usage: runtimeexceptionfilterperf [--allow-unattached] <baseline|filtered> [--no-header] [iterations] [samples]");
            return NotConfigured;
        }

        if (allowUnattached)
        {
            Console.WriteLine("Unattached smoke mode does not measure the runtime exception filter.");
        }

        int firstNumericArgument = scenarioArgument + 1;
        bool writeHeader = args.Length <= firstNumericArgument ||
            !args[firstNumericArgument].Equals("--no-header", StringComparison.Ordinal);
        if (!writeHeader)
        {
            firstNumericArgument++;
        }

        int iterations = args.Length > firstNumericArgument ?
            int.Parse(args[firstNumericArgument]) :
            DefaultIterations;
        int samples = args.Length > firstNumericArgument + 1 ?
            int.Parse(args[firstNumericArgument + 1]) :
            DefaultSamples;

        if (iterations <= 0 || samples <= 0)
        {
            throw new ArgumentOutOfRangeException(
                iterations <= 0 ? nameof(iterations) : nameof(samples),
                "Benchmark arguments must be positive.");
        }

        ValidateFirstChanceNotifications();
        WarmUp();

        PrintScenarioInstructions(scenario);
        if (writeHeader)
        {
            Console.WriteLine("scenario,depth,iterations,median_ms,ns_per_throw,throws_per_second,legacy_expected_filter_evaluations,optimized_expected_filter_evaluations,evaluation_reduction_percent");
        }

        foreach (int depth in new[] { 1, 8, 32, 64 })
        {
            RunBenchmark(scenario, depth, iterations, samples);
        }

        return Success;
    }

    private static bool TryParseScenario(string value, out Scenario scenario)
    {
        if (value.Equals("baseline", StringComparison.OrdinalIgnoreCase))
        {
            scenario = Scenario.Baseline;
            return true;
        }

        if (value.Equals("filtered", StringComparison.OrdinalIgnoreCase))
        {
            scenario = Scenario.Filtered;
            return true;
        }

        scenario = default;
        return false;
    }

    private static void PrintScenarioInstructions(Scenario scenario)
    {
        if (scenario == Scenario.Baseline)
        {
            Console.Error.WriteLine(
                $"BASELINE: do not install the runtime filter. The debugger must receive and continue every caught " +
                $"{typeof(FilteredException).FullName} first-chance callback through DBI.");
        }
        else
        {
            Console.Error.WriteLine(
                $"FILTERED: install the runtime filter so CoreCLR suppresses caught " +
                $"{typeof(FilteredException).FullName} before the DBI callback.");
        }
    }

    private static void ValidateFirstChanceNotifications()
    {
        const int validationThrows = 100;
        EventHandler<FirstChanceExceptionEventArgs> handler = (_, eventArgs) =>
        {
            if (eventArgs.Exception is FilteredException)
            {
                s_firstChanceNotifications++;
            }
        };

        AppDomain.CurrentDomain.FirstChanceException += handler;
        try
        {
            for (int i = 0; i < validationThrows; i++)
            {
                ThrowAndCatch(8);
            }
        }
        finally
        {
            AppDomain.CurrentDomain.FirstChanceException -= handler;
        }

        if (s_firstChanceNotifications != validationThrows)
        {
            throw new InvalidOperationException(
                $"Expected {validationThrows} managed first-chance notifications, observed {s_firstChanceNotifications}.");
        }
    }

    private static void WarmUp()
    {
        for (int i = 0; i < 2_000; i++)
        {
            ThrowAndCatch(8);
        }
    }

    private static void RunBenchmark(Scenario scenario, int depth, int iterations, int samples)
    {
        double[] elapsedMilliseconds = new double[samples];

        for (int sample = 0; sample < samples; sample++)
        {
            long start = Stopwatch.GetTimestamp();
            for (int i = 0; i < iterations; i++)
            {
                ThrowAndCatch(depth);
            }

            elapsedMilliseconds[sample] = Stopwatch.GetElapsedTime(start).TotalMilliseconds;
        }

        Array.Sort(elapsedMilliseconds);
        double medianMilliseconds = elapsedMilliseconds[samples / 2];
        double nanosecondsPerThrow = medianMilliseconds * 1_000_000 / iterations;
        double throwsPerSecond = iterations * 1_000 / medianMilliseconds;
        long optimizedEvaluations = iterations;
        long legacyEvaluations = (long)iterations * (depth + 2);
        double evaluationReduction = 100.0 * (legacyEvaluations - optimizedEvaluations) / legacyEvaluations;

        Console.WriteLine(
            $"{scenario.ToString().ToLowerInvariant()},{depth},{iterations},{medianMilliseconds:F3}," +
            $"{nanosecondsPerThrow:F1},{throwsPerSecond:F0}," +
            $"{legacyEvaluations},{optimizedEvaluations},{evaluationReduction:F2}");
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void ThrowAndCatch(int depth)
    {
        try
        {
            ThrowAtDepth(depth);
        }
        catch (FilteredException)
        {
        }
    }

    [MethodImpl(MethodImplOptions.NoInlining)]
    private static void ThrowAtDepth(int depth)
    {
        if (depth == 0)
        {
            throw new FilteredException();
        }

        ThrowAtDepth(depth - 1);
    }

    private sealed class FilteredException : Exception
    {
    }

    private enum Scenario
    {
        Baseline,
        Filtered
    }
}
