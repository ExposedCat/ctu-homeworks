from __future__ import annotations
from typing import List


class MyVector:
    def __init__(self, values: List[int]) -> MyVector:
        self.vector = values

    def get_vector(self) -> List[int]:
        return self.vector

    def __mul__(self, vector: MyVector) -> int:
        return sum(a1 * a2 for a1, a2 in zip(self.vector, vector.get_vector()))


if __name__ == "__main__":
    vec1 = MyVector([1, 2, 3])  # vektory mohou byt i jine dimenze nez 3!
    vec2 = MyVector([3, 4, 5])
    print(vec1.get_vector())  # priklad ziskani seznamu
    print(vec2.get_vector())  # priklad ziskani seznamu
    # vypocet skalarniho soucinu, pretizeny operator *, vola se __mul__
    dot_product = vec1*vec2
    print(dot_product)       # jen kontrolni vypis
