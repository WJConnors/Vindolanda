#pragma once

#include "CoreMinimal.h"

using StateUpdateFunction = TFunction<void()>;

/**
 * A simple updatable state that calls a stored function each tick.
 */
class State {
public:
    State() = default;
    explicit State(StateUpdateFunction InFunc)
        : Func(MoveTemp(InFunc)) {}

    /** Called each Update() with DeltaTime */
    void Update() {
        if (Func) {
            Func();
        }
    }

private:
    StateUpdateFunction Func;
};
