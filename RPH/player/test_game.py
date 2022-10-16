# This script creates two (identical) players,
# lets them play for some number of iterations and
# then displays their scores.
#
# For running this script, you need to put the 'game.py'
# as well as your 'player.py' to the python path, e.g.
# to the the working directory.
#
# A very simple testing script, feel free to modify it
# according to your needs
#
# example code for students of B4B33RPH course
# Author: Tomas Svoboda, and the RPH team

from game import Game

# assuming your player is in player.py as required
# https://cw.fel.cvut.cz/wiki/courses/b4b33rph/cviceni/veznovo_dilema/specifikace
from analyzer import MyPlayer as Analyzer
from manual import MyPlayer as Manual
from cooperator import MyPlayer as Cooperator
from guilty_tit_for_tat import MyPlayer as GuiltyTFT
from tit_for_tat import MyPlayer as TFT
from defector import MyPlayer as Defector
from player import MyPlayer as Adapter
from pavlov import MyPlayer as Pavlov

# define the payoff matrix; see game.py for detailed explanation of this matrix
payoff_matrix = (
    ((2, 2), (1, 70)),
    ((70, 1), (4, 4)),
)

# define the number of iterations
number_of_iterations = 25

for Player, name in (
    (Analyzer, 'Analyzer'),
    (Cooperator, 'Cooperator'),
    (GuiltyTFT, 'GuiltyTFT'),
    (TFT, 'TFT'),
    (Defector, 'Defector'),
    (Adapter, 'Adapter'),
    (Pavlov, 'Pavlov'),
):
    # create the players
    playerA = Player(payoff_matrix, number_of_iterations)
    # at the moment, me and the opponent are of the same type
    playerB = Adapter(payoff_matrix, number_of_iterations)

    # create the game instance
    my_game = Game(playerA, playerB, payoff_matrix, number_of_iterations)
    # run game
    my_game.run()

    # get scores
    scores = my_game.get_players_payoffs()

    # display scores
    print(f'{name} got:', scores[0], '\nAdaptor got:', scores[1])
