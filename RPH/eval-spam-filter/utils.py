from file_reader import read_text_file


def read_classification_from_file(path: str):
    classification = read_text_file(path)
    spam = {}
    for line in classification.split('\n'):
        if not line:
            continue
        email, result = line.split()
        spam[email] = result
    return spam


def write_classification_to_file(classification: dict, path: str):
    file = open(path, 'w+')
    for (email, result) in classification.items():
        file.write(f"{email} {result}\n")
