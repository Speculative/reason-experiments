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
  /*
  url: string,
  */
  img: DOM.DOM.imageElement,
  w: int,
  h: int,
};

type spriteDef = {
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
  /*
  url: "assets/player.png",
  */
  img: DOM.DOM.image(),
  w: 240,
  h: 240,
};

let tileSheet = {
  sprites: [Background],
  /*
  url: "assets/tiles.png",
  */
  img: DOM.DOM.image(),
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
  sheet: PlayerSheet,
  x: 168,
  y: 96,
  w: 24,
  h: 24,
  frames: 1,
  tframe: 1,
};

let playerWalk = {
  sheet: PlayerSheet,
  x: 0,
  y: 0,
  w: 24,
  h: 24,
  frames: 10,
  tframe: 50,
};

let background = {
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

let get_sheet_sprites = (s: spriteSheet) => {
  switch s {
  | PlayerSheet => [PlayerStand, PlayerWalk]
  | TileSheet => [Background]
  };
};

let initialize = () => {
  DOM.DOM.setSrc(playerSheet.img, "assets/player.png");
  DOM.DOM.setSrc(tileSheet.img, "assets/tiles.png");
};
