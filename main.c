#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define TRUE 1
#define FALSE 0
#define I 256

#define s(CHARP) string_new(CHARP)
#define print(STR) string_print(STR)
#define println(STR) string_println(STR)
#define c(STR) strncmp(input, STR, I) == 0

#define i(x, y, NAME, DESC) setting_install(&map, x, y, setting_new(s(NAME), s(DESC)))

ll game_map_setup() {
  ll map = ll_new();
  setting_install(&map, 0, 0, setting_new(
    s("Lobby"),
    s("A quiet, comfortable waiting place.")
  ));
  map_get(map, 0, 0)->visited = 1;
  setting_install(&map, 1, 0, setting_new(
    s("Hallway 1"),
    s("A well-lit, homely passage with carpeted floors.")
  ));
  setting_install(&map, 5, 5, setting_new(
    s("Secret room"),
    s("Don't tell anyone.")
  ));
  i(2, 0, "Hallway 2", "Another carpeted hallway. The lights are golden-yellow and the air is pleasantly warm.");
  i(2, 1, "Hallway 3", "Another hallway. Not a soul in sight.");
  i(2, 2, "Fresco room", "A small but not cramped room. The roof is covered in a painted fresco depicting a man and a woman, each holding elegant swords.");
  i(2, 3, "Candle room", "This room is stone and lit only by candlelight. A stone pedestal stands at one end.");
  
  return map;
}
Player game_player_setup(ll map) {
  Player player = player_new();
  
  // Starting location
  player.x = 0;
  player.y = 0;
  
  // Starting items
  Weapon *w = (Weapon*)malloc(sizeof(Weapon));
  *w = weapon_new(s("Sword"), s("A big ol' sword"), 10, 5, 0, METAL);
  ll_push(&player.backpack.weapons, w);
  
  w = (Weapon*)malloc(sizeof(Weapon));
  *w = weapon_new(s("Knife"), s("A small knife"), 6, 1, 0, METAL);
  ll_push(&player.backpack.weapons, w);
  
  Armor *a = (Armor*)malloc(sizeof(Armor));
  *a = armor_new(s("Hat"), s("A stiff leather cap. Could protect your head from impacts, but not by much"), 0, 0, 1, 0, 0, HELMET, LIFE);
  ll_push(&player.backpack.armors, a);
  
  Key *k = (Key*)malloc(sizeof(Key));
  *k = key_new(s("rusty"), s("r"));
  ll_push(&player.backpack.keys, k);
  
  Buffitem *b = (Buffitem*)malloc(sizeof(Buffitem));
  *b = buffitem_new(s("Bread"), s("A dry breadroll – tasteless, but it's food"), 10, 0, 0, 0, 0, -1, LIFE);
  ll_push(&player.backpack.buffitems, b);
  
  return player;
}

void cmd_inventory(Player player) {
  printf("\n");
  //printf("You are carrying:\n");
  inventory_display(player.backpack);
  printf("\n");
}
void cmd_location(ll map, Player player) {
  printf("\n");
  setc(YELLOW, 1);
  println(map_locate(map, player)->name);
  setc(NC, 0);
  println(map_locate(map, player)->description);
  Setting *adj = map_get_adjacent(map, player, NORTH);
  if (adj != NULL) {
    printf("To the north, ");
    setc(YELLOW, 1);
    println(adj->name);
    setc(NC, 0);
  }
  adj = map_get_adjacent(map, player, EAST);
  if (adj != NULL) {
    printf("To the east, ");
    setc(YELLOW, 1);
    println(adj->name);
    setc(NC, 0);
  }
  adj = map_get_adjacent(map, player, SOUTH);
  if (adj != NULL) {
    printf("To the south, ");
    setc(YELLOW, 1);
    println(adj->name);
    setc(NC, 0);
  }
  adj = map_get_adjacent(map, player, WEST);
  if (adj != NULL) {
    printf("To the west, ");
    setc(YELLOW, 1);
    println(adj->name);
    setc(NC, 0);
  }
  printf("\n");
}
void cmd_go(ll map, Player *player, enum Direction dir) {
  Setting *adj = map_get_adjacent(map, *player, dir);
  if (adj == NULL) {
    printf("You can't go that way\n");
    return;
  }
  if (dir == NORTH) {
    --player->y;
  } else if (dir == EAST) {
    ++player->x;
  } else if (dir == SOUTH) {
    ++player->y;
  } else if (dir == WEST) {
    --player->x;
  }
  if (!adj->visited) {
    cmd_location(map, *player);
  }
  adj->visited = 1;
}

void prompt(char *s, char *p) {
  printf("%s", p);
  fgets(s, I, stdin);
  // Remove trailing newline
  s[strcspn(s, "\n\r")] = '\0';
}
void process_input(char input[I], ll *map, Player *player) {
  if (c("inventory")) {
    cmd_inventory(*player);
  } else if (c("look") || c("l")) {
    cmd_location(*map, *player);
  } else if (c("north") || c("n")) {
    cmd_go(*map, player, NORTH);
  } else if (c("east") || c("e")) {
    cmd_go(*map, player, EAST);
  } else if (c("south") || c("s")) {
    cmd_go(*map, player, SOUTH);
  } else if (c("west") || c("w")) {
    cmd_go(*map, player, WEST);
  } else {
    printf("Unrecognized command\n");
  }
}

int main() {
  // Map
  ll map = game_map_setup();
  // Main player
  Player player = game_player_setup(map);
  
  char input[I];
  while (1) {
    setc(YELLOW, 1);
    print(map_locate(map, player)->name);
    setc(NC, 0);
    prompt(input, " > ");
    process_input(input, &map, &player);
  }
  
  // Clean up allocated resources
  ll_drop(&map, &mapColumn_drop);
  player_drop(&player);
  return 0;
}