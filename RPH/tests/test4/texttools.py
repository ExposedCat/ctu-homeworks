import sys
import re


def compute_word_frequencies(path: str):
    try:
        file = open(path, 'r')
    except OSError:
        print(f'Error: can\'t open file {path}')
        sys.exit()

    contents = file.read()
    contents = re.sub('\n', ' ', contents)
    words = contents.split(' ')
    frequencies = {}
    for word in words:
        if not len(word):
            continue
        if not word in frequencies:
            frequencies[word] = 1
        else:
            frequencies[word] += 1
    return frequencies