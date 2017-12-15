let toBool = Js.to_bool;

let intEl = (n) =>
n |> string_of_int
  |> ReasonReact.stringToElement;

let floatEl = (n) =>
n |> string_of_float
  |> ReasonReact.stringToElement;

let textEl = ReasonReact.stringToElement;
let arrayEl = ReasonReact.arrayToElement;

let unsafe_elem: (string, array(string)) => bool = [%bs.raw
{|
function (item, arr) {
  return toBool(arr.includes(item))
}
|}
];