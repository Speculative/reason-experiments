open DOM;

/* Sprite sheet definitions */
type spriteSheet =
  | PlayerSheet
  | TileSheet;

type spriteSheetInst = {
  img: DOM.imageElement,
  w: int,
  h: int,
};

let playerSheet = {
  img: DOM.image(),
  w: 240,
  h: 240,
};

let tileSheet = {
  img: DOM.image(),
  w: 384,
  h: 192,
};

let get_sprite_sheet = (s: spriteSheet): spriteSheetInst => {
  switch s {
  | PlayerSheet => playerSheet
  | TileSheet => tileSheet
  };
};

let sprite_url = (s: spriteSheet) => {
  switch s {
  | PlayerSheet => "assets/player.png"
  | TileSheet => "assets/tiles.png"
  };
};

let initialize = () => {
  DOM.setSrc(playerSheet.img, sprite_url(PlayerSheet));
  DOM.setSrc(tileSheet.img, sprite_url(TileSheet));
};


/* Sprite definitions */
type sprite =
  | PlayerStand
  | PlayerWalk
  | Background;

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

let get_sprite = (s: sprite) => {
  switch s {
  | PlayerStand => playerStand
  | PlayerWalk => playerWalk
  | Background => background
  };
};

let get_frame_offset = (s: spriteInst) => {
  let shdef = get_sprite_sheet(s.def.sheet);
  (s.def.x + (((s.frame * s.def.w) mod (shdef.w / s.def.w)) * s.def.w),
   s.def.y + ((s.frame * s.def.w) / shdef.w))
};

let make_sprite = (s: sprite) => {
  {
    def: get_sprite(s),
    frame: 0,
    t: 0,
    flip: false
  }
};

