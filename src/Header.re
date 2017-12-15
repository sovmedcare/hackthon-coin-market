open Utils;

[%bs.raw {|require('./Header.css')|}];

let component = ReasonReact.statelessComponent("Header");

let make = (_children) => {
  ...component,

  render: (self) => {
    <div className="header">
      <div className="title">(textEl("Crypto Market Price Live Demo"))</div>
      <div className="nav-link-list">
        <a className="nav-link" href=("#/")>(textEl("All coin list"))</a>
        <a className="nav-link" href=("#/topPairs")>(textEl("Top traded coin"))</a>
      </div>
    </div>
  }
};