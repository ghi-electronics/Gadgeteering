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

#ifndef _TICTACTOE_HPP_
#define _TICTACTOE_HPP_

#include "../../../../core/Interfaces.hpp"
#include "../../../../core/Mainboard.hpp"
#include "../../../../core/System.hpp"
#include "../../../../core/Types.hpp"

class TicTacToe
{
	public:
		//Enums
		typedef char BoardItem;
		typedef char PlayStatus;

		class BoardItems
		{
			public:
				static const BoardItem Empty = 0x00;
				static const BoardItem X = 0x01;
				static const BoardItem O = 0x02;

			protected:

				BoardItems();
		};

		class PlayStatuses
		{
			public:
				static const PlayStatus Running = 0x00;
				static const PlayStatus Tie = 0x01;
				static const PlayStatus X_Win = 0x02;
				static const PlayStatus O_Win = 0x03;

				PlayStatuses();

			protected:
		};

		TicTacToe();

	protected:
		PlayStatus CheckWin(); //Used to see if the game is over or still running
		PlayStatus Play();

		void Reset();
		void Move();

		bool CAWin(BoardItem v);
		bool CompleteOWin();
		bool AvoidXWin();

		BoardItem *Board;
		char *diag1;
		char *diag2;
};

#endif
