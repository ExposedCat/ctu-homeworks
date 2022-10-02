from __future__ import annotations
from typing import Tuple

COOPERATE = False
DEFECT = True


class MyPlayer:
    '''
    Plays with last opponent's move unless being betrayed twice
    '''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        self.payoff_matrix = payoff_matrix
        self.number_of_iterations = number_of_iterations or None
        self.opponent_last_move = None

        self.betrayed = 0
        self.betrayed_limit = 2

    def _basic_strategy(self) -> bool:
        if self.opponent_last_move is None:
            return COOPERATE
        if self.opponent_last_move == DEFECT:
            self.betrayed += 1
        if self.betrayed == self.betrayed_limit:
            self.betrayed = 0
            return DEFECT
        else:
            return self.opponent_last_move

    def move(self) -> bool:
        return self._basic_strategy()

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        self.opponent_last_move = opponent_last_move
