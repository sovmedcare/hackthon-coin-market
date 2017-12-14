open Utils;

type data = {
  year: string,
  value: float,
};

[@bs.module] external create: array(data) => unit = "./createChart";

type state = {
  data: array(data)
};

type action =
  | LoadedHistories(array(data));

let component = ReasonReact.reducerComponent("DetailPage");

let make = (~id, _children) => {
  let load = ({ReasonReact.state, reduce}) => {
    CoinData.fetchHistoryByHour(reduce(symbol => LoadedHistories(symbol))) |> ignore;
    reduce(() => Loading, ())
  };
  {
    ...component,
    initialState: () => {
      data: [|{ year: "1991", value: 3.0 },
      { year: "1992", value: 4.0 },
      { year: "1993", value: 3.5 },
      { year: "1994", value: 5.0},
      { year: "1995", value: 4.9 },
      { year: "1996", value: 6.0 },
      { year: "1997", value: 7.0 },
      { year: "1998", value: 9.0 },
      { year: "1999", value: 13.0 }|]
    },
    reducer: (action, state) =>
      switch action {
      | LoadedHistories(newData) => ReasonReact.Update({data: newData}) 
      },
    didMount: (_self) => {
      create(_self.state.data);
      ReasonReact.NoUpdate
    },
    render: (_self) => {
      <div>
        <h1> (textEl(id)) </h1>
        <div id="chart"></div>
      </div>
    }
  }
};