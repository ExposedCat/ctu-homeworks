from __future__ import annotations
from typing import Tuple

# Icons & Colors
EMPTY = -1
BLACK = 0
WHITE = 1

# Directions
L = (-1, 0)
L_U = (-1, -1)
L_D = (-1, 1)
U = (0, -1)
D = (0, 1)
R = (1, 0)
R_U = (1, -1)
R_D = (1, 1)


class MyPlayer:
    """Dummy player with no strategy"""

    def __init__(self, my_color: int, opponent_color: int) -> MyPlayer:
        self.name = "prokoart"

        self.color = my_color
        self.opponent_color = opponent_color

    def is_valid(self, row: int, col: int) -> bool:
        return 8 > row >= 0 and 8 > col >= 0

    def get_possible_moves(
        self, me: int, opponent: int, board: list[list[int]]
    ) -> list[tuple[int, int]]:
        moves = []
        for row in range(8):
            for col in range(8):
                if board[row][col] == EMPTY:
                    for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
                        x = row + x_dir
                        y = col + y_dir
                        end = False
                        if self.is_valid(x, y) and board[x][y] == opponent:
                            while self.is_valid(x, y) and board[x][y] != EMPTY:
                                if board[x][y] == me:
                                    moves.append((row, col))
                                    end = True
                                    break
                                x += x_dir
                                y += y_dir
                        if end:
                            break
        return moves

    def move(self, board: list[list[int]]) -> Tuple[int, int] | None:
        possible_moves = self.get_possible_moves(
            self.color, self.opponent_color, board
        )
        if not len(possible_moves):
            return None
        return possible_moves[0]
