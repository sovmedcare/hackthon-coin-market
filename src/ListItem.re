type action =
  | LoadedCoinPrice(CoinData.price)
  | Loading;

type state = {
  price: CoinData.price,
  loading: bool
};

let cellStyle = ReactDOMRe.Style.make(~flex="1", ~textAlign="right", ());

let component = ReasonReact.reducerComponent("ListItem");

let make = (~coinInfo: CoinData.coin, _children) => {
  let loadPriceData = (coinName, {ReasonReact.state, reduce}) => {
    CoinData.fetchPrice(coinName, reduce(payload => LoadedCoinPrice(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,
    initialState: () => {
      price: {usd: 0., eur: 0., twd: 0.},
      loading: false
    },
    reducer: (action, state) =>
      switch action {
      | Loading =>  ReasonReact.Update({...state, loading: true})
      | LoadedCoinPrice(data) => ReasonReact.Update({price: data, loading: false})
      },
    didMount: (self) => {
      loadPriceData(coinInfo.name, self);
      ReasonReact.NoUpdate;
    },
    render: (self) => {
      <div style=(ReactDOMRe.Style.make(~display="flex", ()))>
        <a style=(cellStyle) href={"/#/detail/" ++ coinInfo.name}>{ReasonReact.stringToElement(coinInfo.coinName)}</a>
        <div style=(cellStyle)>{ReasonReact.stringToElement(string_of_float(self.state.price.usd))}</div>
        <div style=(cellStyle)>{ReasonReact.stringToElement(string_of_float(self.state.price.eur))}</div>
        <div style=(cellStyle)>{ReasonReact.stringToElement(string_of_float(self.state.price.twd))}</div>
      </div>
    }
  }
};