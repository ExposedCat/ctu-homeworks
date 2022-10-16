from __future__ import annotations
from typing import Tuple

C = False
D = True


class MyPlayer:
    '''This player always defects'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        pass

    def _basic_strategy(self) -> bool:
        return D

    def move(self) -> bool:
        return self._basic_strategy()

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        pass
