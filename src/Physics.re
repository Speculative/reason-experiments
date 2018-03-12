type boundingBox = (int, int, int, int);
type coord = (int, int);

let collide = (o1: boundingBox, o2: boundingBox) => {
  let (w1, n1, e1, s1) = o1;
  let (w2, n2, e2, s2) = o2;
  !(w2 > e1 || e2 < w1 || n2 > s1 || s2 < n1)
};

let getBoundingBox = (x: int, y: int, w: int, h: int) => (x, y, x + w, y + h);
let getCenter = (x: int, y: int, w: int, h: int) => (x + (w / 2), y + (h / 2));
let getBoxAround = (x: int, y: int, w: int, h: int) => (x - (w / 2), y - (h / 2), x + (w / 2), y + (h / 2));
let gridCoord = (x: int, y: int, w: int, h: int) => (x / w, y / h);
