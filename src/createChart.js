const g2 = require('@antv/g2')

const data = [
  { year: '1991', value: 3 },
  { year: '1992', value: 4 },
  { year: '1993', value: 3.5 },
  { year: '1994', value: 5 },
  { year: '1995', value: 4.9 },
  { year: '1996', value: 6 },
  { year: '1997', value: 7 },
  { year: '1998', value: 9 },
  { year: '1999', value: 13 }
];
module.exports = function create() {
  const _chart = new g2.Chart({
    container: 'chart',
    forceFit: true,
    height: window.innerHeight
  });
  _chart.source(data);
  _chart.scale('value', {
    min: 0
  });
  _chart.scale('year', {
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