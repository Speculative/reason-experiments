open DOM;

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

  type spriteSheetDef = {
    /*
    url: string,
     */
    img: DOM.imageElement,
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

  type spriteInst = {
    def: spriteDef,
    frame: int,
    t: int,
    flip: bool
  };

/*
############################
# Sprite sheet definitions #
############################
 */
let playerSheet = {
  /*
  url: "assets/player.png",
  */
  img: DOM.image(),
  w: 240,
  h: 240,
};

let tileSheet = {
  /*
  url: "assets/tiles.png",
  */
  img: DOM.image(),
  w: 384,
  h: 192,
};

let get_sprite_sheet_def = (s: spriteSheet): spriteSheetDef => {
  switch s {
  | PlayerSheet => playerSheet
  | TileSheet => tileSheet
  };
};

let initialize = () => {
  DOM.setSrc(playerSheet.img, "assets/player.png");
  DOM.setSrc(tileSheet.img, "assets/tiles.png");
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

