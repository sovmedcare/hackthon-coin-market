let baseUrl = "https://min-api.cryptocompare.com";
let coinListUrl = {j|$baseUrl/data/all/coinlist|j};
let historyUrl = {j|$baseUrl/data/histohour?fsym=BTC&tsym=USD&limit=60&aggregate=3&e=CCCAGG|j};

let transformCoinList: (Js.Json.t) => (Js.Json.t) = [%bs.raw
  {|
    function (json) {
      var defaultCoins = ['BTC', 'ETH', 'LTC'];      
      return defaultCoins.map(function(coin) {
        return json['Data'][coin]
      })
    }
  |}
];

let transformHistories: (Js.Json.t) => (Js.Json.t) = [%bs.raw
  {|
    function (json) {
      return json['Data']
    }
  |}
];

type coin = {
  id: string,
  url: string,
  imageUrl: string,
  name: string,
  coinName: string,
  fullName: string,
  algorithm: string,
  proofType: string,
  sortOrder: string
};

type coins = array(coin);

type history = {
  time: int,
  close: float,
  high: float,
  low: float,
  open_: float, /* open 為保留字 */
  volumefrom: float,
  volumeto: float
}; 

type histories = array(history);

module Decode = {
  let coin = (json) : coin =>
    Json.Decode.{
      id: json |> field("Id", string),
      url: json |> field("Url", string),
      imageUrl: json |> field("ImageUrl", string),
      name: json |> field("Name", string),
      coinName: json |> field("CoinName", string),
      fullName: json |> field("FullName", string),
      algorithm: json |> field("Algorithm", string),
      proofType: json |> field("ProofType", string),
      sortOrder: json |> field("SortOrder", string)
    };

  let coins = (json) : array(coin) =>
    Json.Decode.(json |> array(coin));

  let history = (json) : history =>
    Json.Decode.{
      time: json |> field("time", int),
      close: json |> field("close", float),
      high: json |> field("high", float),
      low: json |> field("low", float),
      open_: json |> field("open", float), 
      volumefrom: json |> field("volumefrom", float), 
      volumeto: json |> field("volumeto", float)
    };

  let histories = (json) : array(history) =>
    Json.Decode.(json |> array(history));
};

let fetchCoinList = (callback) => 
  Js.Promise.(
    Fetch.fetch(coinListUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformCoinList
            |> coins => {
              Js.log(coins);
              coins;
            }
            |> Decode.coins
            |> coins => {
              Js.log(coins);
              callback(coins);
              resolve(());
           }
      }
    )
    |> ignore
  );

let fetchHistoryByHour = (callback) =>
  Js.Promise.(
    Fetch.fetch(historyUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformHistories
            |> Decode.histories
            |> histories => {
              Js.log(histories);
              callback(histories);
              resolve(());
            }
      }
    )
    |> ignore

    /* 
    如果要用 fake data
    FakeData.history  |> Js.Json.parseExn
    |> Decode.histories
    |> histories => {
      Js.log(histories);
      callback(histories);
      resolve(());
    } */
  );