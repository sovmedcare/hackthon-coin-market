[@bs.module] external create : unit => unit = "./createChart";

let component = ReasonReact.statelessComponent("DetailPage");

let make = (~id, _children) => {
  ...component,
  didMount: (_self) => {
    create();
    ReasonReact.Update()
  },
  render: (_self) => {
    <div>
      <h1> (ReasonReact.stringToElement(string_of_int(id))) </h1>
      <div id="chart"></div>
    </div>
  }
};