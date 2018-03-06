type actions =
  | Tick
  | PopulateManifest
  | ControlUpdate(Controls.controls)
  | ControlChange
  | NoOp;

