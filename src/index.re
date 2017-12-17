[%bs.raw {|require('./index.css')|}];

[@bs.module "./registerServiceWorker"] external register_service_worker : unit => unit = "default";

register_service_worker();

let renderForRoute = (route) =>
  ReactDOMRe.renderToElementWithId(<App route />, "root");

let router =
  DirectorRe.makeRouter({
    "/": () => renderForRoute(Routing.Home),
    "/detail/:id": (id: string) => renderForRoute(Routing.Detail(id)),
    "/topPairs": () => renderForRoute(Routing.TopPairs)
  });

DirectorRe.init(router, "/");

