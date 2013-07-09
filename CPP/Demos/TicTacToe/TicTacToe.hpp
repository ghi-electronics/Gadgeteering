#ifndef _TICTACTOE_HPP_
#define _TICTACTOE_HPP_

#include "../../Gadgeteering/Interfaces.hpp"
#include "../../Gadgeteering/Mainboard.hpp"
#include "../../Gadgeteering/System.hpp"
#include "../../Gadgeteering/Types.hpp"

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