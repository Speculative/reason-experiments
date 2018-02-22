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
  let ctx = layer_context(l, s);
  if (si.flip) {
    Canvas.drawImage(
      ctx,
      Manifest.get_sprite_sheet(si.def.sheet).img,
      fx, fy,
      si.def.w, si.def.h,
      x, y,
      si.def.w, si.def.h);
    let img = Canvas.getImageData(ctx, x, y, si.def.w, si.def.h);
    Canvas.clearRect(
      ctx,
      x, y,
      si.def.w, si.def.h);
    Canvas.putImageData(ctx, Manifest.hflip_img(img), x, y);
  } else {
    Canvas.drawImage(
      ctx,
      Manifest.get_sprite_sheet(si.def.sheet).img,
      fx, fy,
      si.def.w, si.def.h,
      x, y,
      si.def.w, si.def.h);
  };
  ()
};
