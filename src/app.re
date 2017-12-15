[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");

let make = (~route, _children) => {
  ...component,
  render: (_self) => {
    let page =
      switch route {
      | Routing.Home => <Home />
      | Routing.Detail(id) => <DetailPage id />
      | Routing.Api => <Api />
      | Routing.TopPairs => <TopPairs />
      };
    <div className="app">
      <Header />
      <div className="container"> page </div>
    </div>
  }
};
