open DOM;

module type Sprites = {
  type spriteSheet;
  type sprite;

  /*
  ########################
  # Resource definitions #
  ########################
   */
  type spriteSheetDef = {
    /*
    url: string,
    */
    img: DOM.imageElement,
    w: int,
    h: int,
  };

  type spriteDef = {
    sheet: spriteSheet,
    x: int,
    y: int,
    w: int,
    h: int,
    frames: int,
    tframe: int,
  };

  /*
  #####################
  # Runtime resources #
  #####################
   */
  type spriteInst = {
    def: spriteDef,
    frame: int,
    t: int,
    flip: bool
  };

  let get_sprite_sheet_def: (spriteSheet) => spriteSheetDef;
  let get_sprite_def: (sprite) => spriteDef;
};

module MakeManifest = (Spr: Sprites) => {
  let get_frame_offset = (s: Spr.spriteInst) => {
    let shdef = Spr.get_sprite_sheet_def(s.def.sheet);
    (s.def.x + (((s.frame * s.def.w) mod (shdef.w / s.def.w)) * s.def.w),
     s.def.y + ((s.frame * s.def.w) / shdef.w))
  };

  let make_sprite = (s: Spr.sprite): Spr.spriteInst => {
    {
      def: Spr.get_sprite_def(s),
      frame: 0,
      t: 0,
      flip: false
    }
  };

  let get_sprite_img = (s: Spr.spriteInst) => {

  };

  let loaded: ref(list((Spr.spriteSheet, DOM.imageElement))) = ref([]);

  /*
  let load_sprite_sheet = (s: Spr.spriteSheet) => {
    let img = DOM.image();
    DOM.setOnLoad(img, () => {
      loaded := [(s, img), ...loaded^];
      ()
    });
    DOM.setSrc(img, Spr.get_sprite_sheet_def(s).url);
  };
  */

  let rec range = (s: int, e: int) => {
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
}

