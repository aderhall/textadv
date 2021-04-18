#include <stddef.h>

#ifndef TYPES_H
#define TYPES_H

void debug(int label);
enum Color {
  RED = 31,
  BLUE = 34,
  CYAN = 36,
  GREEN = 32,
  YELLOW = 33,
  NC = 0
};
void setc(enum Color c, int bold);

// Linked list node
typedef struct ll_item {
  void *value;
  struct ll_item *next;
} ll_item;
// Linked list
typedef struct ll {
  struct ll_item *first;
  struct ll_item *last;
} ll;
// Creates a new linked list and gives ownership to caller
ll ll_new();
// Drops the list, taking ownership of it. Second argument is a pointer to a function that will cleanup the list items. If NULL, free() will be used for cleanup
void ll_drop(void *list, void(*drop_value)(void*));
// Pushes value to the list, taking ownership of value and borrowing the list
void ll_push(ll *list, void *value);
void ll_pop_void(ll *list, void(*drop_value)(void*));
// Get the list item at index. Returns NULL if out of range
ll_item *ll_get(ll list, int index);

typedef struct string {
  char *s;
  size_t length;
  size_t max;
} string;
// Allocates a new string and gives ownership to caller
string string_new(char *str);
// Drops a string, taking ownership of it
void string_drop(void *str);
// Modifies the string, re-allocating memory if necessary.
void string_set(string *str, char *value);
// Modifies the string, shrinking it to fit the new value
void string_reset(string *str, char *value);
// Prints the string to stdout
void string_print(string str);
// Prints the string and a new line to stdout
void string_println(string str);

enum Direction {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3,
  UP = 4,
  DOWN = 5
};

enum Type {
  FIRE,
  WATER,
  EARTH,
  AIR,
  LIGHTNING,
  LIFE,
  METAL,
  MIND
};

enum ArmorRole {
  HELMET,
  BREASTPLATE,
  LEGGINGS,
  GLOVES,
  BOOTS,
  CHARM
};

typedef struct Inventory {
  struct ll weapons;
  struct ll armors;
  struct ll buffitems;
  struct ll keys;
  struct ll lockpicks;
} Inventory;
Inventory inventory_new();
void inventory_drop(void *inventory);
void inventory_display(Inventory);

typedef struct Player {
  int HP;
  int STR;
  int DEF;
  int INT;
  int MAG;
  int x;
  int y;
  struct Inventory backpack;
  struct Weapon *weapon;
  struct Armor *armor[6];
} Player;
// Allocates a new player
Player player_new();
void player_drop(void *player);

typedef struct Door {
  int locked;
  int open;
  string adjectives;
  string lockdesc;
  int lockqual;
  string key;
  int quality;
} Door;
Door door_new(int locked, int open, string adjectives, string lockdesc, int lockqual, string key, int quality);
void door_drop(void *door);

// A map location
typedef struct Setting {
  string name;
  string description;
  // Doors to settings north, east, south, west, above, and below this setting
  struct Door doors[6];
  
  // Inventory containing items located at this setting
  struct Inventory items;
  
  // Linked list of creatures at this setting
  struct ll creatures;
  
  // Whether the setting has already been visited
  int visited;
} Setting;
// Allocates a new setting and gives ownership to the caller
Setting setting_new(string name, string description);
// Drops a setting, taking ownership of it
void setting_drop(void *setting);
// Installs a setting at the supplied map coordinates, taking ownership of setting and borrowing map
void setting_install(ll *map, int x, int y, Setting);

// A vertical column in the map
typedef struct MapColumn {
  // The settings in the column
  ll cells;
} MapColumn;
// Allocates a new mapColumn, giving ownership to caller
MapColumn mapColumn_new();
// Drops a mapColumn, taking ownership of it
void mapColumn_drop(void *column);
// Gets the setting at coordinates (x, y). Returns NULL if out of range
Setting *map_get(ll map, int x, int y);
Setting *map_locate(ll map, Player player);
Setting *map_get_adjacent(ll map, Player player, enum Direction dir);

typedef struct Creature {
  string name;
  string description;
  int friendly;
  int HP;
  int damage;
  int defense;
} Creature;
Creature creature_new(string name, string description, int friendly, int HP, int damage, int defense);
void creature_drop(void *creature);

typedef struct Weapon {
  string name;
  string description;
  int damage;
  int weight;
  int magic;
  enum Type type;
} Weapon;
Weapon weapon_new(string name, string description, int damage, int weight, int magic, enum Type type);
void weapon_drop(void *weapon);

typedef struct Armor {
  string name;
  string description;
  int HP;
  int STR;
  int DEF;
  int INT;
  int MAG;
  enum ArmorRole role;
  enum Type type;
} Armor;
Armor armor_new(string name, string description, int HP, int STR, int DEF, int INT, int MAG, enum ArmorRole role, enum Type type);
void armor_drop(void *armor);

typedef struct Buffitem {
  string name;
  string description;
  int HP;
  int STR;
  int DEF;
  int INT;
  int MAG;
  // Number of turns left where this buff is in effect. If -1, buff is indefinite
  int remaining;
  
  enum Type type;
} Buffitem;
Buffitem buffitem_new(string name, string description, int HP, int STR, int DEF, int INT, int MAG, int remaining, enum Type type);
void buffitem_drop(void *buffitem);

typedef struct Key {
  string adjectives;
  string key;
} Key;
Key key_new(string adjectives, string key);
void key_drop(void *key);

typedef struct Lockpick {
  string adjectives;
  int quality;
} Lockpick;
Lockpick lockpick_new(string adjectives, int quality);
void lockpick_drop(void *lockpick);

#endif