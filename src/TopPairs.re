open Utils;

[%bs.raw {|require('./TopPairs.css')|}];

type state = {
  currency: string,
  topPairs: CoinData.topPairs,
  loading: bool
};

type action =
  | Toggle(string)
  | Loaded(CoinData.topPairs)
  | Loading;

let component = ReasonReact.reducerComponent("TopPairs");

let make = (_children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchTopPairs(state.currency, reduce(payload => Loaded(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,
    initialState: () => {
      currency: "BTC",
      topPairs: [||],
      loading: false
    },
    reducer: (action, state) =>
    switch action {
    | Loading =>
      ReasonReact.Update({...state, loading: true})
    | Loaded(data) => {
      ReasonReact.Update({
        ...state,
        topPairs: data,
        loading: false
      })
    }
    | Toggle(currency) => {
      ReasonReact.UpdateWithSideEffects({
        ...state,
        currency: currency,
        loading: false
      }, (self) => load(self))
    }
    },
    didMount: (self) => {
      load(self);
      ReasonReact.NoUpdate;
    },
    render: (self) => {
      <div>
        <div className="select-list">
          {
            [|"BTC", "LTC", "ETH"|]
            |> Array.map(
              (currency: string) => {
                <div className="select-item" onClick=(self.reduce((e)=>Toggle(currency)))>{textEl(currency)}</div>
              }
            )
            |> arrayEl
          }
        </div>
        <div className="table">
          <div className="th">
            <div className="td top">{textEl("Top")}</div>
            <div className="td currency">{textEl("from")}</div>
            <div className="td currency">{textEl("to")}</div>
            <div className="td">{textEl("volumn in 24hr")}</div>
            <div className="td">{textEl("volumn to in 24hr")}</div>
          </div>
          {
            self.state.topPairs
            |> Array.mapi(
                (index: int, topPair: CoinData.topPair) => {
                  <div className="tr">
                    <div className="td top">{textEl(string_of_int(index+1))}</div>
                    <div className="td currency">{textEl(topPair.fromSymbol)}</div>
                    <div className="td currency">{textEl(topPair.toSymbol)}</div>
                    <div className="td">{textEl(string_of_float(topPair.volume24h))}</div>
                    <div className="td">{textEl(string_of_float(topPair.volume24hTo))}</div>
                  </div>
                }
              )
            |> arrayEl
          }
        </div>
      </div>
    }
  }
};