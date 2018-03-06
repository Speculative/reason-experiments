/*
###########################
# Concrete resource names #
###########################
 */
type spriteSheet =
| PlayerSheet
| TileSheet;

type sprite =
| PlayerStand
| PlayerWalk
| Background;

/*
########################
# Resource definitions #
########################
 */
type spriteSheetDef = {
  sprites: list(sprite),
  url: string,
  w: int,
  h: int,
};

type spriteDef = {
  sprite: sprite,
  sheet: spriteSheet,
  x: int,
  y: int,
  w: int,
  h: int,
  frames: int,
  tframe: int,
};

/*
#####################
# Runtime resources #
#####################
 */
type spriteInst = {
  def: spriteDef,
  frame: int,
  t: int,
  flip: bool,
};

/*
############################
# Sprite sheet definitions #
############################
 */
let playerSheet = {
  sprites: [PlayerStand, PlayerWalk],
  url: "assets/player.png",
  w: 240,
  h: 240,
};

let tileSheet = {
  sprites: [Background],
  url: "assets/tiles.png",
  w: 384,
  h: 192,
};

let get_sprite_sheet_def = (s: spriteSheet): spriteSheetDef => {
  switch s {
  | PlayerSheet => playerSheet
  | TileSheet => tileSheet
  };
};

/*
######################
# Sprite definitions #
######################
 */
let playerStand = {
  sprite: PlayerStand,
  sheet: PlayerSheet,
  x: 168,
  y: 96,
  w: 24,
  h: 24,
  frames: 1,
  tframe: 1,
};

let playerWalk = {
  sprite: PlayerWalk,
  sheet: PlayerSheet,
  x: 0,
  y: 0,
  w: 24,
  h: 24,
  frames: 10,
  tframe: 50,
};

let background = {
  sprite: Background,
  sheet: TileSheet,
  x: 0,
  y: 168,
  w: 24,
  h: 24,
  frames: 1,
  tframe: 1,
};

let get_sprite_def = (s: sprite) => {
  switch s {
  | PlayerStand => playerStand
  | PlayerWalk => playerWalk
  | Background => background
  };
};

let get_sprite_id = (s: sprite) => {
  switch s {
  | PlayerStand => 0
  | PlayerWalk => 1
  | Background => 2
  };
};

let numSheets = 2;
let spriteSheets = [PlayerSheet, TileSheet];

