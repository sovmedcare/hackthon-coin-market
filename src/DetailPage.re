open Utils;

[@bs.module] external create: CoinData.histories => unit = "./createChart";

type state = {
  coinName: string,
  histories: CoinData.histories,
  loading: bool,
};

type action =
  | LoadedHistories(CoinData.histories)
  | Loading(string);

let component = ReasonReact.reducerComponent("DetailPage");

let make = (~id, _children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchHistoryByHour(id, reduce(data => LoadedHistories(data))) |> ignore;
    reduce(() => Loading(id), ());
  };
  {
    ...component,
    initialState: () => {
      coinName: id,
      histories: [||],
      loading: false,
    },
    reducer: (action, state) =>
      switch action {
      | LoadedHistories(newData) => ReasonReact.Update({...state, histories: newData, loading: false}) 
      | Loading(coinName) => ReasonReact.Update({...state, coinName, loading: true})
      },
    didMount: (_self) => {
      Js.log("Didmount");
      load(_self);
      ReasonReact.NoUpdate
    },
    didUpdate: ({oldSelf, newSelf}) => {
      if (newSelf.state.coinName !== id) {
        Js.log(newSelf.state.coinName);
        Js.log(id);
        load(newSelf);
      }
    },
    render: (_self) => {
      <div>
        <h1> (textEl(id)) </h1>
        (
          if (!_self.state.loading && Array.length(_self.state.histories) > 0) {
            _self.state.histories
            |> create
            |> (unit) => textEl("")
          } else {
            Js.log("else");
            ReasonReact.nullElement
          }
        )
        <div id="chart"></div>
      </div>
    }
  }
};