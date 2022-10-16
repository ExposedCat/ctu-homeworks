from __future__ import annotations
from typing import Tuple

from random import randint

C = False
D = True


class MyPlayer:
    '''This player detects opponent's strategy and exploits it'''

    def __init__(
        self,
        payoff_matrix: Tuple[Tuple[Tuple[int]]],
        number_of_iterations: int | None = None,
    ) -> MyPlayer:
        self.CC = payoff_matrix[C][C]
        self.CD = payoff_matrix[C][D]
        self.DD = payoff_matrix[D][D]
        self.DC = payoff_matrix[D][C]

        self.my_moves = []
        self.opponent_last_move = None

        self.iteration = 1
        self.state_alternating = False
        self.analyzed = False
        self.finish_detection = 5
        if number_of_iterations is not None and number_of_iterations >= 20:
            self.finish_detection = 10
        if number_of_iterations is not None and number_of_iterations >= 30:
            self.finish_detection = 15
        self.opponent_is_tft = True
        self.opponent_is_random = False
        self.opponent_is_constant = None
        self.opponents_constant = None

        self.preferred = self.DD[0] > self.CC[0]
        self.matrix_is_classic = False
        self.matrix_is_obvious = False
        self.matrix_is_alternating = False
        self._analyze_payoff_matrix()

    def _analyze_payoff_matrix(self) -> None:
        obvious_move = self._get_best_response()
        if obvious_move is not None:
            # Matrix is Obvious
            self.matrix_is_obvious = True
            self.preferred = obvious_move
        elif 2 * self.CC[0] < self.CD[0] + self.DC[0]:
            # Matrix is Alternating
            self.matrix_is_alternating = True
        else:
            # Matrix is Classic or Inverted
            self.matrix_is_classic = True

    def _get_best_response(self) -> bool | None:
        CC, CD, DD, DC = (self.CC, self.CD, self.DD, self.DC)
        if DD[0] > CC[0] and DD[0] > DC[0] and DD[0] > CD[0]:
            return D
        if CC[0] > DD[0] and CC[0] > DC[0] and CC[0] > CD[0]:
            return C
        return None

    def _analyze_opponent(self) -> None:
        if self.iteration > 2 and self.opponent_last_move != self.my_moves[-2]:
            # Opponent is TFT
            self.opponent_is_tft = False
        if self.iteration == self.finish_detection:
            self.analyzed = True
            if not self.opponent_is_tft and not self.opponent_is_constant:
                # Opponent is random (or unknown)
                self.opponent_is_random = True

    def strategy_analyzing(self) -> bool:
        return self.iteration != 2

    def strategy_smart_tft(self, initial: bool = C) -> bool:
        if not len(self.my_moves):
            return initial
        if self.opponent_last_move == self.preferred:
            return self.my_moves[-1]
        else:
            return not self.my_moves[-1]

    def move(self) -> bool:
        self._analyze_opponent()
        if not self.analyzed:
            move = self.strategy_analyzing()
        else:
            if self.matrix_is_obvious:
                if (
                    self.opponent_is_constant
                    and self.opponents_constant != self.preferred
                ):
                    move = D
                elif self.opponent_is_tft:
                    move = self.preferred
                else:
                    move = self.strategy_smart_tft()
            elif self.matrix_is_alternating:
                if self.opponent_is_constant:
                    if self.opponents_constant == D:
                        move = D
                    else:
                        move = not self.my_moves[-1]
                else:
                    if not self.state_alternating and randint(1, 5) == 1:
                        self.state_alternating = True
                        move = not self.my_moves[-1]
                    else:
                        move = self.strategy_smart_tft()
            elif self.opponent_is_constant:
                move = D
            elif self.opponent_is_tft:
                move = self.preferred
            elif self.opponent_is_random:
                move = self.strategy_smart_tft()

        self.iteration += 1
        return move

    def record_last_moves(
        self, my_last_move: bool, opponent_last_move: bool
    ) -> None:
        self.my_moves.append(my_last_move)

        if self.opponent_is_constant is None:
            # Opponent may be constant
            self.opponent_is_constant = True
            self.opponents_constant = opponent_last_move
        elif self.opponent_last_move != opponent_last_move:
            # Opponent is not constant
            self.opponent_is_constant = False
            self.opponents_constant = None

        self.opponent_last_move = opponent_last_move
