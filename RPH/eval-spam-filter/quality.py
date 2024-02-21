from confmat import BinaryConfusionMatrix
from utils import read_classification_from_file


def quality_score(tp: int, tn: int, fp: int, fn: int):
    return (tp + tn) / (tp + tn + 10 * fp + fn)


def compute_quality_for_corpus(corpus_dir: str):
    truth = read_classification_from_file(f'{corpus_dir}/!truth.txt')
    prediction = read_classification_from_file(f'{corpus_dir}/!prediction.txt')
    confmat = BinaryConfusionMatrix(pos_tag='OK', neg_tag='SPAM')
    confmat.compute_from_dicts(truth, prediction)
    return quality_score(**confmat.as_dict())
