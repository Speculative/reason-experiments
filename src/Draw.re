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

let draw_sprite = (s: State.state, l: layer, si: Manifest.spriteInst, x: int, y: int) => {
  let (fx, fy) = Manifest.get_frame_offset(si);
  Canvas.drawImage(
    layer_context(l, s),
    Manifest.get_sprite_sheet(si.def.sheet).img,
    fx, fy,
    si.def.w, si.def.h,
    x, y,
    si.def.w, si.def.h);
  ()
};
