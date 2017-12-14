type action =
  | LoadedCoinList(CoinData.coins)
  | LoadedHistories(CoinData.histories)
  | LoadedPrice(CoinData.price)
  | LoadedTopPairs(CoinData.topPairs)
  | Loading;

type state = {
  coins: CoinData.coins,
  histories: CoinData.histories,
  price: option(CoinData.price),
  loading: bool
};

let component = ReasonReact.reducerComponent("Api");

let make = (_children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchCoinList(reduce(payload => LoadedCoinList(payload))) |> ignore;
    CoinData.fetchHistoryByHour(reduce(payload => LoadedHistories(payload))) |> ignore;
    CoinData.fetchHistoryByDay(reduce(payload => LoadedHistories(payload))) |> ignore;
    CoinData.fetchTopPairs(reduce(payload => LoadedTopPairs(payload))) |> ignore;
    CoinData.fetchPrice("BTC", reduce(payload => LoadedPrice(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,

    initialState: () => {
      coins: [||],
      histories: [||],
      price: None,
      loading: false
    },

    reducer: (action, state) =>
      switch action {
      | Loading => 
        ReasonReact.Update({...state, loading: true})

      | LoadedCoinList(data) =>
        ReasonReact.Update({
          ...state,
          coins: data,
          loading: false
        })
      
      | LoadedHistories(data) =>
        ReasonReact.Update({
          ...state,
          histories: data,
          loading: false
        })

      | LoadedPrice(data) =>
        ReasonReact.Update({
          ...state,
          loading: false
        })

      | LoadedTopPairs(data) =>
        ReasonReact.Update({
          ...state,
          loading: false
        })
      },
    
    didMount: (self) => {
      load(self);
      ReasonReact.NoUpdate;
    },

    render: (self) => {
      <div>
        <div>
        (
          if (Array.length(self.state.coins) > 0) {
            self.state.coins
            |> Array.mapi((index: int, coin: CoinData.coin) => {
              <div>
                <div>{ReasonReact.stringToElement(coin.id)}</div>
                <div>{ReasonReact.stringToElement(coin.coinName)}</div>
                <div>{ReasonReact.stringToElement(coin.name)}</div>
              </div>
            })
            |> ReasonReact.arrayToElement
          } else {
            ReasonReact.nullElement
          }
        )
        </div>
        <div>
        (
          if (Array.length(self.state.histories) > 0) {
            self.state.histories
            |> Array.mapi((index: int, history: CoinData.history) => {
              <div>
                <div>{ReasonReact.stringToElement(string_of_float(history.high))}</div>
                <div>{ReasonReact.stringToElement(string_of_float(history.low))}</div>
              </div>
            })
            |> ReasonReact.arrayToElement
          } else {
            ReasonReact.nullElement
          }
        )
        </div>
      </div>
    }
  }
};