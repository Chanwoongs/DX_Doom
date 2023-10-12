#ifndef ENEMYSTATES_H
#define ENEMYSTATES_H

#include "State.h"

#ifndef StateID

#define PATROL 1
#define APPROACH 2
#define ATTACK 3
#define HITTED 4
#define DEAD 5

#endif


class EnemyClass;

class Patrol : public State<EnemyClass>
{
private:
  
	Patrol() {}
	
	Patrol(const Patrol&);
	Patrol& operator=(const Patrol&);

	float timer;


public:

  //this is a singleton
  static Patrol* Instance();

  virtual void Enter(EnemyClass* enemy);

  virtual void Execute(EnemyClass* enemy, float deltaTime);

  virtual void Exit(EnemyClass* enemy);

  virtual int GetStateID();

};

class Approach : public State<EnemyClass>
{
private:
  
	Approach(){}

	Approach(const Approach&);
	Approach& operator=(const Approach&);

	float timer;
 
public:

  //this is a singleton
  static Approach* Instance();

  virtual void Enter(EnemyClass* enemy);

  virtual void Execute(EnemyClass* enemy, float deltaTime);

  virtual void Exit(EnemyClass* enemy);

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

  virtual void Enter(EnemyClass* enemy);

  virtual void Execute(EnemyClass* enemy, float deltaTime);

  virtual void Exit(EnemyClass* enemy);

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

  virtual void Enter(EnemyClass* enemy);

  virtual void Execute(EnemyClass* enemy, float deltaTime);

  virtual void Exit(EnemyClass* enemy);

  virtual int GetStateID();
};

class Dead : public State<EnemyClass>
{
private:

	Dead() {}

	Dead(const Dead&);
	Dead& operator=(const Dead&);

public:

	//this is a singleton
	static Dead* Instance();

	virtual void Enter(EnemyClass* enemy);

	virtual void Execute(EnemyClass* enemy, float deltaTime);

	virtual void Exit(EnemyClass* enemy);

	virtual int GetStateID();
};




#endif
