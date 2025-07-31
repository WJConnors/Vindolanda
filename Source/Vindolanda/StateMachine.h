#pragma once

#include "CoreMinimal.h"

class State;
class StateTransition;

/** Map a source State → one or more transitions */
using TransitionContainer = TMultiMap<State*, StateTransition*>;

/**
 * A minimal finite‐state‐machine: holds States and transitions between them.
 */
class StateMachine {
public:
    StateMachine();
    virtual ~StateMachine();

    /** Take ownership of a new State */
    void AddState(State* InState);

    /** Take ownership of a new Transition */
    void AddTransition(StateTransition* InTransition);

    /** Call this every frame with DeltaTime */
    virtual void Update(float DeltaTime);

protected:
    /** The currently active state */
    State* ActiveState = nullptr;

    /** All states owned by this machine */
    TArray<State*> AllStates;

    /** All transitions, keyed by their source State */
    TransitionContainer AllTransitions;
};
