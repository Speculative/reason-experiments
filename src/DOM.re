module DOM = {
  type element;
  type imageElement;
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
  [@bs.set] external setId : (element, string) => unit = "id";
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

  [@bs.new] external image : unit => imageElement = "Image";
  [@bs.set] external setSrc : (imageElement, string) => unit = "src";
  [@bs.set] external setOnLoad : (imageElement, unit => unit) => unit = "onload";
};

module Uint8ClampedArray = {
  type t;
  [@bs.val] external from : (Js.Array.t(int)) => t = "Uint8ClampedArray.from";
};

module Canvas = {
  type context = DOM.context;
  type imageData;
  /* Canvas API */
  [@bs.set] external fillStyle : (context, string) => unit = "fillStyle";
  [@bs.set] external strokeStyle : (context, string) => unit = "strokeStyle";
  [@bs.set] external globalAlpha : (context, float) => unit = "globalAlpha";
  [@bs.set] external globalCompositeOperation : (context, string) => unit = "globalCompositeOperation";
  [@bs.send] external save : context => unit = "save";
  [@bs.send] external restore : context => unit = "restore";
  [@bs.send] external scale : context => float => float => unit = "scale";
  [@bs.send] external beginPath : context => unit = "beginPath";
  [@bs.send] external fill : context => unit = "fill";
  [@bs.send] external stroke : context => unit = "stroke";
  [@bs.send] external fillRect : (context, int, int, int, int) => unit = "fillRect";
  [@bs.send] external clearRect : (context, int, int, int, int) => unit = "clearRect";
  [@bs.send] external arc : (context, float, float, float, float, float) => unit = "arc";
  [@bs.send] external drawImage : (context, DOM.imageElement, int, int, int, int, int, int, int, int) => unit = "drawImage";
  [@bs.send] external getImageData : (context, int, int, int, int) => imageData = "getImageData";
  [@bs.send] external putImageData : (context, imageData, int, int) => unit = "putImageData";

  /* ImageData */
  [@bs.get] external width : (imageData) => int = "width";
  [@bs.get] external height : (imageData) => int = "height";
  /* this is a lie, it isn't Js.Array */
  [@bs.get] external data : (imageData) => Js.Array.t(int) = "data";
  [@bs.new] external newImageData : (Uint8ClampedArray.t, int, int) => imageData = "ImageData";
};

module Math = {
  [@bs.val] external floor : float => int = "Math.floor";
  [@bs.val] external ceil : float => int = "Math.ceil";
  [@bs.val] external pi : float = "Math.PI";
  [@bs.val] external sin : float => float = "Math.sin";
  [@bs.val] external cos : float => float = "Math.cos";
  [@bs.val] external random : unit => float = "Math.random";
};

