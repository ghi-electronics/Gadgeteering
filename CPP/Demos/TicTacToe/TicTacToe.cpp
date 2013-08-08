/*
Copyright 2013 GHI Electronics LLC

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "TicTacToe.hpp"

TicTacToe::TicTacToe()
{
	this->diag1 = new char[3];
	this->diag1[0] = 0;
	this->diag1[1] = 4;
	this->diag1[2] = 8;
	
	this->diag2 = new char[3];
	this->diag2[0] = 2;
	this->diag2[1] = 4;
	this->diag2[2] = 6;

	this->Board = new BoardItem[9];

	this->Reset();

	GHI::System::RandomNumberSeed(1337);
}

void TicTacToe::Reset()
{
	for(int i = 0; i < 9; i++)
	{
		this->Board[i] = BoardItems::Empty;
	}
}

TicTacToe::PlayStatus TicTacToe::CheckWin()
{
	for(int y = 0; y < 3; y++)
    {
        bool allXes = true;
        bool allOes = true;

        for (int x = 0; x < 3; x++)
        {
            if (Board[y * 3 + x] == BoardItems::Empty)
            {
                allXes = false;
                allOes = false;
                break;
            }

            if (Board[y * 3 + x] != BoardItems::X)
            {
                allXes = false;
            }

            if (Board[y * 3 + x] != BoardItems::O)
            {
                allOes = false;
            }
        }

        if (allXes)
            return PlayStatuses::X_Win;

        if (allOes)
            return PlayStatuses::O_Win;
    }


    for (int x = 0; x < 3; x++)
    {
        bool allXes = true;
        bool allOes = true;

        for (int y = 0; y < 3; y++)
        {
            if (Board[y * 3 + x] == BoardItems::Empty)
            {
                allXes = false;
                allOes = false;
                break;
            }

            if (Board[y * 3 + x] != BoardItems::X)
            {
                allXes = false;
            }

            if (Board[y * 3 + x] != BoardItems::O)
            {
                allOes = false;
            }
        }

        if (allXes)
            return PlayStatuses::X_Win;

        if (allOes)
            return PlayStatuses::O_Win;
    }

    if (Board[0] == BoardItems::X && Board[4] == BoardItems::X && Board[8] == BoardItems::X)
        return PlayStatuses::X_Win;

    if (Board[0] == BoardItems::O && Board[4] == BoardItems::O && Board[8] == BoardItems::O)
        return PlayStatuses::O_Win;

    if (Board[2] == BoardItems::X && Board[4] == BoardItems::X && Board[6] == BoardItems::X)
        return PlayStatuses::X_Win;

    if (Board[2] == BoardItems::O && Board[4] == BoardItems::O && Board[6] == BoardItems::O)
        return PlayStatuses::O_Win;

            for (int i = 0; i < 9; i++)
                if (Board[i] == BoardItems::Empty)
                    return PlayStatuses::Running;

            return PlayStatuses::Tie;
}

bool TicTacToe::CAWin(TicTacToe::BoardItem v)
{
    int oCount;
    int emptyPos;

    for (int y = 0; y < 3; y++)
    {
        emptyPos = -1;
        oCount = 0;

        for (int x = 0; x < 3; x++)
        {
            if (Board[y * 3 + x] == BoardItems::Empty)
            {
                emptyPos = y * 3 + x;
            }

            else if (Board[y * 3 + x] == v)
            {
                oCount++;
            }
        }

        if (oCount == 2 && emptyPos != -1)
        {
            Board[emptyPos] = BoardItems::O;
            return true;
        }
    }

    for (int x = 0; x < 3; x++)
    {
        emptyPos = -1;
        oCount = 0;

        for (int y = 0; y < 3; y++)
        {
            if (Board[y * 3 + x] == BoardItems::Empty)
            {
                emptyPos = y * 3 + x;
            }

            else if (Board[y * 3 + x] == v)
            {
                oCount++;
            }
        }

        if (oCount == 2 && emptyPos != -1)
        {
            Board[emptyPos] = BoardItems::O;
            return true;
        }
    }

    oCount = 0;
    emptyPos = -1;
    for (int i = 0; i < 3; i++)
    {
        if (Board[diag1[i]] == BoardItems::Empty)
        {
            emptyPos = diag1[i];
        }
        else if (Board[diag1[i]] == v)
        {
            oCount++;
        }
    }
    if (oCount == 2 && emptyPos != -1)
    {
        Board[emptyPos] = BoardItems::O;
        return true;
    }

    oCount = 0;
    emptyPos = -1;
    for (int i = 0; i < 3; i++)
    {
        if (Board[diag2[i]] == BoardItems::Empty)
        {
            emptyPos = diag2[i];
        }
        else if (Board[diag2[i]] == v)
        {
            oCount++;
        }
    }
    if (oCount == 2 && emptyPos != -1)
    {
        Board[emptyPos] = BoardItems::O;
        return true;
    }

    return false;
}

bool TicTacToe::CompleteOWin()
{
    return CAWin(BoardItems::O);
}

bool TicTacToe::AvoidXWin()
{
    return CAWin(BoardItems::X);
}

void TicTacToe::Move()
{

    while (true)
    {
		int x = GHI::System::RandomNumber(0,9);

        if (Board[x] == BoardItems::Empty)
        {
            Board[x] = BoardItems::O;
            break;
        }
    }
}

TicTacToe::PlayStatus TicTacToe::Play()
{
    PlayStatus status = CheckWin();
    if (status != PlayStatuses::Running)
        return status;

    if(!CompleteOWin())
    {
        if(!AvoidXWin())
        {
            Move();
        }
    }

    return CheckWin();
}
///////////////////////
//Helper class methods
///////////////////////

TicTacToe::BoardItems::BoardItems()
{
}

TicTacToe::PlayStatuses::PlayStatuses()
{
}