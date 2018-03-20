type boundingBox = (float, float, float, float);
type coord = (int, int);
type pvec = (float, float);

let collide = (o1: boundingBox, o2: boundingBox) =>
{
  let (l1, t1, r1, b1) = o1;
  let (l2, t2, r2, b2) = o2;
  !(l2 > r1 || r2 < l1 || t2 > b1 || b2 < t1)
};

let getBoundingBox = (x: int, y: int, w: int, h: int) => (
  float_of_int(x),
  float_of_int(y),
  float_of_int(x + w),
  float_of_int(y + h),
  );
let getCenter = (x: int, y: int, w: int, h: int) => (x + (w / 2), y + (h / 2));
let getBoxAround = (x: int, y: int, w: int, h: int) => (x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
let gridCoord = (x: int, y: int, w: int, h: int) => (x / w, y / h);
let boundVertices = ((l, t, r, b): boundingBox) => [(l, t), (r, t), (r, b), (l, b)];

let intBoundingBox = ((l, t, r, b): boundingBox) => (
  int_of_float(l),
  int_of_float(t),
  int_of_float(r),
  int_of_float(b),
  );

let move = ((l, t, r, b): boundingBox, (vx, vy): pvec, t: float) => {
  (l +. vx *. t,
   t +. vy *. t,
   r +. vx *. t,
   b +. vy *. t)
};

type sign =
  | Positive
  | Zero
  | Negative;

let toSign = (f: float) => {
  if (f > 0.) {
    Positive
  } else if (f == 0.) {
    Zero
  } else {
    Negative
  }
};

let getAreaSwept = ((l, t, r, b): boundingBox, (vx, vy): pvec, dt: float) => {
  let dx = vx *. dt;
  let dy = vy *. dt;
  switch (toSign(vx), toSign(vy)) {
  | (Zero, Zero) =>
    [
      (l, t),
      (r, t),
      (r, b),
      (l, b)
    ]
  | (Positive, Zero) =>
    [
      (l, t),
      (r +. dx, t),
      (r +. dx, b),
      (l, b)
    ]
  | (Negative, Zero) =>
    [
      (l +. dx, t),
      (r, t),
      (r, b),
      (l +. dx, b)
    ]
  | (Zero, Positive) =>
    [
      (l, t),
      (r, t),
      (r, b +. dy),
      (l, b +. dy)
    ]
  | (Zero, Negative) =>
    [
      (l, t +. dy),
      (r, t +. dy),
      (r, b),
      (l, b)
    ]
  | (Positive, Positive) =>
    [
      (l, t),
      (r, t),
      (r +. dx, t +. dy),
      (r +. dx, b +. dy),
      (l +. dx, b +. dy),
      (l, b)
    ]
  | (Positive, Negative) =>
    [
      (l, b),
      (l, t),
      (l +. dx, t +. dy),
      (r +. dx, t+. dy),
      (r +. dx, b +. dy),
      (r, b)
    ]
  | (Negative, Positive) =>
    [
      (r, t),
      (r, b),
      (r +. dx, b +. dy),
      (l +. dx, b +. dy),
      (l +. dx, t +. dy),
      (l, t)
    ]
  | (Negative, Negative) =>
    [
      (r, b),
      (l, b),
      (l +. dx, b +. dy),
      (l +. dx, t +. dy),
      (r +. dx, t +. dy),
      (r, t)
    ]
  };
};

let edges = (v: list(pvec)) => {
  let nv = List.length(v);
  List.mapi(
    (i, (v1x, v1y)) => {
      let (v2x, v2y) = List.nth(v, (i + 1) mod nv);
      (v2x -. v1x, v2y -. v1y)
    },
    v)
};

let dot = ((v1x, v1y): pvec, (v2x, v2y): pvec) => {
  v1x *. v2x +. v1y *. v2y
};

let separatingAxis = (v1: list(pvec), v2: list(pvec), e: pvec) => {
  let p1 = List.map(dot(e), v1);
  let p2 = List.map(dot(e), v2);
  let p1max = List.fold_left(max, List.hd(p1), p1);
  let p1min = List.fold_left(min, List.hd(p1), p1);
  let p2max = List.fold_left(max, List.hd(p2), p2);
  let p2min = List.fold_left(min, List.hd(p2), p2);
  p1min > p2max || p2min > p1max
};

let satCollision = (v1: list(pvec), v2: list(pvec)) => {
  let e1 = edges(v1);
  let e2 = edges(v2);
  !(List.rev_append(e1, e2)
    |> List.map(((ex, ey)) => (-1. *. ey, ex))
    |> List.map(separatingAxis(v1, v2))
    |> List.fold_left((b1, b2) => b1 || b2, false))
};

