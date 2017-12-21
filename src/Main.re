open DOM;

let width = 800;
let height = 600;
let request = ref(0);

let colors = [
  "#69D2E7",
  "#A7DBD8",
  "#E0E4CC",
  "#F38630",
  "#FA6900",
  "#FF4E50",
  "#F9D423"
];

let updatesPerSecond = 60.;

let updateTicks = 1000. /. updatesPerSecond;

type particle = {
  x: float,
  y: float,
  vx: float,
  vy: float,
  r: float,
  wander: float,
  theta: float,
  drag: float,
  color: string,
  alive: bool
};

type controls = {
  left: bool,
  right: bool,
  up: bool,
  down: bool,
  jump: bool,
};

let neutralControls = {
  left: false,
  right: false,
  up: false,
  down: false,
  jump: false,
};

let controlFrame = ref(neutralControls);

type state = {
  c: Canvas.context,
  t: float,
  currentControls: controls,
  pastControls: controls,
  sx: float,
  sy: float,
  player: Manifest.spriteInst,
};

type actions =
  | Tick
  | ControlUpdate(controls)
  | NoOp;

type rootStore = Reduce.t(state, actions);

let randRange = (min: float, max: float) =>
  Math.random() *. (max -. min) +. min;

let updateControls = (s: state, c: controls) => {
  {
    ...s,
    currentControls: c,
    pastControls: s.currentControls
  }
};

let update = (s: state, ticks: float) => {
  let speed = 5.;

  let svx = switch (s.currentControls.left, s.currentControls.right) {
  | (true, false) => -1.
  | (false, true) => 1.
  | (true, true) => 0.
  | (false, false) => 0.
  };

  let svy = switch (s.currentControls.up, s.currentControls.down) {
  | (true, false) => -1.
  | (false, true) => 1.
  | (true, true) => 0.
  | (false, false) => 0.
  };

  let sx' = s.sx +. svx *. speed;
  let sy' = s.sy +. svy *. speed;

  {
    ...s,
    t: s.t +. ticks,
    sx: sx',
    sy: sy',
    currentControls: s.currentControls,
    pastControls: s.pastControls,
    player: {
      ...s.player,
      t: s.player.t + int_of_float(ticks),
      frame: ((s.player.t + int_of_float(ticks)) / s.player.def.tframe) mod s.player.def.frames
    }
  }
};

let render = (s: state) => {
  Canvas.clearRect(s.c, 0, 0, width, height);
  let sdef = s.player.def;
  let (fx, fy) = Manifest.get_frame_offset(s.player);
  Canvas.drawImage(
    s.c,
    Manifest.get_sprite_sheet(sdef.sheet).img,
    fx, fy,
    sdef.w, sdef.h,
    int_of_float(s.sx), int_of_float(s.sy),
    sdef.w, sdef.h);
  Js.log2(fx, fy);
  ()
};

let rec frame = (store: rootStore, t': float) => {
  DOM.cancelAnimationFrame(request^);

  Reduce.dispatch(store, ControlUpdate(controlFrame^));
  while (Reduce.getState(store).t < t') {
    Reduce.dispatch(store, Tick)
  };
  render(Reduce.getState(store));

  request := DOM.requestAnimationFrame(frame(store))
};

let reduce = (s: state, a: actions) =>
  switch a {
  | ControlUpdate(c) => (updateControls(s, c), [])
  | Tick => (update(s, updateTicks), [])
  | NoOp => (s, [])
  };

let bootstrap = () => {
  let canvas = DOM.createElement("canvas");
  let _ = DOM.appendChild(canvas);
  DOM.setWidth(canvas, width);
  DOM.setHeight(canvas, height);
  DOM.setOnKeyDown(
    DOM.document,
    (e) => {
      switch (DOM.getKey(e)) {
      | "ArrowLeft" => controlFrame := {...controlFrame^, left: true}
      | "ArrowRight" => controlFrame := {...controlFrame^, right: true}
      | "ArrowUp" => controlFrame := {...controlFrame^, up: true}
      | "ArrowDown" => controlFrame := {...controlFrame^, down: true}
      | "s" => controlFrame := {...controlFrame^, jump: true}
      | _ => ()
      };
    }
  );
  DOM.setOnKeyUp(
    DOM.document,
    (e) => {
      switch (DOM.getKey(e)) {
      | "ArrowLeft" => controlFrame := {...controlFrame^, left: false}
      | "ArrowRight" => controlFrame := {...controlFrame^, right: false}
      | "ArrowUp" => controlFrame := {...controlFrame^, up: false}
      | "ArrowDown" => controlFrame := {...controlFrame^, down: false}
      | "s" => controlFrame := {...controlFrame^, jump: false}
      | _ => ()
      };
    }
  );
  let context = DOM.getContext(canvas, "2d");

  let initialState = {
    c: context,
    t: DOM.now(),
    sx: float_of_int(width) /. 2.,
    sy: float_of_int(height) /. 2.,
    currentControls: neutralControls,
    pastControls: neutralControls,
    player: Manifest.make_sprite(Player),
  };

  let store = Reduce.create(initialState, reduce);
  Manifest.initialize();

  let _ = DOM.requestAnimationFrame(frame(store));
  ()
};

DOM.addEventListener(DOM.window, "load", bootstrap);
