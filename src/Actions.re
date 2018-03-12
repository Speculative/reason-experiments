type actions =
  | Tick
  | PopulateManifest
  | ControlUpdate(Controls.controls)
  | ControlChange
  | Jump
  | NoOp;

