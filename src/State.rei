open DOM;

type layers = {
  player: Canvas.context,
  background: Canvas.context,
};

type state = {
  t: float,
  l: layers,
  controls: Controls.controls,
  sx: float,
  sy: float,
  player: Entity.entityInst,
  manifest: ManifestState.state,
};

