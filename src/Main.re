module DOM = {
  type element;
  type context;
  type mouseMoveEvent;
  type keyboardEvent;
  let window: element = [%bs.raw "window"];
  let document: element = [%bs.raw "document"];
  [@bs.val] external createElement : string => element =
    "document.createElement";
  [@bs.val] external appendChild : element => element =
    "document.body.appendChild";
  [@bs.send]
  external addEventListener : (element, string, unit => unit) => unit =
    "addEventListener";
  [@bs.set] external setWidth : (element, int) => unit = "width";
  [@bs.set] external setHeight : (element, int) => unit = "height";
  [@bs.set] external setOnMouseMove : (element, mouseMoveEvent => unit) => unit = "onmousemove";
  [@bs.get] external getMouseX : mouseMoveEvent => int = "clientX";
  [@bs.get] external getMouseY : mouseMoveEvent => int = "clientY";

  [@bs.set] external setOnKeyDown : (element, keyboardEvent => unit) => unit = "onkeydown";
  [@bs.set] external setOnKeyUp : (element, keyboardEvent => unit) => unit = "onkeyup";
  [@bs.get] external getKey : keyboardEvent => string = "key";

  [@bs.val] external requestAnimationFrame : (float => unit) => int =
    "window.requestAnimationFrame";
  [@bs.val] external cancelAnimationFrame : int => unit =
    "window.cancelAnimationFrame";
  [@bs.val] external now : unit => float = "performance.now";
  [@bs.send] external getContext : (element, string) => context = "getContext";
};

module Canvas = {
  type context = DOM.context;
  /* Canvas API */
  [@bs.set] external fillStyle : (context, string) => unit = "fillStyle";
  [@bs.set] external strokeStyle : (context, string) => unit = "strokeStyle";
  [@bs.set] external globalAlpha : (context, float) => unit = "globalAlpha";
  [@bs.set] external globalCompositeOperation : (context, string) => unit =
    "globalCompositeOperation";
  [@bs.send] external beginPath : context => unit = "beginPath";
  [@bs.send] external fill : context => unit = "fill";
  [@bs.send] external stroke : context => unit = "stroke";
  [@bs.send] external fillRect : (context, int, int, int, int) => unit =
    "fillRect";
  [@bs.send] external clearRect : (context, int, int, int, int) => unit =
    "clearRect";
  [@bs.send]
  external arc : (context, float, float, float, float, float) => unit =
    "arc";
};

module Math = {
  [@bs.val] external floor : float => int = "Math.floor";
  [@bs.val] external ceil : float => int = "Math.ceil";
  [@bs.val] external pi : float = "Math.PI";
  [@bs.val] external sin : float => float = "Math.sin";
  [@bs.val] external cos : float => float = "Math.cos";
  [@bs.val] external random : unit => float = "Math.random";
};

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
  particles: list(particle)
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

  let updatedParticles =
    List.map(
      (p) => {
        x: p.x +. p.vx,
        y: p.y +. p.vy,
        vx: p.vx *. p.drag +. Math.cos(p.theta) *. 0.1,
        vy: p.vy *. p.drag +. Math.sin(p.theta) *. 0.1,
        theta: p.theta +. randRange((-0.5), 0.5) *. p.wander,
        r: p.r *. 0.96,
        alive: p.r > 0.5,
        wander: p.wander,
        drag: p.drag,
        color: p.color
      },
      s.particles
    );
  let updatedParticles = List.filter((p) => p.alive, updatedParticles);
  let theta = randRange(0., Math.pi *. 2.);
  let force = randRange(2., 8.);
  let newParticle: particle = {
    x: sx',
    y: sy',
    vx: Math.cos(theta) *. force,
    vy: Math.sin(theta) *. force,
    r: s.currentControls.jump && !s.pastControls.jump ? 100. : randRange(5., 40.),
    wander: randRange(0.5, 2.0),
    theta: randRange(0., Math.pi *. 2.),
    drag: randRange(0.9, 0.99),
    color: List.nth(colors, Math.floor(randRange(0., 7.))),
    alive: true
  };

  {
    ...s,
    particles: [newParticle, ...updatedParticles],
    t: s.t +. ticks,
    sx: sx',
    sy: sy',
    currentControls: s.currentControls,
    pastControls: s.pastControls
  }
};

let render = (s: state) => {
  Canvas.clearRect(s.c, 0, 0, width, height);
  Canvas.globalCompositeOperation(s.c, "screen");
  let _ =
    List.map(
      (p) => {
        Canvas.beginPath(s.c);
        Canvas.arc(s.c, p.x, p.y, p.r, 0., Math.pi *. 2.);
        Canvas.fillStyle(s.c, p.color);
        Canvas.fill(s.c)
      },
      s.particles
    );
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
    particles: [],
    sx: float_of_int(width) /. 2.,
    sy: float_of_int(height) /. 2.,
    currentControls: neutralControls,
    pastControls: neutralControls
  };

  let store = Reduce.create(initialState, reduce);

  let _ = DOM.requestAnimationFrame(frame(store));
  ()
};

DOM.addEventListener(DOM.window, "load", bootstrap);
