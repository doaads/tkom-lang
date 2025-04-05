#include "handler_manager.h"
#include "dfa_states.h"
#include <memory>
#include <stdexcept>

HandlerManager::HandlerManager() {
    state_handlers[DFAState::IN_WHITESPACE] = std::make_unique<InWhitespaceHandler>();
    state_handlers[DFAState::IN_IDENTIFIER] = std::make_unique<InIdentifierHandler>();
    state_handlers[DFAState::IN_NUMBER] = std::make_unique<InNumberHandler>();
    state_handlers[DFAState::IN_FLT] = std::make_unique<InFltHandler>();
    state_handlers[DFAState::IN_STRING] = std::make_unique<InStringHandler>();
    state_handlers[DFAState::IN_ESCAPE] = std::make_unique<InEscapeHandler>();
    state_handlers[DFAState::IN_OPERATOR] = std::make_unique<InOperatorHandler>();
    state_handlers[DFAState::IN_LONG_OPERATOR] = std::make_unique<InLongOperatorHandler>();
    state_handlers[DFAState::IN_FIRST_CHAR_LONG_OP] = std::make_unique<InFirstCharLongOpHandler>();
};

std::unique_ptr<StateHandler>& HandlerManager::operator[](DFAState state) {
    auto it = state_handlers.find(state);
    if (it == state_handlers.end()) {
        throw std::out_of_range("Handler not found for the specified state.");
    }
    return it->second;
}
