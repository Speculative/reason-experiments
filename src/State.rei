open DOM;
open Immutable;

type controls = {
  left: bool,
  right: bool,
  up: bool,
  down: bool,
  jump: bool,
};

type layers = {
  player: Canvas.context,
  background: Canvas.context,
};

type spriteCacheEntry = {
  s: list(DOM.imageElement),
  sr: list(DOM.imageElement)
};

type manifest = {
  spriteCache: HashMap.t(Manifest.sprite, spriteCacheEntry)
};

type state = {
  t: float,
  l: layers,
  controls: controls,
  sx: float,
  sy: float,
  player: Entity.entityInst,
};

