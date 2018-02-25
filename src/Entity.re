open MyManifest;

type entity =
  | Player;

type moveState =
  | Stand
  | Walk;

type entityDef = {
  sprStand: MySprites.sprite,
  sprWalk: MySprites.sprite,
  w: int,
  h: int,
};

type entityInst = {
  def: entityDef,
  move: moveState,
  spr: MySprites.spriteInst,
  x: int,
  y: int,
};

let player = {
  sprStand: MySprites.PlayerStand,
  sprWalk: MySprites.PlayerWalk,
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
    spr: MyManifest.make_sprite(edef.sprStand),
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

  let spr = MyManifest.make_sprite(s);

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

