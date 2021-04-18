Map:
A bunch of settings pointing to up to 6 other settings (north, south, east, west, up, down)

Setting:
Name, description, pointers to neighboring settings, linked list of items, linked list of creatures, doors: array of 6 doors (or NULLs), one for each neighboring setting.

Door:
Locked, open, adjectives, lock description, key: string, door quality, lock quality

Creature:
Name, friendly description, angry description, friendly: bool, HP, damage, defense

Player:
Has HP, STR, DEF, INT, MAG. Pointer to backpack. Pointer to weapon. Pointer to armor. Pointer to setting.

Inventory:
A bunch of items in a linked list

## Items:
Weapon:
Damage, weight, magic, name, type: Enum(fire, water, earth, air, lightning, life, metal, mind)

Key:
description, key: string

Lockpick:
description, quality

BuffItem:
Name, description, HP, STR, DEF, INT, MAG, turns remaining (int or NULL for infinite)

Armor:
Name, description, HP, STR, DEF, INT, MAG, role: Enum(helmet, breastplate, leggings, gloves, boots, charm), type: Enum(fire, ...)