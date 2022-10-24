from typing import Literal


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


class Game:
    def __init__(self) -> None:
        self.board: list[list[int]] = [
            [-1, -1, -1, -1, -1, -1, -1, -1],
            [-1, -1, -1, -1, -1, -1, -1, -1],
            [-1, -1, -1, -1, -1, -1, -1, -1],
            [-1, -1, -1, 1, 0, -1, -1, -1],
            [-1, -1, -1, 0, 1, -1, -1, -1],
            [-1, -1, -1, -1, -1, -1, -1, -1],
            [-1, -1, -1, -1, -1, -1, -1, -1],
            [-1, -1, -1, -1, -1, -1, -1, -1],
        ]

    def is_valid(self, x: int, y: int) -> bool:
        return 8 > x >= 0 and 8 > y >= 0

    def get_occupied_cells(
        self, player: int, x: int, y: int
    ) -> list[tuple[int, int]] | Literal[None]:
        row = x
        col = y
        if self.board[row][col] == EMPTY:
            for x_dir, y_dir in (L_U, L, U, L_D, R_U, R, D, R_D):
                x = row + x_dir
                y = col + y_dir
                cells = []
                if self.is_valid(x, y) and self.board[x][y] == (not player):
                    possible = False
                    while self.is_valid(x, y) and self.board[x][y] != EMPTY:
                        cells.append((x, y))
                        if self.board[x][y] == player:
                            possible = True
                            break
                        x += x_dir
                        y += y_dir
                    if possible:
                        return cells
                    cells = []
        return None

    def move(self, player: int, position: tuple[int, int]):
        x, y = position
        cells = self.get_occupied_cells(player, x, y)
        if not self.is_valid(x, y) or cells is None:
            raise Exception(f'Invalid move of player {player} to {x} {y}')
        self.board[x][y] = player
        print(f'Fill {player} move {x};{y}')
        print(f'Fill {player} cells', cells)
        for x, y in cells:
            self.board[x][y] = player

    def __str__(self) -> str:
        result = ''
        print(' ', end='')
        for y in range(8):
            print(y, end='')
        print()
        for x in range(8):
            print(x, end='')
            for y in range(8):
                print(
                    'X'
                    if self.board[x][y] == 1
                    else '#'
                    if self.board[x][y]
                    else 'O',
                    end='',
                )
            print()
        return result
