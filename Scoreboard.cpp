#include "Scoreboard.h"


// class constants
int Scoreboard::SECOND_LIMIT = 33;
int Scoreboard::totalScore = 0;
int Scoreboard::roundScore = 100;
int Scoreboard::secondCounter = 0;
int Scoreboard::round = 1;
bool Scoreboard::isRoundActive = true;


/**
\brief		Draws the scoreboard stats to the window using the passed Gdiplus::Graphics pointer.
\param[in]	g The pointer to the graphics object used to draw to the window.
\param[in]	windowWidth The width of the window to draw to.
\param[in]	windowHeight The height of the window to draw to.
*/
void Scoreboard::Draw(Gdiplus::Graphics* g, int windowWidth, int windowHeight)
{
	Gdiplus::Font font(L"Vrinda", 12); // font
	Gdiplus::SolidBrush brush(Gdiplus::Color::Black); // brush (color)
	
	// total score
	wstring tScoreString = L"total score : " + to_wstring(totalScore);
	const WCHAR* tScoreWCHAR = tScoreString.c_str();
	Gdiplus::PointF tScorePoint(0, 0);
	g->DrawString(tScoreWCHAR, tScoreString.length(), &font, tScorePoint, &brush);

	// round score
	wstring rScoreString = L"round score : " + to_wstring(roundScore);
	const WCHAR* rScoreWCHAR = rScoreString.c_str();
	Gdiplus::PointF rScorePoint(0, 15);
	g->DrawString(rScoreWCHAR, rScoreString.length(), &font, rScorePoint, &brush);

	// round number
	wstring roundNumberString = L"round : " + to_wstring(round);
	const WCHAR* roundNumberWCHAR = roundNumberString.c_str();
	Gdiplus::PointF roundPoint(Gdiplus::REAL(windowWidth / 2), 0);
	g->DrawString(roundNumberWCHAR, roundNumberString.length(), &font, roundPoint, &brush);
}


/**
\brief		Updates the scoreboard stats for the next drawing frame.
\param[in]	windowWidth The width of the window to draw to.
\param[in]	windowHeight The height of the window to draw to.
\return		A bool indicating if the reptile should be reset.
*/
bool Scoreboard::Update(int windowWidth, int windowHeight)
{
	bool resetReptile = false; // indicates if reptile needs to be reset because no points left in round

	// increment second counter 
	secondCounter++;

	// only decrement roundScore if round is active (reptile has not been hit)
	if (secondCounter >= SECOND_LIMIT && isRoundActive == true)
	{
		roundScore -= 10;
		secondCounter = 0;

		// if roundScore reaches zero, start a new round
		if (roundScore < 0)
		{
			roundScore = 0;
			// reset reptile and move to next round
			resetReptile = true;
			EndRound();
		}
	}

	return resetReptile;
}


/**
\brief		Starts the next round of the Unhappy Flying Reptiles game.
*/
void Scoreboard::StartNextRound(void)
{
	round++;
	roundScore = 100;
	secondCounter = 0;
	isRoundActive = true;
}


/**
\brief		Ends the current round of the Unhappy Flying Reptiles game
			by adding the round score to the total score.
*/
void Scoreboard::EndRound(void)
{
	if (isRoundActive == true)
	{
		isRoundActive = false;
		totalScore += roundScore;
	}	
}


/**
\brief		Adds bonus points to the total score of the Unhappy Flying Reptiles game.
\param[in]	points The amount of bonus points to add to the total score.
*/
void Scoreboard::AddPoints(int points)
{
	totalScore += points;
}



