let component = ReasonReact.statelessComponent("DetailPage");

let make = (~id, _children) => {
  ...component,
  render: (_self) => {
    <div>
      <h1> (ReasonReact.stringToElement(string_of_int(id))) </h1>
    </div>
  }
};