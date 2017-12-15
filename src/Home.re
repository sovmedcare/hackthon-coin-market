open Utils;

type action =
  | LoadedCoinList(CoinData.coins)
  | LoadedObserveList(array(string))
  | ObserveListChange
  | ChangeFilter(string)
  | Loading;

type state = {
  coins: CoinData.coins,
  observeList: array(string),
  loading: bool,
  filter: string,
};

let containerStyle = ReactDOMRe.Style.make(
  ~margin="0 auto",
  ~display="flex",
  ~flexDirection="column",
  ~width="500px",
  ()
);

let listHeaderStyle = ReactDOMRe.Style.make(
  ~display="flex",
  ~justifyContent="space-around",
  ~borderBottom="solid black 1px",
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
      loading: false,
      filter: "All"
    },
    reducer: (action, state) =>
      switch action {
      | Loading => ReasonReact.Update({...state, loading: true})
      | LoadedCoinList(data) => ReasonReact.Update({...state, coins: data, loading: false})
      | LoadedObserveList(list) => ReasonReact.Update({...state, observeList: list})
      | ObserveListChange => ReasonReact.SideEffects((self) => {
        loadObseveList(self)
      })
      | ChangeFilter(filter) => ReasonReact.Update({...state, filter})
      },
    didMount: (self) => {
      loadCoinsList(self);
      loadObseveList(self);
      ReasonReact.NoUpdate;
    },
    render: (self) => {
      let { loading, coins, observeList, filter } = self.state;
      let visibleList = Array.fold_right(
        (coinInfo: CoinData.coin, acc) => {
          filter === "All"
            ? Array.append([|coinInfo|], acc)
            : unsafe_elem(coinInfo.name, observeList)
              ? Array.append([|coinInfo|], acc)
              : acc
        }, coins, [||]);
      <div style=(containerStyle)>
        <div style=(listHeaderStyle)>
          <div style=(cellStyle)>
            {
              filter == "All"
              ? <button onClick={self.reduce((_) => ChangeFilter("MyFavor"))}>{textEl("Show MyFavor")}</button>
              : <button onClick={self.reduce((_) => ChangeFilter("All"))}>{textEl("Show All")}</button>
            }
          </div>
          <div style=(cellStyle)>{textEl("coinName")}</div>
          <div style=(cellStyle)>{textEl("usd")}</div>
          <div style=(cellStyle)>{textEl("eur")}</div>
          <div style=(cellStyle)>{textEl("twd")}</div>
        </div>
        {
          loading
            ? <div>{textEl("Loading...")}</div>
            : ReasonReact.nullElement
        }
        {
          visibleList
          |> Array.map((coinInfo: CoinData.coin) =>
              <ListItem
                key=(coinInfo.id)
                coinInfo
                added=(unsafe_elem(coinInfo.name, observeList))
                onObseveListChange=(self.reduce((_) => ObserveListChange))
              />
            )
          |> arrayEl
        }
      </div>
    }
  }
};