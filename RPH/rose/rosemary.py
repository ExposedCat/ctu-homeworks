BRIE = "Aged Brie"
DIAMOND = "Diamond"
TICKET = "Tickets"


class Item:
    def __init__(self, name: str, days_left: int, quality: int):
        self.name = name
        self.days_left = days_left
        self.quality = quality


def update(item: Item):
    # Update number of days left
    if item.name != DIAMOND:
        item.days_left -= 1
    # Update quality
    if item.name == BRIE:
        # print(f'{item.quality} → ', end='')
        item.quality += item.quality < 50
        # print(item.quality)
    elif item.name != DIAMOND and item.quality > 0:
        if item.name == TICKET:
            if item.days_left > 1:
                item.quality += 3
            elif item.days_left > 6:
                item.quality += 2
            elif item.days_left > 10:
                item.quality += 1
            else:
                item.quality = 0
        else:
            item.quality -= 1 if item.days_left >= 0 else 2
