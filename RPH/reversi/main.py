from game import Game
from player import MyPlayer

game = Game()
player = MyPlayer(1, 0)
while True:
    game.move(1, player.move(game.board))
    print(game)
    # Manual player
    x, y = list(input('> '))
    game.move(0, (int(x), int(y)))
    print(game)
