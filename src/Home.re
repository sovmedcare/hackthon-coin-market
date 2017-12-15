type action =
  | LoadedCoinList(CoinData.coins)
  | Loading;

type state = {
  coins: CoinData.coins,
  loading: bool
};

let containerStyle = ReactDOMRe.Style.make(
  ~display="flex",
  ~flexDirection="column",
  ~width="500px",
  ()
);
let cellStyle = ReactDOMRe.Style.make(
  ~flex="1",
  ~textAlign="right",
  ()
);

let component = ReasonReact.reducerComponent("Home");

let make = (_children) => {
  let loadCoinsList = ({ReasonReact.state, reduce}) => {
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
      | Loading => ReasonReact.Update({...state, loading: true})
      | LoadedCoinList(data) => ReasonReact.Update({coins: data, loading: false})
      },
    didMount: (self) => {
      loadCoinsList(self);
      ReasonReact.NoUpdate;
    },
    render: (self) => {
      <div style=(containerStyle)>
        <div style=(
          ReactDOMRe.Style.make(
            ~display="flex",
            ~justifyContent="space-around",
            ~borderBottom="solid black 1px",
            ()
          )
        )>
          <div style=(cellStyle)>{ReasonReact.stringToElement("id")}</div>
          <div style=(cellStyle)>{ReasonReact.stringToElement("coinName")}</div>
          <div style=(cellStyle)>{ReasonReact.stringToElement("usd")}</div>
          <div style=(cellStyle)>{ReasonReact.stringToElement("eur")}</div>
          <div style=(cellStyle)>{ReasonReact.stringToElement("twd")}</div>
        </div>
        {
          self.state.coins
          |> Array.map((coinInfo: CoinData.coin) => <ListItem coinInfo />)
          |> ReasonReact.arrayToElement
        }
      </div>
    }
  }
};