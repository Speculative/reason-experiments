open DOM;

type spriteCacheEntry = {
  s: list(Canvas.imageData),
  sf: list(Canvas.imageData)
};

type state = {
  complete: bool,
  sheetsLoaded: int,
  spriteCache: Immutable.IntMap.t(spriteCacheEntry)
};

