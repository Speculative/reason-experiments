open DOM;

type spriteCacheEntry = {
  s: list(DOM.element),
  sf: list(DOM.element)
};

type state = {
  complete: bool,
  sheetsLoaded: int,
  spriteCache: Immutable.IntMap.t(spriteCacheEntry)
};

