#ifndef STATE_H
#define STATE_H

template <class type>
class State
{
public:
	virtual ~State(){}

	//this will execute when the state is entered
	virtual void Enter(type*)=0;

	//this is the states normal update function
	virtual void Execute(type*, float)=0;

	//this will execute when the state is exited. (My word, isn't
	//life full of surprises... ;o))
	virtual void Exit(type*)=0;

	virtual int GetStateID()=0;
};

#endif