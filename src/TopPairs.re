open Utils;

type state = {
  topPairs: CoinData.topPairs,
  loading: bool
};

type action =
  | Loaded(CoinData.topPairs)
  | Loading;

let component = ReasonReact.reducerComponent("TopPairs");

let make = (_children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchTopPairs(reduce(payload => Loaded(payload))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,
    initialState: () => {
      topPairs: [||],
      loading: false
    },
    reducer: (action, state) =>
    switch action {
    | Loading =>
      ReasonReact.Update({...state, loading: true})
  
    | Loaded(data) => {
      Js.log(data);
      ReasonReact.Update({
        topPairs: data,
        loading: false
      })
    }
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
        <div>{textEl("id")}</div>
        <div>{textEl("coinName")}</div>
      </div>
        {
          self.state.topPairs
          |> Array.map(
              (topPair: CoinData.topPair) => {
                <div style=(
                  ReactDOMRe.Style.make(
                    ~display="flex",
                    ~justifyContent="space-around",
                    ()
                  )
                )>
                  <div>{textEl(topPair.fromSymbol)}</div>
                  <div>{textEl(topPair.toSymbol)}</div>
                </div>
              }
            )
          |> arrayEl
        }
      </div>
    }
  }
};