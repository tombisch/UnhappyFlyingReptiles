#include <windows.h>
#include <gdiplus.h>
#include <string>
using namespace Gdiplus;
using namespace std;


#ifndef __SCOREBOARD_H__
#define __SCOREBOARD_H__


/**
\class		Scoreboard
\author		Tom Bisch
\date		April 5, 2016
\brief		Represents the scoreboard for the Unhappy Flying Reptiles game.
\details	Contains methods for drawing game stats to the window, updating the game stats on a regular interval,
			ending a round in the game, starting a new round in the game and adding points to the total score.
			This is a static class that belongs to all objects in the Unhapppy Flying Reptiles game.
*/
class Scoreboard
{

private:
	static int totalScore;				// player total score in the game
	static int roundScore;				// player score in the current round
	static int secondCounter;			// counts up to SECOND_LIMIT
	static int round;					// current round in the game
	static bool isRoundActive;			// indicates if reptile has been hit or not

public:
	static int SECOND_LIMIT;			// number of refresh cycles until roundScore is decremented

	static void Draw(Gdiplus::Graphics* g, int windowWidth, int windowHeight);
	static bool Update(int windowWidth, int windowHeight);
	static void EndRound(void);
	static void StartNextRound(void);
	static void AddPoints(int points);

};


#endif