from os import listdir
from file_reader import read_text_file


class Corpus:
    def __init__(self, path: str):
        self.path = path

    def emails(self):
        for filename in listdir(self.path):
            if filename[0] == '!':
                continue
            # TODO: Use read_email_file
            email = read_text_file(f'{self.path}/{filename}')
            # TODO: Use email['body']
            yield (filename, email)
