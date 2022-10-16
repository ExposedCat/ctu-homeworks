from __future__ import annotations
from typing import Tuple

C = False
D = True


class MyPlayer:
    '''Pavlov strategy'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        self.CC = payoff_matrix[C][C]
        self.CD = payoff_matrix[C][D]
        self.DD = payoff_matrix[D][D]
        self.DC = payoff_matrix[D][C]

        self.my_last_move = None
        self.opponent_last_move = []

        self.switch_each = round(number_of_iterations * 0.25)

        self._analyze_payoff_matrix()

    def _analyze_payoff_matrix(self):
        self.preferred = self.DD[0] > self.CC[0]
        self.best_response = self._get_best_response()

    def _get_best_response(self) -> bool | None:
        CC = self.CC
        CD = self.CD
        DD = self.DD
        DC = self.DC
        if DD[0] > CC[0] and DD[0] > DC[0] and DD[0] > CD[0]:
            return D
        if CC[0] > DD[0] and CC[0] > DC[0] and CC[0] > CD[0]:
            return C
        return None

    def strategy_nash_equilibria(self) -> bool:
        return self.best_response

    def strategy_smart_tft(self) -> bool:
        if self.my_last_move is None:
            return self.preferred
        if self.opponent_last_move == self.my_last_move:
            return self.my_last_move
        else:
            return not self.my_last_move

    def move(self) -> bool:
        if self.best_response is not None:
            return self.strategy_nash_equilibria()
        return self.strategy_smart_tft()

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        self.my_last_move = my_last_move
        self.opponent_last_move = opponent_last_move
