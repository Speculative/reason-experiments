module DOM = {
    type element;
    type context;
    type mouseMoveEvent;

    let window: element = [%bs.raw "window"];

    [@bs.val] external createElement : string => element = "document.createElement";
    [@bs.val] external appendChild : element => element = "document.body.appendChild";

    [@bs.send] external addEventListener : element => string => (unit => unit) => unit = "addEventListener";

    [@bs.set] external setWidth : element => int => unit = "width";
    [@bs.set] external setHeight : element => int => unit = "height";

    [@bs.set] external setOnMouseMove : element => (mouseMoveEvent => unit) => unit = "onmousemove";
    [@bs.get] external getMouseX: mouseMoveEvent => int = "clientX";
    [@bs.get] external getMouseY: mouseMoveEvent => int = "clientY";


    [@bs.val] external requestAnimationFrame : (float => unit) => int = "window.requestAnimationFrame";
    [@bs.val] external cancelAnimationFrame : int => unit = "window.cancelAnimationFrame";
    [@bs.val] external now : unit => float = "performance.now";

    [@bs.send] external getContext : element => string => context = "getContext";
};

module Canvas = {
    type context = DOM.context;

    /* Canvas API */
    [@bs.set] external fillStyle : context => string => unit = "fillStyle";
    [@bs.set] external strokeStyle : context => string => unit = "strokeStyle";
    [@bs.set] external globalAlpha : context => float => unit = "globalAlpha";
    [@bs.set] external globalCompositeOperation : context => string => unit = "globalCompositeOperation";

    [@bs.send] external beginPath : context => unit = "beginPath";
    [@bs.send] external fill : context => unit = "fill";
    [@bs.send] external stroke : context => unit = "stroke";
    [@bs.send] external fillRect : context => int => int => int => int => unit = "fillRect";
    [@bs.send] external clearRect : context => int => int => int => int => unit = "clearRect";
    [@bs.send] external arc : context => float => float => float => float => float => unit = "arc";
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
let mouseX = ref(width / 2);
let mouseY = ref(height / 2);
let colors = ["#69D2E7", "#A7DBD8", "#E0E4CC", "#F38630", "#FA6900", "#FF4E50", "#F9D423"];

type particle = {
    x : float,
    y : float,
    vx : float,
    vy : float,
    r : float,
    wander : float,
    theta : float,
    drag : float,
    color : string,
    alive : bool
};

type state = {
    t : float,
    particles : list(particle)
};

let randRange = (min: float, max: float) => {
    Math.random() *. (max -. min) +. min
};

let rec update = (c : DOM.context, s : state, t' : float) => {
    DOM.cancelAnimationFrame(request^);
    let dt = (t' -. s.t) /. 1000.;

    Canvas.clearRect(c, 0, 0, width, height);
    Canvas.globalCompositeOperation(c, "screen");

    let updatedParticles = List.map((p) => {
        Canvas.beginPath(c);
        Canvas.arc(c, p.x, p.y, p.r, 0., Math.pi *. 2.);
        Canvas.fillStyle(c, p.color);
        Canvas.fill(c);
        {
            x: p.x +. p.vx,
            y: p.y +. p.vy,
            vx: (p.vx *. p.drag) +. Math.cos(p.theta) *. 0.1,
            vy: (p.vy *. p.drag) +. Math.sin(p.theta) *. 0.1,
            theta: p.theta +. randRange(-0.5, 0.5) *. p.wander,
            r: p.r *. 0.96,
            alive: p.r > 0.5,
            wander: p.wander,
            drag: p.drag,
            color: p.color
        }
    }, s.particles);

    let updatedParticles = List.filter((p) => {
        p.alive
    }, updatedParticles);

    let theta = randRange(0., Math.pi *. 2.);
    let force = randRange(2., 8.);

    let newParticle : particle = {
        x: float_of_int(mouseX^),
        y: float_of_int(mouseY^),
        vx: Math.cos(theta) *. force,
        vy: Math.sin(theta) *. force,
        r: randRange(5., 40.),
        wander: randRange(0.5, 2.0),
        theta: randRange(0., Math.pi *. 2.),
        drag: randRange(0.9, 0.99),
        color: List.nth(colors, Math.floor(randRange(0., 7.))),
        alive: true
    };


    request := DOM.requestAnimationFrame(update(c, {
        t: t',
        particles: [newParticle, ...updatedParticles]
    }));
};

let bootstrap = () => {
    let canvas = DOM.createElement("canvas");
    let _ = DOM.appendChild(canvas);

    DOM.setWidth(canvas, width);
    DOM.setHeight(canvas, height);
    DOM.setOnMouseMove(canvas, (e) => {
        mouseX := DOM.getMouseX(e);
        mouseY := DOM.getMouseY(e);
    });

    let context = DOM.getContext(canvas, "2d");
    Canvas.clearRect(context, 0, 0, width, height);
    let _ = DOM.requestAnimationFrame(update(context, {
        t: DOM.now(),
        particles: []
    }));
};

DOM.addEventListener(DOM.window, "load", bootstrap);
