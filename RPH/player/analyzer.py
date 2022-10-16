from __future__ import annotations
from typing import Tuple

C = False
D = True


class MyPlayer:
    '''This player always plays mutually beneficial'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        self.analyze_payoff_matrix(payoff_matrix)

    def analyze_payoff_matrix(self, payoff_matrix):
        # If DD is mutually beneficial over CC, swap moves
        self.preferred = payoff_matrix[D][D][0] > payoff_matrix[C][C][0]

    def _basic_strategy(self) -> bool:
        return self.preferred

    def move(self) -> bool:
        return self._basic_strategy()

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        pass
