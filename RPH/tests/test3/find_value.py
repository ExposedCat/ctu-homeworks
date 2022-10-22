from turtle import position


def value_count(data, value):
    count = 0
    for row in data:
        for element in row:
            count += element == value
    return count


def value_positions(data, value):
    coords = []
    for row in range(len(data)):
        for col in range(len(data[row])):
            if data[row][col] == value:
                coords.append((row, col))
    return coords