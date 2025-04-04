#pragma once

#include <map>
#include <memory>
#include "handlers.h"
#include "dfa_states.h"


class HandlerManager {
    private:
        std::map<DFAState, std::unique_ptr<StateHandler>> state_handlers;
    public:
        HandlerManager();
        std::unique_ptr<StateHandler>& operator[](DFAState state);
};
