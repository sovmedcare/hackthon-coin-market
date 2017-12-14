let intEl = (n) =>
n |> string_of_int
  |> ReasonReact.stringToElement;

let textEl = ReasonReact.stringToElement;
let arrayEl = ReasonReact.arrayToElement;