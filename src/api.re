type action =
  | Loaded(CoinData.coins)
  | Loading;

type state = {
  coins: CoinData.coins,
  loading: bool
};

let component = ReasonReact.reducerComponent("Api");

let make = (_children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchCoinList(reduce(payload => Loaded(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,

    initialState: () => {
      coins: [||],
      loading: false
    },

    reducer: (action, state) =>
      switch action {
      | Loading => 
        ReasonReact.Update({...state, loading: true})
      
      | Loaded(data) =>
        ReasonReact.Update({
          coins: data,
          loading: false
        })
      },
    
    didMount: (self) => {
      load(self);
      ReasonReact.NoUpdate;
    },

    render: (self) => {
      <div>
        (
          if (Array.length(self.state.coins) > 0) {
            self.state.coins
            |> Array.mapi((index: int, coin: CoinData.coin) => {
              Js.log(coin);
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
    }
  }
};