#include "types.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>

#define TRUE 1
#define FALSE 0

void debug(int label) {
  printf("DEBUG: %d\n", label);
}
void setc(enum Color c, int bold) {
  if (c == NC) {
    printf("\033[0m");
  } else {
    printf("\033[%d;%dm", bold, c);
  }
}

ll ll_new() {
  ll result = {NULL};
  return result;
}
void ll_drop(void *list, void(*drop_value)(void*)) {
  // Takes ownership of list
  ll_item *current = ((ll*)list)->first;
  ll_item *next;
  if (current == NULL) {
    return;
  }
  if (drop_value == NULL) {
    while (current != NULL) {
      next = current->next;
      free(current->value);
      free(current);
      current = next;
    }
    return;
  }
  while (current != NULL) {
    next = current->next;
    (*drop_value)(current->value);
    free(current);
    current = next;
  }
}
void ll_push(ll *list, void *value) {
  ll_item *item = (ll_item*)malloc(sizeof(ll_item));
  item->value = value;
  
  if (list->first == NULL) {
    list->first = item;
    list->last = item;
    return;
  }
  list->last->next = item;
  list->last = item;
}
void ll_pop_void(ll *list, void(*drop_value)(void*)) {
  // Takes ownership of last item
  ll_item *current = list->first;
  if (current == NULL) {
    return;
  }
  if (current->next == NULL) {
    if (drop_value == NULL) {
      free(current->value);
    } else {
      (*drop_value)(current->value);
    }
    free(current);
    list->first = NULL;
    return;
  }
  while (current->next->next != NULL) {
    current = current->next;
  }
  if (drop_value == NULL) {
    free(current->next->value);
  } else {
    (*drop_value)(current->next->value);
  }
  free(current->next);
  list->last = current;
}
ll_item *ll_get(ll list, int index) {
  // Does not take ownership of list or the item found
  ll_item *current = list.first;
  if (index < 0) {
    return NULL;
  }
  while (index > 0) {
    if (current == NULL) {
      return NULL;
    }
    current = current->next;
    --index;
  }
  return current;
}

string string_new(char *str) {
  // Borrows str
  size_t length = strlen(str);
  // Ownership of s is given to caller
  char *s = malloc(length * sizeof(char));
  for (int i = 0; i < length; ++i) {
    s[i] = *str;
    ++str;
  }
  string result = {.s = s, .length = length, .max = length};
  return result;
}
void string_drop(void *str) {
  free(((string*)str)->s);
}
void string_set(string *str, char *value) {
  size_t length = strlen(value);
  if (length > str->max) {
    free(str->s);
    str->s = malloc(length * sizeof(char));
    str->max = length;
  }
  str->length = length;
  for (int i = 0; i < length; ++i) {
    str->s[i] = *value;
    ++value;
  }
}
void string_reset(string *str, char *value) {
  string_drop(str);
  *str = string_new(value);
}
void string_print(string str) {
  for (int i = 0; i < str.length; ++i) {
    printf("%c", str.s[i]);
  }
}
void string_println(string str) {
  string_print(str);
  printf("\n");
}

Inventory inventory_new() {
  Inventory result = {
    .weapons = ll_new(),
    .armors = ll_new(),
    .buffitems = ll_new(),
    .keys = ll_new(),
    .lockpicks = ll_new()
  };
  return result;
}
void inventory_drop(void *inventory) {
  ll_drop(&((Inventory*)inventory)->weapons, &weapon_drop);
  ll_drop(&((Inventory*)inventory)->armors, &armor_drop);
  ll_drop(&((Inventory*)inventory)->buffitems, &buffitem_drop);
  ll_drop(&((Inventory*)inventory)->keys, &key_drop);
  ll_drop(&((Inventory*)inventory)->lockpicks, &lockpick_drop);
}
void inventory_display(Inventory inventory) {
  ll_item *current = inventory.weapons.first;
  if (current != NULL) {
    setc(CYAN, 0);
    printf("Weapons:\n");
    setc(NC, 0);
    while (current != NULL) {
      printf("\t");
      setc(CYAN, 1);
      string_print(((Weapon*)current->value)->name);
      setc(NC, 0);
      printf(": ");
      string_println(((Weapon*)current->value)->description);
      current = current->next;
    }
  }
  current = inventory.armors.first;
  if (current != NULL) {
    setc(CYAN, 0);
    printf("Armor:\n");
    setc(NC, 0);
    while (current != NULL) {
      printf("\t");
      setc(CYAN, 1);
      string_print(((Armor*)current->value)->name);
      setc(NC, 0);
      printf(": ");
      string_println(((Armor*)current->value)->description);
      current = current->next;
    }
  }
  current = inventory.buffitems.first;
  if (current != NULL) {
    setc(CYAN, 0);
    printf("Items:\n");
    setc(NC, 0);
    while (current != NULL) {
      printf("\t");
      setc(CYAN, 1);
      string_print(((Buffitem*)current->value)->name);
      setc(NC, 0);
      printf(": ");
      string_println(((Buffitem*)current->value)->description);
      current = current->next;
    }
  }
  current = inventory.keys.first;
  if (current != NULL) {
    setc(CYAN, 0);
    printf("Keys:\n");
    setc(NC, 0);
    while (current != NULL) {
      printf("\tA ");
      setc(BLUE, 0);
      string_print(((Key*)current->value)->adjectives);
      setc(NC, 0);
      printf(" key\n");
      current = current->next;
    }
  }
  current = inventory.lockpicks.first;
  if (current != NULL) {
    setc(CYAN, 0);
    printf("Lockpicks:\n");
    setc(NC, 0);
    while (current != NULL) {
      printf("\tA ");
      setc(BLUE, 0);
      string_print(((Lockpick*)current->value)->adjectives);
      setc(NC, 0);
      printf(" lockpick\n");
      current = current->next;
    }
  }
}

Player player_new() {
  Player result = {
    .HP = 100,
    .STR = 10,
    .DEF = 10,
    .INT = 10,
    .MAG = 10,
    .backpack = inventory_new(),
    // Will eventually borrow from backpack.weapons
    .weapon = NULL,
    // Empty array, will eventually contain pointers that borrow from backpack.armors
    .armor = {},
    .x = 0,
    .y = 0
  };
  return result;
};
void player_drop(void *player) {
  inventory_drop(&((Player*)player)->backpack);
}

Setting setting_new(string name, string description) {
  // `result` takes ownership of name and desc, which is handed back to caller
  Setting result = {
    .name = name,
    .description = description,
    .items = inventory_new(),
    .creatures = ll_new(),
    .doors = {0},
    .visited = 0
  };
  return result;

  /*Setting *result = (Setting*)malloc(sizeof(Setting));
  result->name = name;
  result->description = desc;
  result->items = inventory_new();
  result->creatures = ll_new();
  return result;*/
}
void setting_drop(void *setting) {
  string_drop(&((Setting*)setting)->name);
  string_drop(&((Setting*)setting)->description);
  inventory_drop(&((Setting*)setting)->items);
  ll_drop(&((Setting*)setting)->creatures, &creature_drop);
}
void setting_install(ll *map, int x, int y, Setting setting) {
  ll_item *currentColumn = map->first;
  int i = 0;
  while (1) {
    if (currentColumn == NULL) {
      while (1) {
        // Push a new column and initialize it
        ll_push(map, malloc(sizeof(MapColumn)));
        *(MapColumn*)map->last->value = mapColumn_new();
        if (i == x) {
          break;
        }
        ++i;
      }
      currentColumn = map->last;
      break;
    }
    if (i < x) {
      currentColumn = currentColumn->next;
    } else {
      break;
    }
    ++i;
  }
  ll_item *currentCell = ((MapColumn*)currentColumn->value)->cells.first;
  i = 0;
  while (1) {
    if (currentCell == NULL) {
      while (1) {
        // Push a new Setting and initialize it to zero
        ll_push(&((MapColumn*)currentColumn->value)->cells, calloc(1, sizeof(Setting)));
        if (i == y) {
          break;
        }
        ++i;
      }
      currentCell = ((MapColumn*)currentColumn->value)->cells.last;
      break;
    }
    if (i < y) {
      currentCell = currentCell->next;
    } else {
      break;
    }
    ++i;
  }
  *(Setting*)currentCell->value = setting;
}
MapColumn mapColumn_new() {
  MapColumn result = {ll_new()};
  return result;
}
void mapColumn_drop(void *column) {
  ll_drop(&((MapColumn*)column)->cells, &setting_drop);
}
Setting *map_get(ll map, int x, int y) {
  ll_item *columnNode = ll_get(map, x);
  if (columnNode == NULL) {
    return NULL;
  }
  ll_item *cellNode = ll_get(((MapColumn*)columnNode->value)->cells, y);
  if (cellNode == NULL) {
    return NULL;
  }
  return (Setting*)cellNode->value;
}
Setting *map_locate(ll map, Player player) {
  return map_get(map, player.x, player.y);
}
Setting *map_get_adjacent(ll map, Player player, enum Direction dir) {
  if (dir == NORTH) {
    return map_get(map, player.x, player.y - 1);
  }
  if (dir == EAST) {
    return map_get(map, player.x + 1, player.y);
  }
  if (dir == SOUTH) {
    return map_get(map, player.x, player.y + 1);
  }
  if (dir == WEST) {
    return map_get(map, player.x - 1, player.y);
  }
  return NULL;
}

Door door_new(int locked, int open, string adjectives, string lockdesc, int lockqual, string key, int quality) {
  // Takes ownership of adjectives, lockdesc, and key
  Door result = {locked, open, adjectives, lockdesc, lockqual, key, quality};
  return result;
}
void door_drop(void *door) {
  string_drop(&((Door*)door)->adjectives);
  string_drop(&((Door*)door)->lockdesc);
  string_drop(&((Door*)door)->key);
}

Creature creature_new(string name, string description, int friendly, int HP, int damage, int defense) {
  Creature result = {name, description, friendly, HP, damage, defense};
  return result;
}
void creature_drop(void *creature) {
  string_drop(&((Creature*)creature)->name);
  string_drop(&((Creature*)creature)->description);
}

Weapon weapon_new(string name, string description, int damage, int weight, int magic, enum Type type) {
  Weapon result = {name, description, damage, weight, magic, type};
  return result;
}
void weapon_drop(void *weapon) {
  string_drop(&((Weapon*)weapon)->name);
  string_drop(&((Weapon*)weapon)->description);
}

Armor armor_new(string name, string description, int HP, int STR, int DEF, int INT, int MAG, enum ArmorRole role, enum Type type) {
  Armor result = {name, description, HP, STR, DEF, INT, MAG, role, type};
  return result;
}
void armor_drop(void *armor) {
  string_drop(&((Armor*)armor)->name);
  string_drop(&((Armor*)armor)->description);
}

Buffitem buffitem_new(string name, string description, int HP, int STR, int DEF, int INT, int MAG, int remaining, enum Type type) {
  Buffitem result = {name, description, HP, STR, DEF, INT, MAG, remaining, type};
  return result;
}
void buffitem_drop(void *buffitem) {
  string_drop(&((Buffitem*)buffitem)->name);
  string_drop(&((Buffitem*)buffitem)->description);
}

Key key_new(string adjectives, string key) {
  Key result = {adjectives, key};
  return result;
}
void key_drop(void *key) {
  string_drop(&((Key*)key)->adjectives);
  string_drop(&((Key*)key)->key);
}

Lockpick lockpick_new(string adjectives, int quality) {
  Lockpick result = {adjectives, quality};
  return result;
}
void lockpick_drop(void *lockpick) {
  string_drop(&((Lockpick*)lockpick)->adjectives);
}
