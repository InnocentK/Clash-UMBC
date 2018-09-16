/**************************************
 ** File:    Troop.cpp
 ** Project: CMSC 202 Project 2 - Clash UMBC, Fall 2017
 ** Author:  Innocent Kironji
 ** Date:    10/19/17
 ** Section: 0107
 ** E-mail:  wambugu1@umbc.edu
 **
 ** This file contains the implimentation for all member functions of the
 ** troop class. The functions are for setting basic parameters for a Troop
 ** object (ex. troop's name, health, damage strength, target capabilities,
 ** if the troop is flying). It also includes the Attack method for 
 ** determining how a battle will occur.
 ***************************************/

#include "Troop.h"

// Troop() - Overloaded constructor
// Builds a new troop
// Preconditions - Requires name, health, damage, target, and isFlying 
//                   (from file)
Troop::Troop(string name, int health, int damage, string target
	     , bool isFlying){

  SetName(name);
  SetHealth(health);

  // Input validation for damage
  if (damage > 0){
    m_damage = damage;
  }else
    m_damage = 1;

  // Input validation for target
  if( (target == "Ground") || (target == "Both") ){
    m_target = target;
  }
  else if (target == "ground"){
    m_target = "Ground";
  }
  else if (target == "both"){
    m_target = "Both";
  }
  // Default if input is incorrect
  else
    m_target = "Ground";

  // Input Valildation for isFlying 
  if( (isFlying == 0) || (isFlying == 1) ){
    m_isFlying = isFlying;
  }
  // Default for invalid input
  else
    m_isFlying = 0;
}

// GetName()
// Returns troop name
string Troop::GetName(){

  return m_name;
}

// SetName(string)
// Allows the user to change the name of the troop
void Troop::SetName(string name){

  m_name = name;
}

// GetHealth()
// Returns troop health
int Troop::GetHealth(){

  return m_health;
}

// SetHealth(int)
// Allows the user to change the health of the troop
void Troop::SetHealth(int health){

  if (health > 0){
    m_health = health;
  }else
    m_health = 0;
}

// GetDamage()
// Returns the damage of the troop
int Troop::GetDamage(){

  return m_damage;
}

// GetTarget()
// Returns troop target capabilities ("Both" can attack ground + air)
//                                   ("Ground" can ONLY attack ground)
string Troop::GetTarget(){

  string target = "";

  // Output validation for GetTarget
  if( (m_target == "Both") || (m_target == "both") ){
    target = "Both";
  }else
    target = "Ground";

  return target;
}

// GetIsFlying()
// Returns if troop is flying (1 is flying and only things with target both)
//                            (0 is not flying)
bool Troop::GetIsFlying(){

  return m_isFlying;
}

// AttackTroop(Troop&)
// Checks to see if 1. Attacking troop can attack "both" or "ground"
//                  2. If the target is flying or not.
//                  3. Updates health of target (current health - damage)
void Troop::AttackTroop(Troop& enemy){

  int userDamage = GetDamage();
  int enemyHealth = enemy.GetHealth();
  int remainingHealth = 0;

  cout << GetName() << " attacks " << enemy.GetName() << " on the ";

  // Checks if it is even possible to do damage to that enemy
  if( enemy.GetIsFlying() ){
    cout << "sky" << endl;

    // Can only damage flying unit if troop is able to attack one
    if (GetTarget() == "Both"){
      remainingHealth = enemyHealth - userDamage;
    }

    // Attack will miss if troop cannot attack a flying enemy
    else {
      remainingHealth = enemyHealth;
      userDamage = 0;
      cout << "Attack missed. Cannot damage " 
	   << "flying enemy with your ground-targeting unit." << endl;
    }
  }

  // Any troop can damage a grounded enemy
  else{
    cout << "ground" << endl;
    remainingHealth = enemyHealth - userDamage;
  }

  // After damage calculation health of the target is updated
  cout << userDamage << " Damage Done!" << endl; 
  enemy.SetHealth(remainingHealth);
  return;
}
