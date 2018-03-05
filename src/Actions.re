type actions =
  | Tick
  | ControlUpdate(Controls.controls)
  | ControlChange
  | NoOp;

