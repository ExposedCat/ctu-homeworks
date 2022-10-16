from __future__ import annotations
from typing import Tuple

C = False
D = True


class MyPlayer:
    '''This player always plays opponent's last move'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        self.moves = 0
        self.opponent_last_move = None

    def _basic_strategy(self) -> bool:
        if self.moves < 4:
            return D
        if self.moves == 4:
            return C
        return self.opponent_last_move

    def move(self) -> bool:
        return self._basic_strategy()

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        self.moves += 1
        self.opponent_last_move = opponent_last_move
