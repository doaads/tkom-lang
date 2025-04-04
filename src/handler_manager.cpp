#include "handler_manager.h"
#include "dfa_states.h"
#include <memory>
#include <stdexcept>

HandlerManager::HandlerManager() {
    state_handlers[DFAState::IN_WHITESPACE] = std::make_unique<InWhitespaceHandler>();
    state_handlers[DFAState::IN_IDENTIFIER] = std::make_unique<InIdentifierHandler>();
    state_handlers[DFAState::IN_NUMBER] = std::make_unique<InNumberHandler>();
};

std::unique_ptr<StateHandler>& HandlerManager::operator[](DFAState state) {
    auto it = state_handlers.find(state);
    if (it == state_handlers.end()) {
        throw std::out_of_range("Handler not found for the specified state.");
    }
    return it->second;
}
