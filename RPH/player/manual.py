from __future__ import annotations
from typing import Tuple

C = False
D = True


class MyPlayer:
    '''Player asks user for input'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        pass

    def move(self) -> bool:
        move = input('Enter opponent\'s move [c/d]:')
        return move == 'd'

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        pass
