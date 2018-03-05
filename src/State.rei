open DOM;

type layers = {
  player: Canvas.context,
  background: Canvas.context,
};

type spriteCacheEntry = {
  s: list(Canvas.imageData),
  sf: list(Canvas.imageData)
};

type manifest = {
  spriteCache: Immutable.IntMap.t(spriteCacheEntry)
};

type state = {
  t: float,
  l: layers,
  controls: Controls.controls,
  sx: float,
  sy: float,
  player: Entity.entityInst,
  manifest: manifest,
};

