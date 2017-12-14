let baseUrl = "https://min-api.cryptocompare.com";
let coinListUrl = {j|$baseUrl/data/all/coinlist|j};

let transformAPI: (Js.Json.t) => (Js.Json.t) = [%bs.raw
  {|
    function (json) {
      var data = json['Data']
      return Object.values(data)
    }
  |}
];

let coinListFakeData = {| [
  {
    "Id": "1182",
    "Url": "/coins/btc/overview",
    "ImageUrl": "/media/19633/btc.png",
    "Name": "BTC",
    "Symbol": "BTC",
    "CoinName": "Bitcoin",
    "FullName": "Bitcoin (BTC)",
    "Algorithm": "SHA256",
    "ProofType": "PoW",
    "FullyPremined": "0",
    "TotalCoinSupply": "21000000",
    "PreMinedValue": "N/A",
    "TotalCoinsFreeFloat": "N/A",
    "SortOrder": "1",
    "Sponsored": false
  },
  {
    "Id": "7605",
    "Url": "/coins/eth/overview",
    "ImageUrl": "/media/20646/eth_logo.png",
    "Name": "ETH",
    "Symbol": "ETH",
    "CoinName": "Ethereum",
    "FullName": "Ethereum (ETH)",
    "Algorithm": "Ethash",
    "ProofType": "PoW",
    "FullyPremined": "0",
    "TotalCoinSupply": "0",
    "PreMinedValue": "N/A",
    "TotalCoinsFreeFloat": "N/A",
    "SortOrder": "2",
    "Sponsored": false
  },
  {
    "Id": "7605",
    "Url": "/coins/eth/overview",
    "ImageUrl": "/media/20646/eth_logo.png",
    "Name": "ETH",
    "Symbol": "ETH",
    "CoinName": "Ethereum",
    "FullName": "Ethereum (ETH)",
    "Algorithm": "Ethash",
    "ProofType": "PoW",
    "FullyPremined": "0",
    "TotalCoinSupply": "0",
    "PreMinedValue": "N/A",
    "TotalCoinsFreeFloat": "N/A",
    "SortOrder": "2",
    "Sponsored": false
  },
  {
    "Id": "3808",
    "Url": "/coins/ltc/overview",
    "ImageUrl": "/media/19782/litecoin-logo.png",
    "Name": "LTC",
    "Symbol": "LTC",
    "CoinName": "Litecoin",
    "FullName": "Litecoin (LTC)",
    "Algorithm": "Scrypt",
    "ProofType": "PoW",
    "FullyPremined": "0",
    "TotalCoinSupply": "84000000",
    "PreMinedValue": "N/A",
    "TotalCoinsFreeFloat": "N/A",
    "SortOrder": "3",
    "Sponsored": false
  }
] |};

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

module Decode = {
  let parseCoinListAPI = (json) : Js.Json.t => {
    Js.log(transformAPI(json));
    json;
  };

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
};

let fetchCoinList = (callback) => 
  Js.Promise.(
    Fetch.fetch(coinListUrl)
    |> then_(Fetch.Response.json)
    |> then_(json => {
      coinListFakeData  |> Js.Json.parseExn
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