open DOM;

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

type state = {
  t: float,
  l: layers,
  controls: controls,
  sx: float,
  sy: float,
  player: Entity.entityInst,
};

