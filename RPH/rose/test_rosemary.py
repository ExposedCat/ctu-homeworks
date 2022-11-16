from typing import Iterable
from rosemary import Item, update

# Special names
BRIE = "Aged Brie"
DIAMOND = "Diamond"
TICKET = "Tickets"


# Helpers
def ensure_static_name(name: str) -> bool:
    item = Item(name, 1, 1)
    update(item)
    return item.name == name


def validate_quality(
    name: str, days_left: int, quality: int, expected: int
) -> bool:
    item = Item(name, days_left, quality)
    update(item)
    return item.quality == expected


def validate_days_left(
    name: str, days_left: int, quality: int, expected: int
) -> bool:
    item = Item(name, days_left, quality)
    update(item)
    return item.days_left == expected


# Tests
# Název zboží nebo předmětu se nikdy nemění.
def test_static_name_basic():
    return ensure_static_name('Basic')


def test_static_name_brie():
    return ensure_static_name(BRIE)


def test_static_name_diamond():
    return ensure_static_name(DIAMOND)


def test_static_name_ticket():
    return ensure_static_name(TICKET)


#  Kvalita zboží nikdy není menší než 0 a větší než 50
# s jedinou výjimkou (Diamanty).
def test_basic_quality():
    return (
        validate_quality('Basic', 1, 0, 0)
        and validate_quality('Basic', 0, 1, 0)
        and validate_quality('Basic', -1, 1, 0)
        and validate_quality('Basic', 1, 50, 49)
        and validate_quality('Basic', 0, 49, 47)
        and validate_quality('Basic', -1, 49, 47)
    )


def test_brie_quality_0():
    return (
        validate_quality(BRIE, 0, 0, 1)
        and validate_quality(BRIE, 0, 1, 2)
        and validate_quality(BRIE, 0, 50, 50)
    )


def test_brie_quality_1():
    return (
        validate_quality(BRIE, 1, 0, 1)
        and validate_quality(BRIE, 1, 1, 2)
        and validate_quality(BRIE, 1, 50, 50)
    )


def test_brie_quality_m1():
    return (
        validate_quality(BRIE, -1, 0, 1)
        and validate_quality(BRIE, -1, 1, 2)
        and validate_quality(BRIE, -1, 50, 50)
    )


def test_diamond_quality():
    return (
        validate_quality(DIAMOND, 0, 100, 100)
        and validate_quality(DIAMOND, 1, 100, 100)
        and validate_quality(DIAMOND, -1, 100, 100)
    )


def test_ticket_quality_end():  
    return (
        validate_quality(TICKET, 0, 0, 0)
        and validate_quality(TICKET, -1, 1, 0)
    )


def test_ticket_quality_1_5():
    return (
        validate_quality(TICKET, 1, 0, 3)
        and validate_quality(TICKET, 2, 1, 4)
        and validate_quality(TICKET, 3, 1, 4)
        and validate_quality(TICKET, 4, 1, 4)
        and validate_quality(TICKET, 5, 1, 4)
    )


def test_ticket_quality_6_10():
    return (
        validate_quality(TICKET, 6, 0, 2)
        and validate_quality(TICKET, 7, 1, 3)
        and validate_quality(TICKET, 8, 1, 3)
        and validate_quality(TICKET, 9, 1, 3)
        and validate_quality(TICKET, 10, 1, 3)
    )


def test_ticket_quality_more():
    return (
        validate_quality(TICKET, 11, 0, 1)
        and validate_quality(TICKET, 12, 1, 2)
        and validate_quality(TICKET, 12, 50, 50)
    )


# Ve hře se na konci každého dne zavolá pro každé zboží na skladě funkce
# update(). Ta aktualizuje (obvykle sníží) hodnoty days_left
def test_brie_days_0():
    return (
        validate_days_left(BRIE, 0, 0, -1)
        and validate_days_left(BRIE, 0, 1, -1)
        and validate_days_left(BRIE, 0, 50, -1)
    )


def test_brie_days_1():
    return (
        validate_days_left(BRIE, 1, 0, 0)
        and validate_days_left(BRIE, 1, 1, 0)
        and validate_days_left(BRIE, 1, 50, 0)
    )


def test_brie_days_m1():
    return (
        validate_days_left(BRIE, -1, 0, -2)
        and validate_days_left(BRIE, -1, 1, -2)
        and validate_days_left(BRIE, -1, 50, -2)
    )


def test_diamond_days_left():
    return (
        validate_days_left(DIAMOND, 0, 100, 0)
        and validate_days_left(DIAMOND, 1, 100, 1)
        and validate_days_left(DIAMOND, -1, 100, -1)
    )
