#pragma once

#include "CoreMinimal.h"

class State;
using StateTransitionFunction = TFunction<bool()>;

/**
 * A one‐way link between two States guarded by a bool predicate.
 */
class StateTransition {
public:
    StateTransition(State* InSource, State* InDest, StateTransitionFunction InFunc)
        : SourceState(InSource)
        , DestinationState(InDest)
        , Function(MoveTemp(InFunc))
    {}

    /** True when the transition should fire */
    bool CanTransition() const {
        return Function ? Function() : false;
    }

    /** Where to go if CanTransition() */
    State* GetDestinationState() const {
        return DestinationState;
    }
    /** Which state this transition fires from */
    State* GetSourceState() const {
        return SourceState;
    }

private:
    State* SourceState         = nullptr;
    State* DestinationState    = nullptr;
    StateTransitionFunction    Function;
};
