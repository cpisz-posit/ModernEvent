# ModernEvent
Demonstration and comparison of how one might possibly implement event types in response to recent protests to using inheritance and/or dynamic_cast.

# Benchmark
First build using the RelWithDebugInfo preset, as that is best for profiling.

To put the benchmark results in a file, run it with args like so:
```
/build/relwithdebuginfo/src/eventapp/eventapp --benchmark_out=results.json --benchmark_out_format=json --benchmark_time_unit=ms
```

Then check the file:
```
cat /workspace/result.json
```

Time, CPU, and Real Time are average times per iteration. Iterations column shows how many times the benchmark function was executed to gather the measurements.
The framework decides the number of iterations automatically based on timing precision and minimum runtime.

## Method 0 - Classic inheritance
Using a type identifier to dynamic cast to a concrete type which holds the data of interest

Pros
Cons
- Requires a dynamic_cast
- Changes to base classes may require changes to all subclasses

My opinion: Meh. If we can do better easily, let's.

## Method 1 - Pure composition using std::variant and std::visit
If we follow the directive "prefer composition over inheritance when possible" in the most naive and literal way

Pros
- No inheritance
- No virtual dispatch
- Can add new event types
- Can add data members to events without effecting other objects
Cons
- Constructors are complicated: Example: `purecomplib::AuthLoginEvent authLoginEvent{ {{9876, std::chrono::system_clock::now()} , "fred"} };`
- Accessing member data in superclass is ugly! Example: `concreteEvent.sessionBaseData_.eventBaseData_.pid_`
- Error messages are hard to make sense out of
- Must update variant lists when adding new types.
- A bit more compile time and artifact size compared to inheritance due to std::visit and std:variant

My opinion: Not worth it just to circumvent two levels of inheritance.

## Method 2 - Inheritance but no dynamic cast
Not implemented yet
Heard a tale about using template methods to safely (or at least encapsulate the UB danger) of using static_cast instead of dynamic_cast

Pros
- Stuff
Cons
- Stuff

My opinion: NA

## Method 3 - CRTP
Not implemented yet

Pros
- Stuff
Cons
- Stuff

My opinion: NA