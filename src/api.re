type action =
  | Loaded(array(CoinData.coin))
  | Loading;

type state = {
  coins: array(CoinData.coin),
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
    
    didMount: (_self) => {
      load(_self);
      ReasonReact.NoUpdate;
    },

    render: (_self) => {
      <div>
        <h1> (ReasonReact.stringToElement("api")) </h1>
      </div>
    }
  }
};