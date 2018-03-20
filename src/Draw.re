open DOM;

type layer =
  | Player
  | Background
  | Debug;

let layer_context = (l: layer, s: State.state) => {
  switch l {
  | Player => s.l.player
  | Background => s.l.background
  | Debug => s.l.debug
  };
};

let clear = (s: State.state, l: layer) => {
  Canvas.clearRect(
    layer_context(l, s),
    0, 0,
    Config.width, Config.height);
};

let blank_canvas = () => {
  DOM.createElement("canvas");
};

let get_sprite_canvas = (s: State.state, si: Sprites.spriteInst, ctx: Canvas.context) => {
  let optionCacheEntry = Immutable.IntMap.get(Sprites.get_sprite_id(si.def.sprite), s.manifest.spriteCache);
  switch optionCacheEntry {
  | Some(cacheEntry) => {
    let dirSpriteList = if (si.flip) {
      cacheEntry.sf
    } else {
      cacheEntry.s
    };
    List.nth(dirSpriteList, si.frame)
    }
  | None => blank_canvas()
  };
};

let draw_sprite = (s: State.state, l: layer, si: Sprites.spriteInst, x: int, y: int) => {
  let ctx = layer_context(l, s);
  let spriteCanvas = get_sprite_canvas(s, si, ctx);
  Canvas.drawCanvas(
    ctx,
    spriteCanvas,
    0, 0,
    si.def.w, si.def.h,
    x, y,
    si.def.w, si.def.h);
  ()
};

let draw_bounding = (s: State.state, bbox: Physics.boundingBox, color: string) => {
  let ctx = layer_context(Player, s);
  let (w, n, e, s) = Physics.intBoundingBox(bbox);
  Canvas.strokeStyle(ctx, color);
  Canvas.strokeRect(ctx, w, n, e - w, s - n);
};

let draw_poly = (ctx: Canvas.context, vertices: list(Physics.pvec)) => {
  Canvas.beginPath(ctx);
  let (lx, ly) = List.hd(List.rev(vertices));
  Canvas.moveTo(ctx, int_of_float(lx), int_of_float(ly));
  List.iter(
    ((vx, vy)) => {
      Canvas.lineTo(ctx, int_of_float(vx), int_of_float(vy));
    },
    vertices);
  Canvas.stroke(ctx);
};

let draw_collision_test = (s: State.state) => {
  let ctx = layer_context(Debug, s);
  Canvas.strokeStyle(ctx, "#FF0000");
  let dt = 20.;
  let sweeps = [
    Physics.getAreaSwept((30., 30., 40., 40.), (-1., -1.), dt),
    Physics.getAreaSwept((50., 30., 60., 40.), ( 0., -1.), dt),
    Physics.getAreaSwept((70., 30., 80., 40.), ( 1., -1.), dt),
    Physics.getAreaSwept((30., 50., 40., 60.), (-1.,  0.), dt),
    Physics.getAreaSwept((50., 50., 60., 60.), ( 0.,  0.), dt),
    Physics.getAreaSwept((70., 50., 80., 60.), ( 1.,  0.), dt),
    Physics.getAreaSwept((30., 70., 40., 80.), (-1.,  1.), dt),
    Physics.getAreaSwept((50., 70., 60., 80.), ( 0.,  1.), dt),
    Physics.getAreaSwept((70., 70., 80., 80.), ( 1.,  1.), dt),
  ];
  List.iter(draw_poly(ctx), sweeps);

  let player_bound = Physics.getBoundingBox(s.player.x, s.player.y, s.player.spr.def.w, s.player.spr.def.h);
  List.iter(
    (sweep) => {
      if (Physics.satCollision(Physics.boundVertices(player_bound), sweep)) {
        Canvas.strokeStyle(ctx, "#0000FF");
        draw_poly(ctx, sweep);
      };
    },
    sweeps);
};

let print_debug = (s: State.state, p: string, row: int) => {
  let ctx = layer_context(Debug, s);
  Canvas.font(ctx, "12px consolas");
  Canvas.fillStyle(ctx, "#FF0000");
  Canvas.fillText(ctx, p, 0, row * 12 + 12);
};
