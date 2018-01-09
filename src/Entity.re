type entity =
  | Player;

type moveState =
  | Stand
  | Walk;

type entityDef = {
  sprStand: Manifest.sprite,
  sprWalk: Manifest.sprite,
  w: int,
  h: int,
};

type entityInst = {
  def: entityDef,
  move: moveState,
  spr: Manifest.spriteInst,
  x: int,
  y: int,
};

let player = {
  sprStand: PlayerStand,
  sprWalk: PlayerWalk,
  w: 24,
  h: 24,
};

let make_entity = (e: entity, sx, sy) => {
  let edef = switch (e) {
  | Player => player
  };
  {
    def: edef,
    move: Stand,
    spr: Manifest.make_sprite(edef.sprStand),
    x: sx,
    y: sy,
  }
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

let change_move = (ei: entityInst, m: moveState) => {
  let s = switch(m) {
  | Stand => ei.def.sprStand
  | Walk => ei.def.sprWalk
  };

  let spr = Manifest.make_sprite(s);

  {
    ...ei,
    move: m,
    spr: {
      ...spr,
      flip: ei.spr.flip
    }
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
}

