open DOM;
open Sprites;

let make_sprite = (s: sprite): spriteInst => {
  {
    def: get_sprite_def(s),
    frame: 0,
    t: 0,
    flip: false
  }
};

/*
#################
# Image helpers #
#################
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

let get_frame_offset = (sdef: spriteDef, shdef: spriteSheetDef, fnum: int) => {
  (sdef.x + (((fnum * sdef.w) mod (shdef.w / sdef.w)) * sdef.w),
   sdef.y + ((fnum * sdef.w) / shdef.w))
};

let get_frame_img = (sdef: spriteDef, shdef: spriteSheetDef, img: DOM.imageElement, flip: bool) => {
  List.map(
    (fnum: int) => {
      let canvas = DOM.createElement("canvas");
      let {w, h} = sdef;
      DOM.setWidth(canvas, w);
      DOM.setHeight(canvas, h);
      let ctx = DOM.getContext(canvas, "2d");
      let (fx, fy) = get_frame_offset(sdef, shdef, fnum);
      Canvas.drawImage(
        ctx,
        img,
        fx, fy,
        w, h,
        0, 0,
        w, h);
      let imgData = if (flip) {
        hflip_img(Canvas.getImageData(ctx, 0, 0, w, h));
      } else {
        Canvas.getImageData(ctx, 0, 0 , w, h)
      };
      Canvas.clearRect(ctx, 0, 0, w, h);
      Canvas.putImageData(ctx, imgData, 0, 0);

      canvas
    },
    range(0, sdef.frames))
};


/*
###########
# Loading #
###########
 */
let loaded: ref(list((spriteSheet, DOM.imageElement))) = ref([]);

let load_sprite_sheet = (sheet: spriteSheet) => {
  let img = DOM.image();
  let sheetDef = get_sprite_sheet_def(sheet);

  DOM.setOnLoad(img, () => {
    loaded := [(sheet, img), ...loaded^];
    AsyncDispatch.asyncDispatch(PopulateManifest);
    ()
  });

  DOM.setSrc(img, sheetDef.url);
};

let populate_manifest = (state: ManifestState.state): ManifestState.state => {
  let sheetsLoaded' = state.sheetsLoaded + List.length(loaded^);
  let spriteCache' = Immutable.IntMap.mutate(state.spriteCache);
  List.iter(
    (loadedSheet: (spriteSheet, DOM.imageElement)) => {
      let (sheet: spriteSheet, img: DOM.imageElement) = loadedSheet;
      let sheetDef = get_sprite_sheet_def(sheet);
      List.iter(
        (sprite: sprite) => {
          let spriteDef = get_sprite_def(sprite);
          let cacheEntry: ManifestState.spriteCacheEntry = {
            s: get_frame_img(spriteDef, sheetDef, img, false),
            sf: get_frame_img(spriteDef, sheetDef, img, true)
          };
          Js.log2(spriteDef, cacheEntry);
          Immutable.IntMap.Transient.put(
            get_sprite_id(sprite),
            cacheEntry,
            spriteCache');
          ()
        },
        sheetDef.sprites);
    },
    loaded^);

  {
    complete: sheetsLoaded' == Sprites.numSheets,
    sheetsLoaded: sheetsLoaded',
    spriteCache: Immutable.IntMap.Transient.persist(spriteCache'),
  }
};

let initialize = () => {
  List.iter(load_sprite_sheet, Sprites.spriteSheets);
  ()
};

