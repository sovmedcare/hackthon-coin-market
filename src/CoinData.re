let baseUrl = "https://min-api.cryptocompare.com";

let coinListUrl = {j|$baseUrl/data/all/coinlist|j};
let priceUrl = (coinName) => {j|$baseUrl/data/price?fsym=$coinName&tsyms=USD,EUR,TWD|j};
let historyHourUrl = {j|$baseUrl/data/histohour?fsym=BTC&tsym=USD&limit=60&aggregate=3&e=CCCAGG|j};
let historyDayUrl = {j|$baseUrl/data/histoday?fsym=BTC&tsym=USD&limit=60&aggregate=3&e=CCCAGG|j};
let topPairUrl = {j|$baseUrl/data/top/pairs?fsym=ETH|j};

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

let transformData: (Js.Json.t) => (Js.Json.t) = [%bs.raw
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

type price = {
  usd: float,
  eur: float,
  twd: float
};

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

type topPair = {
  exchange: string,
  fromSymbol: string,
  toSymbol: string,
  volume24h: float,
  volume24hTo: float
}; 

type topPairs = array(topPair);

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

  let price = (json) : price =>
    Json.Decode.{
      usd: json |> field("USD", float),
      eur: json |> field("EUR", float),
      twd: json |> field("TWD", float),
    };

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

  let topPair = (json) : topPair =>
    Json.Decode.{
      exchange: json |> field("exchange", string),
      fromSymbol: json |> field("fromSymbol", string),
      toSymbol: json |> field("toSymbol", string),
      volume24h: json |> field("volume24h", float),
      volume24hTo: json |> field("volume24hTo", float)
    };

  let topPairs = (json) : array(topPair) =>
    Json.Decode.(json |> array(topPair));

};

let fetchCoinList = (callback) => 
  Js.Promise.(
    Fetch.fetch(coinListUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformCoinList
            |> Decode.coins
            |> coins => {
              callback(coins);
              resolve(());
           }
      }
    )
    |> ignore
  );

let fetchPrice = (coinName, callback) =>
  {
    Js.Promise.(
      Fetch.fetch(priceUrl(coinName))
      |> then_(Fetch.Response.json)
      |> then_(json => {
        json  |> Decode.price
              |> price => {
                callback(price);
                resolve(());
              }
        }
      )
      |> ignore
    )

  };

let fetchHistoryByHour = (callback) =>
  Js.Promise.(
    Fetch.fetch(historyHourUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformData
            |> Decode.histories
            |> histories => {
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

let fetchHistoryByDay = (callback) =>
  Js.Promise.(
    Fetch.fetch(historyDayUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformData
            |> Decode.histories
            |> histories => {
              callback(histories);
              resolve(());
            }
      }
    )
    |> ignore
  );

let fetchTopPairs = (callback) =>
  Js.Promise.(
    Fetch.fetch(topPairUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      json  |> transformData
            |> Decode.topPairs
            |> topPairs => {
              callback(topPairs);
              resolve(());
            }
      }
    )
    |> ignore
  );