open Utils;

type action =
  | LoadedCoinList(CoinData.coins)
  | LoadedObserveList(array(string))
  | Loading;

type state = {
  coins: CoinData.coins,
  observeList: array(string),
  loading: bool
};

let containerStyle = ReactDOMRe.Style.make(
  ~margin="0 auto",
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

let unsafeGetKeysInLocalStorage: unit => array(string) = [%bs.raw
{|
function () {
  return Object.keys(localStorage)
}
|}
];

let component = ReasonReact.reducerComponent("Home");

let make = (_children) => {
  let loadCoinsList = ({ReasonReact.state, reduce}) => {
    CoinData.fetchCoinList(reduce(payload => LoadedCoinList(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  let loadObseveList = ({ReasonReact.state, reduce}) => {
    let items = unsafeGetKeysInLocalStorage();
    reduce(() => LoadedObserveList(items), ())
  };
  {
    ...component,
    initialState: () => {
      coins: [||],
      observeList: [||],
      loading: false
    },
    reducer: (action, state) =>
      switch action {
      | Loading => ReasonReact.Update({...state, loading: true})
      | LoadedCoinList(data) => ReasonReact.Update({...state, coins: data, loading: false})
      | LoadedObserveList(list) => ReasonReact.Update({...state, observeList: list})
      },
    didMount: (self) => {
      loadCoinsList(self);
      loadObseveList(self);
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
          <div style=(cellStyle)></div>
          <div style=(cellStyle)>{textEl("coinName")}</div>
          <div style=(cellStyle)>{textEl("usd")}</div>
          <div style=(cellStyle)>{textEl("eur")}</div>
          <div style=(cellStyle)>{textEl("twd")}</div>
        </div>
        {
          self.state.coins
          |> Array.map((coinInfo: CoinData.coin) =>
              <ListItem
                key=(coinInfo.id)
                coinInfo
                added=(
                  unsafe_elem(coinInfo.name, self.state.observeList)
                )
              />
            )
          |> ReasonReact.arrayToElement
        }
      </div>
    }
  }
};