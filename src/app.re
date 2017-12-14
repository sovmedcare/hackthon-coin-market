[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~route, _children) => {
  ...component,
  render: (_self) => {
    let page =
      switch route {
      | Routing.Home => <h1>(ReasonReact.stringToElement("Hello"))</h1>
      | Routing.Detail(id) => <DetailPage id />
      | Routing.Api => <Api />
      };
    <div> page </div>
  }
};
