# ModernEvent
Demonstration and comparison of how one might possibly implement event types in response to recent protests to using inheritance and/or dynamic_cast.

## Method 0 - Classic inheritance
Pros
Cons
- Requires a dynamic_cast

My opinion: Meh. If we can do better easily, let's.

## Method 1 - Pure composition using std::variant and std::visit
Pros
- No inheritance
- No virtual dispatch
- Can add new event types
- Can add data members to events without effecting other objects
Cons
- Constructors are complicated: Example: `purecomplib::AuthLoginEvent authLoginEvent{ {{9876, std::chrono::system_clock::now()} , "fred"} };`
- Accessing member data in superclass is ugly! Example: `concreteEvent.sessionBaseData_.eventBaseData_.pid_`
- Error messages are hard to make sense out of
- Must update variants and handlers, when adding new types.
- A bit more compile time and artifact size compared to inheritance due to std::visit and std:variant

My opinion: Not worth it just to circumvent two levels of inheritance.

## Method 2 - Inheritance but no dynamic cast
Pros
- Stuff
Cons
- Stuff

My opinion: I dig it