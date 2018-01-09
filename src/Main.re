open DOM;

let width = 600;
let height = 240;
let request = ref(0);

let updatesPerSecond = 60.;

let updateTicks = 1000. /. updatesPerSecond;

let neutralControls: State.controls = {
  left: false,
  right: false,
  up: false,
  down: false,
  jump: false,
};

let controlFrame = ref(neutralControls);

type actions =
  | Tick
  | ControlUpdate(State.controls)
  | NoOp;

type rootStore = Reduce.t(State.state, actions);

let randRange = (min: float, max: float) =>
  Math.random() *. (max -. min) +. min;

let updateControls = (s: State.state, c: State.controls) => {
  {
    ...s,
    currentControls: c,
    pastControls: s.currentControls
  }
};

let update = (s: State.state, ticks: float) => {
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

  let p = if (abs(int_of_float(svx)) > 0 && s.currentControls != s.pastControls) {
    if (svx > 0.) {
      Entity.flip(Entity.change_move(s.player, Walk), false)
    } else {
      Entity.flip(Entity.change_move(s.player, Walk), true)
    }
  } else if (svx == 0.) {
    Entity.change_move(s.player, Stand)
  } else {
    s.player
  };

  let sx' = s.sx +. svx *. speed;
  let sy' = s.sy +. svy *. speed;

  let p = Entity.move(p, int_of_float(sx'), int_of_float(sy'));

  {
    ...s,
    t: s.t +. ticks,
    sx: sx',
    sy: sy',
    currentControls: s.currentControls,
    pastControls: s.pastControls,
    player: Entity.tick_sprite(p, ticks)
  }
};

let render = (s: State.state) => {
  Draw.clear(s, Background);
  Draw.clear(s, Player);

  let bg = Map.get_map();
  for (y in 0 to 9) {
    for (x in 0 to 24) {
      let tile = List.nth(List.nth(bg, y), x);
      Draw.draw_sprite(
        s,
        Background,
        tile,
        x * 24, y * 24);
    };
  };

  Draw.draw_sprite(
    s,
    Player,
    s.player.spr,
    s.player.x, s.player.y);
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

let reduce = (s: State.state, a: actions) =>
  switch a {
  | ControlUpdate(c) => (updateControls(s, c), [])
  | Tick => (update(s, updateTicks), [])
  | NoOp => (s, [])
  };

let create_layer = (id: string) => {
  let canvas = DOM.createElement("canvas");
  let _ = DOM.appendChild(canvas);
  DOM.setWidth(canvas, Config.width);
  DOM.setHeight(canvas, Config.height);
  DOM.setId(canvas, id);

  DOM.getContext(canvas, "2d");
};

let bootstrap = () => {
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

  let initialState: State.state = {
    t: DOM.now(),
    l: {
      background: create_layer("bg"),
      player: create_layer("player"),
    },
    sx: float_of_int(width) /. 2.,
    sy: float_of_int(height) /. 2.,
    currentControls: neutralControls,
    pastControls: neutralControls,
    player: Entity.make_entity(Player, 0, 0),
  };

  let store = Reduce.create(initialState, reduce);
  Manifest.initialize();

  request := DOM.requestAnimationFrame(frame(store));
  ()
};

DOM.addEventListener(DOM.window, "load", bootstrap);
