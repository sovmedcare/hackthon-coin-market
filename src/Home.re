type action =
  | LoadedCoinList(CoinData.coins)
  | Loading;

type state = {
  coins: CoinData.coins,
  loading: bool
};

let component = ReasonReact.reducerComponent("Home");

let make = (_children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchCoinList(reduce(payload => LoadedCoinList(payload))) |> ignore;
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
  
    | LoadedCoinList(data) =>
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
      <div style=(
        ReactDOMRe.Style.make(
          ~display="flex",
          ~flexDirection="column",
          ~width="300px",
          ()
        )
      )>
      <div style=(
        ReactDOMRe.Style.make(
          ~display="flex",
          ~justifyContent="space-around",
          ~borderBottom="solid black 1px",
          ()
        )
      )>
        <div>{ReasonReact.stringToElement("id")}</div>
        <div>{ReasonReact.stringToElement("coinName")}</div>
      </div>
        {
          self.state.coins
          |> Array.map(
              (coin: CoinData.coin) => {
                <div style=(
                  ReactDOMRe.Style.make(
                    ~display="flex",
                    ~justifyContent="space-around",
                    ()
                  )
                )>
                  <div>{ReasonReact.stringToElement(coin.id)}</div>
                  <a href={"/#/detail/" ++ coin.name}>{ReasonReact.stringToElement(coin.coinName)}</a>
                </div>
              }
            )
          |> ReasonReact.arrayToElement
        }
      </div>
    }
  }
};