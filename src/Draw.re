open DOM;

type layer =
  | Player
  | Background;

let layer_context = (l: layer, s: State.state) => {
  switch l {
  | Player => s.l.player
  | Background => s.l.background
  };
};

let clear = (s: State.state, l: layer) => {
  Canvas.clearRect(
    layer_context(l, s),
    0, 0,
    Config.width, Config.height);
};

let get_sprite_img = (s: State.state, si: Sprites.spriteInst, ctx: Canvas.context) => {
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
  | None => Canvas.createImagedata(ctx, si.def.w, si.def.h)
  };
};

let draw_sprite = (s: State.state, l: layer, si: Sprites.spriteInst, x: int, y: int) => {
  let ctx = layer_context(l, s);
  let spriteImg = get_sprite_img(s, si, ctx);
  Canvas.putImageData(ctx, spriteImg, x, y);
  ()
};

