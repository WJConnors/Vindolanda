#include "StateMachine.h"
#include "State.h"
#include "StateTransition.h"

StateMachine::StateMachine() = default;

StateMachine::~StateMachine() {
    for (State* S : AllStates) {
        delete S;
    }
    for (auto& Pair : AllTransitions) {
        delete Pair.Value;
    }
}

void StateMachine::AddState(State* InState) {
    AllStates.Add(InState);
    if (!ActiveState) {
        ActiveState = InState;
    }
}

void StateMachine::AddTransition(StateTransition* InTransition) {
    AllTransitions.Add(InTransition->GetSourceState(), InTransition);
}

void StateMachine::Update(float DeltaTime) {
    if (!ActiveState) {
        return;
    }

    // Update current state
    ActiveState->Update(DeltaTime);

    // Check all outgoing transitions
    for (auto It = AllTransitions.CreateConstKeyIterator(ActiveState); It; ++It) {
        StateTransition* Transition = It.Value();
        if (Transition && Transition->CanTransition()) {
            ActiveState = Transition->GetDestinationState();
            break;
        }
    }
}
