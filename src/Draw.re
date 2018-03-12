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
  let (w, n, e, s) = bbox;
  Canvas.strokeStyle(ctx, color);
  Canvas.strokeRect(ctx, w, n, e - w, s - n);
};

let print_debug = (s: State.state, p: string, row: int) => {
  let ctx = layer_context(Debug, s);
  Canvas.font(ctx, "12px consolas");
  Canvas.fillStyle(ctx, "#FF0000");
  Canvas.fillText(ctx, p, 0, row * 12 + 12);
};
