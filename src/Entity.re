open Sprites;

type entity =
  | Player;

type moveState =
  | Stand
  | Walk;


type entityDef = {
  getSprite: (moveState) => Sprites.sprite,
  w: int,
  h: int,
};

type entityInst = {
  def: entityDef,
  move: moveState,
  spr: spriteInst,
  x: int,
  y: int,
};

let tick_sprite = (ei: entityInst, ticks: float) => {
  {
    ...ei,
    spr: {
      ...ei.spr,
      t: ei.spr.t + int_of_float(ticks),
      frame: ((ei.spr.t + int_of_float(ticks)) / ei.spr.def.tframe) mod ei.spr.def.frames
    }
  }
};

let move = (ei: entityInst, x': int, y': int) => {
  {
    ...ei,
    x: x',
    y: y'
  }
};

let flip = (ei: entityInst, f: bool) => {
  {
    ...ei,
    spr: {
      ...ei.spr,
      flip: f
    }
  }
};

let player = {
  getSprite: (m: moveState) =>
    switch (m) {
    | Stand => PlayerStand
    | Walk => PlayerWalk
    },
  w: 24,
  h: 24,
};

let get_entity_def = (e: entity) =>
  switch (e) {
  | Player => player
  };

let make_entity = (e: entity, sx, sy) => {
  let edef = get_entity_def(e);
  {
    def: edef,
    move: Stand,
    spr: Manifest.make_sprite(edef.getSprite(Stand)),
    x: sx,
    y: sy,
  }
};

let change_move = (ei: entityInst, m: moveState) => {
  let spr = Manifest.make_sprite(ei.def.getSprite(m));

  {
    ...ei,
    move: m,
    spr: {
      ...spr,
      flip: ei.spr.flip
    }
  }
};

