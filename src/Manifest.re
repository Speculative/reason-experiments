open DOM;
open Sprites;

let get_frame_offset = (s: spriteInst) => {
  let shdef = get_sprite_sheet_def(s.def.sheet);
  (s.def.x + (((s.frame * s.def.w) mod (shdef.w / s.def.w)) * s.def.w),
   s.def.y + ((s.frame * s.def.w) / shdef.w))
};

let make_sprite = (s: sprite): spriteInst => {
  {
    def: get_sprite_def(s),
    frame: 0,
    t: 0,
    flip: false
  }
};

let get_sprite_img = (s: spriteInst) => {

};

/*
let loaded: ref(list((spriteSheet, DOM.imageElement))) = ref([]);

let load_sprite_sheet = (sheet: spriteSheet) => {
  let img = DOM.image();
  let sheetDef = get_sprite_sheet_def(sheet);

  DOM.setOnLoad(img, () => {
    loaded := [(s, img), ...loaded^];
    let dependentSprites = get_sheet_sprites(s);
    AsyncDispatch.asyncDispatch(NoOp);
    ()
  });

  DOM.setSrc(img, sheetDef.url);
};
*/

let range = (s: int, e: int) => {
  let l = ref([]);
  for (i in s to e - 1) {
    l := [i, ...l^];
  };
  List.rev(l^)
};

let hflip_img = (img: Canvas.imageData): Canvas.imageData => {
  let data = Canvas.data(img);
  let w = Canvas.width(img);
  let h = Canvas.height(img);

  let flipped = Js.Array.mapi(
    (v: int, i: int) => {
      let px = (i / 4) mod w;
      let py = (i / 4) / h;
      let po = i mod 4;
      let px' = w - px - 1;
      let i' = ((py * w) + px') * 4 + po;
      data[i']
    },
    Array.of_list(range(0, w * h * 4))
  );

  Canvas.newImageData(
    Uint8ClampedArray.from(
      flipped
    ),
    w, h
  )
};

