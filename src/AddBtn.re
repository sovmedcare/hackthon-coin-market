open Utils;

type action =
  | AddToObserveList(string)
  | RemoveFromObserveList(string);

type state = {added: bool};

let setItem = (coinName) => {
  Dom.Storage.(localStorage |> setItem(coinName, "true"));
};

let removeItem = (coinName) => {
  Dom.Storage.(localStorage |> removeItem(coinName));
};

let component = ReasonReact.reducerComponent("AddBtn");

let make = (~coinInfo: CoinData.coin, ~added: bool, ~onObseveListChange: unit => unit, ~style, _children) => {
  ...component,
  initialState: () => {added: added},
  reducer: (action, state) => {
    switch action {
      | RemoveFromObserveList(coinName) => ReasonReact.UpdateWithSideEffects({added: !state.added}, (_self) => {
        removeItem(coinName);
        onObseveListChange();
      })
      | AddToObserveList(coinName) => ReasonReact.UpdateWithSideEffects({added: !state.added}, (_self) => {
        setItem(coinName);
        onObseveListChange();        
      })
    }
  },
  render: (self) => {
    let reducer = self.state.added ? (_) => RemoveFromObserveList(coinInfo.name) : (_) => AddToObserveList(coinInfo.name);
    {
      <div style=(style) onClick={self.reduce(reducer)}>
        <button>{self.state.added ? textEl("remove") : textEl("add")}</button>
      </div>
    }
  }
};
