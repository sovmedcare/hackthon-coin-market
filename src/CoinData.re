let baseUrl = "https://min-api.cryptocompare.com";
let coinListUrl = {j|$baseUrl/data/all/coinlist|j};

/* "Data": {
  "LTC": {
    "Id": "3808",
    "Url": "/coins/ltc/overview",
    "ImageUrl": "/media/19782/ltc.png",
    "Name": "LTC",
    "CoinName": "Litecoin",
    "FullName": "Litecoin (LTC)",
    "Algorithm": "Scrypt",
    "ProofType": "PoW",
    "SortOrder": "2"
  }
    ...
}, */

type coin = {
  id: int,
  url: string,
  imageUrl: string,
  name: string,
  coinName: string,
  fullName: string,
  algorithm: string,
  proofType: string,
  sortOrder: int
};

module Decode = {
  let parseCoinListAPI = (json) : Js.Json.t => {
    Js.log(json);
    /* Json.Decode.{
      data: json |> field("DATA", string)
    }; */
    json
  };

  let coins = (json) : coin =>
    Json.Decode.{
      id: json |> field("Id", int),
      url: json |> field("Url", string),
      imageUrl: json |> field("ImageUrl", string),
      name: json |> field("Name", string),
      coinName: json |> field("CoinName", string),
      fullName: json |> field("FullName", string),
      algorithm: json |> field("Algorithm", string),
      proofType: json |> field("ProofType", string),
      sortOrder: json |> field("SortOrder", int)
    };
};

let fetchCoinList = (callback) => 
  Js.Promise.(
    Fetch.fetch(coinListUrl)
    |> then_ (Fetch.Response.json)
    |> then_ (json => {
      json  |> Decode.parseCoinListAPI
            |> json => { 
              Js.log(json); 
              json 
            }
            |> Decode.coins
            |> coins => {
              callback([||]);
              resolve(());
            }
      }
    )
    |> ignore
  );