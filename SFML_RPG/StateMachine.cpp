#include "StateMachine.h"
#include "State.h"

#include "Log.h"



lbe::StateMachine::StateMachine()
{
}

lbe::StateMachine::~StateMachine()
{
}

void lbe::StateMachine::Run(std::unique_ptr<State> state)
{
	bIsRunning = true;

	States.push(std::move(state));
}

void lbe::StateMachine::NextState()
{
	if (bResume)
	{
		// Cleanup the current state
		if (!States.empty())
		{
			States.pop();
		}

		// Resume previous state
		if (!States.empty())
		{
			States.top()->Resume();
		}
		else
		{
			Quit();
		}

		bResume = false;
	}

	// There needs to be a state
	if (!States.empty())
	{
		std::unique_ptr<State> temp = States.top()->Next();

		// Only change states if there's a next one existing
		if (temp != nullptr)
		{
			// Replace the running state
			if (temp->GetbIsReplacing())
				States.pop();
			// Pause the running state
			else
				States.top()->Pause();

			States.push(std::move(temp));
		}
	}
}

void lbe::StateMachine::LastState()
{
	bResume = true;
}

void lbe::StateMachine::Update()
{
	if (!States.empty())
	{
		States.top()->Update();
	}
}

void lbe::StateMachine::Draw()
{
	if (!States.empty())
	{
		States.top()->Draw();
	}
}