#ifndef ENEMYSTATES_H
#define ENEMYSTATES_H

#include "State.h"

#ifndef StateID

#define PATROL 1
#define APPROACH 2
#define ATTACK 3
#define HITTED 4

#endif


class EnemyClass;

class Patrol : public State<EnemyClass>
{
private:
  
	Patrol() {}
	
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);

public:

  //this is a singleton
  static Patrol* Instance();

  virtual void Enter(EnemyClass* woong);

  virtual void Execute(EnemyClass* woong);

  virtual void Exit(EnemyClass* woong);

  virtual int GetStateID();

};

class Approach : public State<EnemyClass>
{
private:
  
	Approach(){}

	Approach(const Approach&);
	Approach& operator=(const Approach&);
 
public:

  //this is a singleton
  static Approach* Instance();

  virtual void Enter(EnemyClass* woong);

  virtual void Execute(EnemyClass* woong);

  virtual void Exit(EnemyClass* woong);

  virtual int GetStateID();
};

class Attack : public State<EnemyClass>
{
private:
  
	Attack(){}

	Attack(const Attack&);
	Attack& operator=(const Attack&);
 
public:

  //this is a singleton
  static Attack* Instance();

  virtual void Enter(EnemyClass* woong);

  virtual void Execute(EnemyClass* woong);

  virtual void Exit(EnemyClass* woong);

  virtual int GetStateID();
};

class Hitted : public State<EnemyClass>
{
private:
  
	Hitted(){}

	Hitted(const Hitted&);
	Hitted& operator=(const Hitted&);
 
public:

  //this is a singleton
  static Hitted* Instance();

  virtual void Enter(EnemyClass* woong);

  virtual void Execute(EnemyClass* woong);

  virtual void Exit(EnemyClass* woong);

  virtual int GetStateID();
};




#endif
