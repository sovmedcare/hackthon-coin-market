const g2 = require('@antv/g2')

module.exports = function create(data) {
  console.log(data)
  const _chart = new g2.Chart({
    container: 'chart',
    forceFit: true,
    height: window.innerHeight
  });
  _chart.source(data);
  _chart.scale(1, {
    min: 0
  });
  _chart.scale(0, {
    range: [ 0 , 1 ]
  });
  _chart.tooltip({
    crosshairs: {
      type: 'line'
    }
  });
  _chart.line().position('year*value');
  _chart.point().position('year*value').size(4).shape('circle').style({
    stroke: '#fff',
    lineWidth: 1
  });
  _chart.render()
}