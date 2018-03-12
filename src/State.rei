open DOM;

type layers = {
  player: Canvas.context,
  background: Canvas.context,
  debug: Canvas.context,
};

type state = {
  t: float,
  l: layers,
  controls: Controls.controls,
  sx: float,
  sy: float,
  svx: float,
  svy: float,
  grounded: bool,
  player: Entity.entityInst,
  manifest: ManifestState.state,
};

