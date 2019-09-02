#include "State.h"



lbe::State::State(StateMachine & Machine, sf::RenderWindow & Window, bool Replace) : Machine(Machine), Window(Window), bIsReplacing(Replace)
{
}

lbe::State::~State()
{
}

std::unique_ptr<lbe::State> lbe::State::Next()
{
	return std::move(NextState);
}

bool lbe::State::GetbIsReplacing()
{
	return bIsReplacing;
}
