from __future__ import annotations
from typing import Tuple

# Icons & Colors
EMPTY = -1
BLACK = 0
WHITE = 1

# Math
INF = 99999
MAX_WEIGHT = 30

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
    """https://i.imgur.com/WdW78nF.png"""

    def __init__(self, my_color: int, opponent_color: int) -> MyPlayer:
        self.name = "prokoart"
        self.color = my_color
        self.vs_color = opponent_color
        self.board_size = None

        self.max_depth = 2

        # Move priorities by
        # https://web.fe.up.pt/~eol/IA/MIA0203/trabalhos/Damas_Othelo/Docs/Eval.html#othello
        self.move_weights = [
            [100, -30, 6, 2, 2, 6, -30, 100],
            [-30, -50, 0, 0, 0, 0, -50, -30],
            [6, 0, 0, 0, 0, 0, 0, 6],
            [2, 0, 0, 3, 3, 0, 0, 2],
            [2, 0, 0, 3, 3, 0, 0, 2],
            [6, 0, 0, 0, 0, 0, 0, 6],
            [-30, -50, 0, 0, 0, 0, -50, -30],
            [100, -30, 6, 2, 2, 6, -30, 100],
        ]

        # Timed game - different strategies for different rounds played
        self.rounds = 0

    def move(self, board: list[list[int]]) -> Tuple[int, int] | None:
        if self.board_size is None:
            self.board_size = len(board)
            # Because we love corners. Explained later
            self.corner_positions = (
                (0, 0),  # Top left
                (self.board_size - 1, 0),  # Top right
                (0, self.board_size - 1),  # Bottom left
                (self.board_size - 1, self.board_size - 1),  # Bottom right
            )
        self.max_depth = self.calc_max_depth()
        alpha = -INF
        beta = INF
        return self.alpha_beta(board, alpha, beta)

    # PLAYER METHODS
    # Average rounds per game is around 28
    def calc_max_depth(self):
        self.rounds += 1
        if self.rounds >= 20:
            return 3
        return self.max_depth

    def alpha_beta(
        self, board: list[list[int]], alpha: int, beta: int
    ) -> Tuple[int, int] | None:
        possible_moves = self.get_possible_moves(
            self.color, self.vs_color, board
        )
        best_move = None
        for [row, col] in possible_moves:
            temp_board = self.clone_board(board)
            temp_board = self.reverse(
                temp_board, row, col, self.color, self.vs_color
            )
            temp_alpha = self.minimax(temp_board, 1, alpha, beta)
            if temp_alpha > alpha:
                alpha = temp_alpha
                best_move = (row, col)
        # Inspired by random guy idea:
        # When we take a dot at the corner,
        # it would be nice to place other dots around that,
        # so we can increate weights of that dots
        for position in self.corner_positions:
            if position == best_move:
                for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
                    if self.is_valid(
                        row + x_dir, col + y_dir, self.board_size
                    ):
                        self.move_weights[row + x_dir][
                            col + y_dir
                        ] += MAX_WEIGHT
        return best_move

    def minimax(
        self, board: list[list[int]], depth: int, alpha: int, beta: int
    ):
        if depth == self.max_depth:
            return self.evaluate_board_stats(board)
        else:
            self_move = depth % 2 == 0  # Even checks are mine
            color = self.color if self_move else self.vs_color
            vs_color = self.vs_color if self_move else self.color

            possible_moves = self.get_possible_moves(color, vs_color, board)
            if not len(possible_moves):
                return self.minimax(board, depth + 1, alpha, beta)
            for [row, col] in possible_moves:
                temp_board = self.clone_board(board)
                temp_board = self.reverse(
                    temp_board, row, col, color, vs_color
                )
                temp_alpha_beta = self.minimax(
                    temp_board, depth + 1, alpha, beta
                )

                if self_move:  # Maximizing
                    if temp_alpha_beta > alpha:
                        alpha = temp_alpha_beta
                    if alpha >= beta:
                        return alpha
                else:  # Minimizing
                    if temp_alpha_beta < beta:
                        beta = temp_alpha_beta
                    if beta <= alpha:
                        return beta

            return alpha if self_move else beta

    def evaluate_board_stats(self, board: list[list[int]]) -> int:
        self_moves = self.get_possible_moves(self.color, self.vs_color, board)
        vs_moves = self.get_possible_moves(self.vs_color, self.color, board)

        stats = [
            0,  # self_score
            0,  # self_stability
            len(self_moves),  # self_mobility
            0,  # self_corners
            0,  # vs_score
            0,  # vs_stability
            len(vs_moves),  # vs_mobility
            0,  # vs_corners
        ]

        for row in range(self.board_size):
            for col in range(self.board_size):
                if board[row][col] == EMPTY:
                    continue
                offset = 4 if board[row][col] == self.vs_color else 0
                stats[offset + 1] += 1
                stats[offset + 0] += self.move_weights[row][col]
                for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
                    if (
                        self.is_valid(
                            row + x_dir, col + y_dir, self.board_size
                        )
                        and board[row + x_dir][col + y_dir] == EMPTY
                    ):
                        stats[offset + 3] += 1
                        break
        return self.evaluate_stats(*stats)

    # https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
    # + Idea of timed game
    def evaluate_stats(
        self,
        # Own stats
        self_score: int,
        self_stability: int,
        self_mobility: int,
        self_corners: int,
        # VS opponent stats
        vs_score: int,
        vs_stability: int,
        vs_mobility: int,
        vs_corners: int,
    ) -> int:
        # Not gonna divide by zero
        if vs_mobility == 0:
            vs_mobility = 1
        if vs_stability == 0:
            vs_stability = 1
        if self_corners == 0:
            self_corners = 1

        score = self_score - vs_score
        mobility = self_mobility / vs_mobility
        stability = self_stability / vs_stability
        corners = vs_corners / self_corners

        if self.rounds >= 36:
            # Priority: Stability & Score
            return stability + score
        elif self.rounds >= 16:
            # Priority: Mobility & Corners & Score
            return mobility * 5 + corners * 10 + score
        else:
            # Priority: Mobility & Score
            return mobility + score

    # GAME METHODS
    def is_valid(self, row: int, col: int, size: int) -> bool:
        return size > row >= 0 and size > col >= 0

    def reverse(
        self,
        board: list[list[int]],
        x: int,
        y: int,
        color: int,
        opponent: int,
    ):
        board[x][y] = color
        for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
            new_x = x + x_dir
            new_y = y + y_dir
            if (
                self.is_valid(new_x, new_y, self.board_size)
                and board[new_x][new_y] == opponent
            ):
                # Look for end
                while (
                    self.is_valid(new_x, new_y, self.board_size)
                    and board[new_x][new_y] == EMPTY
                ):
                    if board[new_x][new_y] == color:
                        new_x += -x_dir
                        new_y += -y_dir
                        # Repeat until reach start
                        while (
                            self.is_valid(new_x, new_y, self.board_size)
                            and board[new_x][new_y] != color
                        ):
                            board[new_x][new_y] = color
                            new_x += -x_dir
                            new_y += -y_dir
                        break
                    #  Restore coordinates
                    new_x += x_dir
                    new_y += y_dir
        return board

    def clone_board(self, board: list[list[int]]):
        return [
            [board[row][col] for col in range(len(board))]
            for row in range(self.board_size)
        ]

    def get_possible_moves(
        self, me: int, opponent: int, board: list[list[int]]
    ) -> list[tuple[int, int]]:
        moves = []
        for row in range(self.board_size):
            for col in range(self.board_size):
                if board[row][col] == EMPTY:
                    for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
                        x = row + x_dir
                        y = col + y_dir
                        end = False
                        if (
                            self.is_valid(x, y, self.board_size)
                            and board[x][y] == opponent
                        ):
                            while (
                                self.is_valid(x, y, self.board_size)
                                and board[x][y] != EMPTY
                            ):
                                if board[x][y] == me:
                                    moves.append((row, col))
                                    end = True
                                    break
                                x += x_dir
                                y += y_dir
                        if end:
                            break
        return moves
