open DOM;
open MyManifest;

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

let draw_sprite = (s: State.state, l: layer, si: MySprites.spriteInst, x: int, y: int) => {
  let (fx, fy) = MyManifest.get_frame_offset(si);
  let ctx = layer_context(l, s);
  if (si.flip) {
    Canvas.drawImage(
      ctx,
      MySprites.get_sprite_sheet_def(si.def.sheet).img,
      fx, fy,
      si.def.w, si.def.h,
      x, y,
      si.def.w, si.def.h);
    let img = Canvas.getImageData(ctx, x, y, si.def.w, si.def.h);
    Canvas.clearRect(
      ctx,
      x, y,
      si.def.w, si.def.h);
    Canvas.putImageData(ctx, MyManifest.hflip_img(img), x, y);
  } else {
    Canvas.drawImage(
      ctx,
      MySprites.get_sprite_sheet_def(si.def.sheet).img,
      fx, fy,
      si.def.w, si.def.h,
      x, y,
      si.def.w, si.def.h);
  };
  ()
};

