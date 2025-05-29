# ModernEvent
Demonstration of how one might possibly implement event types without inheritance while allowing for them to be extensible in the future.

Pros
- No inheritance
- No virtual dispatch
- Can add new event types
- Can add data members to events without effecting other objects
Cons
- Error messages are REALLY ugly
- Must update variants and handlers when adding new types
- A bit more compile time and artifact size compared to inheritance due to std::visit and std:variant
