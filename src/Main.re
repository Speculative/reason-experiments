open DOM;

let width = 600;
let height = 240;
let request = ref(0);
let gravity = 2.;

let updatesPerSecond = 60.;

let updateTicks = 1000. /. updatesPerSecond;

let neutralControls: Controls.controls = {
  left: false,
  right: false,
  up: false,
  down: false,
  jump: false,
};

let controlFrame = ref(neutralControls);

type rootStore = Reduce.t(State.state, Actions.actions);

let randRange = (min: float, max: float) =>
  Math.random() *. (max -. min) +. min;

let updateControls = (s: State.state, c: Controls.controls) => {
  let dispatch = ref([]);
  if (c != s.controls) {
    dispatch := [Actions.ControlChange, ...dispatch^];
  };

  if (c.jump && !s.controls.jump) {
    dispatch := [Actions.Jump, ...dispatch^];
  };

  ({
    ...s,
    controls: c
  },
  dispatch^)
};

let controlVelocity = (c: Controls.controls) => {
  let vx = switch (c.left, c.right) {
  | (true, false) => -1.
  | (false, true) => 1.
  | (true, true) => 0.
  | (false, false) => 0.
  };

  let vy = switch (c.up, c.down) {
  | (true, false) => -1.
  | (false, true) => 1.
  | (true, true) => 0.
  | (false, false) => 0.
  };

  (vx, vy)
};

let onControlChange = (s: State.state) => {
  let (svx, svy) = controlVelocity(s.controls);
  let p = if (svx > 0.) {
    Entity.flip(Entity.change_move(s.player, Walk), false)
  } else if (svx < 0.) {
    Entity.flip(Entity.change_move(s.player, Walk), true)
  } else {
    Entity.flip(Entity.change_move(s.player, Stand), s.player.spr.flip)
  };

  ({
    ...s,
    player: p
  },
  [])
};

let populateManifest = (s: State.state) => {
  ({
    ...s,
    manifest: Manifest.populate_manifest(s.manifest)
  },
  [])
};

let jump = (s: State.state) => {
  let jump = s.grounded && s.controls.jump;

  ({
    ...s,
    grounded: jump ? false : s.grounded,
    svy: jump ? -5. : s.svy
  },
  [])
};

let update = (s: State.state, ticks: float) => {
  let speed = 5.;
  let (svx, svy) = controlVelocity(s.controls);

  let sx' = s.sx +. svx *. speed;
  /* let sy' = s.sy +. s.svy; */
  let sy' = s.sy +. svy *. speed;

  let p = Entity.move(s.player, int_of_float(sx'), int_of_float(sy'));
  let (x, y, w, h) = (p.x, p.y, p.spr.def.w, p.spr.def.h);
  let player_bound = Physics.getBoundingBox(x, y, w, h);
  let (tx, ty) = Physics.gridCoord(x, y, w, h);
  let terrain = Map.get_terrain();
  let grounded = List.nth(List.nth(terrain, ty + 1), tx);

  ({
    ...s,
    t: s.t +. ticks,
    sx: sx',
    sy: sy',
    /* svy: grounded ? 0.0 : s.svy +. gravity, */
    grounded: grounded,
    player: Entity.tick_sprite(p, ticks)
  },
  [])
};

let render = (s: State.state) => {
  Draw.clear(s, Background);
  Draw.clear(s, Player);
  Draw.clear(s, Debug);

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

  Draw.draw_collision_test(s);

  Draw.draw_sprite(
    s,
    Player,
    s.player.spr,
    s.player.x, s.player.y);

  let player_bound = Physics.getBoundingBox(s.player.x, s.player.y, s.player.spr.def.w, s.player.spr.def.h);
  Draw.draw_bounding(s, player_bound, "#FF0000");
  /*
  Draw.print_debug(s, "(" ++ string_of_int(s.player.x) ++ "," ++ string_of_int(s.player.y) ++ ")", 0);
  Draw.print_debug(s, string_of_bool(s.grounded), 1);
  */
  ()
};

let rec frame = (store: rootStore, t': float) => {
  DOM.cancelAnimationFrame(request^);

  Reduce.drainDispatch(store, AsyncDispatch.getDispatch());
  AsyncDispatch.clearDispatch();
  Reduce.dispatch(store, ControlUpdate(controlFrame^));
  while (Reduce.getState(store).t < t') {
    Reduce.dispatch(store, Tick)
  };
  render(Reduce.getState(store));

  request := DOM.requestAnimationFrame(frame(store))
};

let reduce = (s: State.state, a: Actions.actions) =>
  switch a {
  | ControlUpdate(c) => updateControls(s, c)
  | ControlChange => onControlChange(s)
  | Jump => jump(s)
  | PopulateManifest => populateManifest(s)
  | Tick => update(s, updateTicks)
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

  let spriteCache: Immutable.IntMap.t(ManifestState.spriteCacheEntry) = Immutable.IntMap.empty();

  let initialState: State.state = {
    t: DOM.now(),
    l: {
      background: create_layer("bg"),
      player: create_layer("player"),
      debug: create_layer("debug"),
    },
    sx: float_of_int(width) /. 2.,
    sy: float_of_int(height) /. 2.,
    svx: 0.,
    svy: 0.,
    grounded: false,
    controls: neutralControls,
    player: Entity.make_entity(Player, 0, 0),
    manifest: {
      complete: false,
      sheetsLoaded: 0,
      spriteCache: spriteCache,
    }
  };

  let store = Reduce.create(initialState, reduce);
  Manifest.initialize();

  request := DOM.requestAnimationFrame(frame(store));
  ()
};

DOM.addEventListener(DOM.window, "load", bootstrap);
