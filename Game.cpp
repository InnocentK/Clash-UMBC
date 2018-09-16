/**************************************
 ** File:    Game.cpp
 ** Project: CMSC 202 Project 2 - Clash UMBC, Fall 2017
 ** Author:  Innocent Kironji
 ** Date:    10/19/17
 ** Section: 0107
 ** E-mail:  wambugu1@umbc.edu
 **
 ** This file contains the implimentation for all member functions of the
 ** game class and handles starting and operating  the game.
 ***************************************/

#include "Game.h"
#include "Troop.h"
#include <ctime>

const int DEAD = 0;
const int LOSE = DEAD;

/* Game() Default Constructor
  // Builds a new game by:
  // 1. Asking user for their name,
  // 2. Asking number of troops on team,
  // 3. Loading all troops from file (function call)
  // 4. Randomizes both teams (function call)
  // 5. Starts game (function call)
  */
Game::Game(){

  // Getting basic info from user
  cout << "What is your name? ";
  getline(cin, m_name);
  cout << "Nice to meet you " << m_name << endl;

  // Determining how many troops need to be generated for both teams
  do{
    cout << "How many troops would you like? (2-5) ";
    cin >> m_max;
  }while( (m_max < 2) || (m_max > 5) );

  // Loads troops
  LoadTroops();
  cout << "Please wait for teams to load." << endl;
  
  // Populates all teams with random troops
  for(int i = 0; i < m_max; i++){
    srand( time(NULL) + rand() );
    RandomizeTeam();
  }
  cout << "Both teams have been populated." << endl;

  StartGame();

}

// LoadTroops
// Loads each troop into m_troop from file
void Game::LoadTroops(){
  
  // Initilializing variables needed to save troop vector
  ifstream troopStream( PROJ2_TROOPS.c_str() );
  string troopName = "";
  int troopHealth = 0;
  int troopDamage = 0;
  string troopTarget = "";
  bool troopIsFlying = 0;

  // Verification if file is properly loaded 
  if( troopStream.is_open() ){
    while (troopStream >> troopName){
    
      // Holds the elements of each new Troop
      troopStream >> troopHealth >> troopDamage >> troopTarget 
		  >> troopIsFlying;
      Troop newTroop (troopName, troopHealth, troopDamage, troopTarget
		      , troopIsFlying);

      // Troop is added to the troop vector (vector is of unknown size)
      m_troop.push_back(newTroop);
    }
  }else
    cout << "Sir! File was unable to open, sir!" << endl;

  troopStream.close();
  return;
}

// RandomizeTeam
// Randomly chooses a troop and puts it on a team
//  (For both computer and player teams)
void Game::RandomizeTeam(){

  // Setting the values for the randomisation range
  const int MAX = (int)m_troop.size() - 1;
  const int MIN = 0;

  //Setting valid random numbers (one for player & computer each)
  srand( rand() );
  int randNum = rand() % MAX + MIN;
  srand(randNum);
  int randNum2 = rand() % MAX + MIN;

  // Adding a new troop to the player's team
  Troop playerTroop( m_troop.at(randNum).GetName(),
		     m_troop.at(randNum).GetHealth(),
		     m_troop.at(randNum).GetDamage(),
		     m_troop.at(randNum).GetTarget(),
		     m_troop.at(randNum).GetIsFlying() );
  m_player.push_back(playerTroop);

  // Adding a new troop to the enemy's team
  Troop computerTroop( m_troop.at(randNum2).GetName(),
		       m_troop.at(randNum2).GetHealth(),
		       m_troop.at(randNum2).GetDamage(),
		       m_troop.at(randNum2).GetTarget(),
		       m_troop.at(randNum2).GetIsFlying() );
  m_computer.push_back(computerTroop);

  return;
}

// StartGame()
// Manages the game itself including win conditions continually calling 
//  the main menu
void Game::StartGame(){

  int playerTeamSize = (int)m_player.size();
  int computerTeamSize = (int)m_computer.size();

  // Refers to element number in vector for player troop
  m_curTroop = 0;

  // Either 1 (for player) or 2 (for computer)
  m_curPlayer = 1;

  DisplayTeams();
  
  // Runs the game until one time has been eliminated
  while( ( playerTeamSize > LOSE) &&
	 ( computerTeamSize > LOSE) ){
    MainMenu();
    
    // Increments and checks for valid current player
    m_curPlayer ++;
    if (m_curPlayer > 2)
      m_curPlayer = 1;
    

    // Continually updates team sizes
    playerTeamSize = (int)m_player.size();
    computerTeamSize = (int)m_computer.size();
  }

  // Outputs victory and defeat prompts
  if( (playerTeamSize == LOSE) && (computerTeamSize == LOSE) ){
    cout << " \n Rats! Its a Draw. I wanted to see a bloodbath. \n" << endl;
  }else if (computerTeamSize == LOSE){
    cout << endl << "You win! \n" << endl;
  }else if (playerTeamSize == LOSE)
    cout << endl << "Sorry, you lose! \n" << endl;

  return;
}

// DisplayTroops()
// Test Function that displays all troops (to check if loaded properly)
void Game::DisplayTroops(){

  cout << endl << "-------------------------------------------" << endl;

  // Prints all info on all troops
  for(vector<Troop>::iterator it = m_troop.begin(); it != m_troop.end(); ++it){

    cout << it->GetName() << " " << it->GetHealth() << " " << it->GetDamage()
	 << " " << it->GetTarget() << " " << it->GetIsFlying() << endl;
  }
  cout << "-------------------------------------------" << endl;
  return;
}

// ChangeTroop()
// Allows the user to change current troop
void Game::ChangeTroop(){

  const int TEAM_SIZE = (int)m_player.size();
  int troopChoice = 0;
  DisplayTeam();

  // Validates user's choice for next usuable troop
  do{
    cout << "What troop do you want to use? " << endl;
    cin >> troopChoice;

  }while( (troopChoice < 1) || (troopChoice > TEAM_SIZE));

  // Sets current troop to position of chosen troop in player vector
  m_curTroop = troopChoice - 1;

  return;
}

// MainMenu()
// Displays and manages menu
void Game::MainMenu(){

  int choice = 0;
  bool playerDraw = false;
  bool compDraw = false;
  
  // Prompts user for a choice on next action
  do{
    cout << "*******************" << endl;
    cout << "What would you like to do?" << endl 
	 << "1. Display Team" << endl
	 << "2. Change Current Troop" << endl
	 << "3. Attack!" << endl
	 << "4. Quit" << endl;
    cin >> choice;
  }while( (choice < 1) || (choice > 4) );

  // Shows what occurs for each of the player's possible choices
  switch (choice)
    {
    case 1:
      DisplayTeam();
      break;

      // Checks if this is a valid option to make (i.e. no units to pick from)
    case 2:
      if( (int)m_player.size() > 1){
	ChangeTroop();
      }else
	cout << "Foolish Mortal! Tis not a choice with only one unit" << endl;
      DisplayTeams();
      break;

      // Allows comp/player to attack; switches control to next attacker after
    case 3:
      // Checks for draw
      playerDraw = ( (m_player.at(m_curTroop).GetTarget() == "Ground") 
			  && (m_computer.at(0).GetIsFlying() ));
      compDraw = (( m_computer.at(0).GetTarget() == "Ground") 
		       && (m_player.at(m_curTroop).GetIsFlying() ));

      // When draw occurs game ends
      if (playerDraw && compDraw){
	cout << "You have just entered an infiinte loop for attacking." 
	     << "\n Appropriate measures will now be taken" << endl;

	// Displays to user what the repetition would look like before quitting
	cout << "If you don't believe me I'll show you just once: \n" << endl;
	cout << "-------------------------------------------" << endl;
	Attack();
	Attack();
	cout << "-------------------------------------------" << endl;
	cout << endl << "Now if you're satisfied I can do my job." << endl;

	// Conditions for a draw is that both teams are empty
	m_player.clear();
	m_computer.clear();

      }else {
	//Business as usual when there is no draw

	// If Player starts turn this is how attack sequence goes
	if (m_curPlayer == 1){
	  Attack();
	  
	  // Computer can only retaliate if they didn't die
	  if (m_curPlayer == 2)
	    Attack();

	}else{
	  // Computer Starts turn. This is how attack sequence goes
	  Attack();
	
	  // Player can only retaliate if they didn't die
	  if (m_curPlayer == 1)
	    Attack();
	}
      }

      DisplayTeams();
      break;

      // If the user chooses to quit
    default:
      m_player.clear();
      cout << "It was fun playing with you, even if you are a dirty quitter."
	   << endl;
      DisplayTroops();
    }
  return;
}

// DisplayTeams()
// Displays the current teams (both player and computer)
void Game::DisplayTeams(){

  // Prints out info on player team 
  cout << endl << "*******************" << endl;
  cout << m_name << "'s Team" << endl;
  for(vector<Troop>::iterator it = m_player.begin(); it != m_player.end();
      ++it)
  {    
    cout << it->GetName() << " " << it->GetHealth() << endl;
  }

  // Prints out info on computer team
  cout << "*******************" << endl;
  cout << "Computer's Team" << endl;
  for(vector<Troop>::iterator it = m_computer.begin(); it != m_computer.end();
      ++it)
  {
    cout << it->GetName() << " " << it->GetHealth() << endl;
  }
  return;
}

// DisplayTeam()
// Displays just player team (for changing current troop)
void Game::DisplayTeam(){
 
  int troopNum = 0;

  // Prints out info on player team                                       
  cout << endl << "*******************" << endl;
  cout << m_name << "'s Team" << endl;
  for(vector<Troop>::iterator it = m_player.begin(); it != m_player.end();
      ++it)
    {   
      cout << ++troopNum << ". " << it->GetName() << " " << it->GetHealth() 
	   << endl;
    }
  return;
}

// Attack()
// Manages the troops attacking each other
// If a troop has less than or equal to 0 health, removes "dead" troop 
//  from vector
void Game::Attack(){

  //Checks who is currently attacking
  switch (m_curPlayer)
    {

      // Player is attacking
    case 1:
      m_player[m_curTroop].AttackTroop( m_computer.at(0) );
      m_curPlayer = 2;

      // Checks and updates if computer died in battle
      if(m_computer.at(0).GetHealth() == DEAD){
	cout << m_computer.at(0).GetName() << " dies!" << endl;
	m_computer.erase( m_computer.begin() );
	m_curPlayer = 1;
      }
      break;

      // Computer is attacking 
    case 2:
      m_computer.at(0).AttackTroop(m_player.at(m_curTroop));
      m_curPlayer = 1;

      // Checks and updates if player died in battle
      if(m_player.at(m_curTroop).GetHealth() == DEAD){
	cout << m_player.at(m_curTroop).GetName() << " dies!" << endl;
	m_player.erase(m_player.begin() + m_curTroop);

	// Allows player to pick next unit when one dies but only if there are
	//  enough troops to pick from (only a choice with 2 or more troops)
	if( (int)m_player.size() > 1){
	  ChangeTroop();
	}else
	  m_curTroop = 0;
      }
      break;

      // Invalid attacker
    default:
      cout << "There is no Player " << m_curPlayer << "." << endl;
    }
  return;
}
