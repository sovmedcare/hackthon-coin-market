const g2 = require('@antv/g2')
const DataSet = require('@antv/data-set')
const moment = require('moment')
const R = require('ramda')



const create = (temp) => {
  if (document.querySelectorAll('#chart > div').length !== 0) {
    const chartDiv = document.getElementById('chart')
    chartDiv.innerHTML = ''
  }
  const chart = new g2.Chart({
    container: 'chart',
    width: '760',
    height: '700'
  })
  const ds = new DataSet()
  const dv = ds.createView()
  const data = R.map(R.zipObj(['time', 'close', 'high', 'low', 'open', 'volumefrom', 'volumeto']), temp)
  dv.source(data)
    .transform({
      type: 'map',
      callback: obj => {
        obj.trend = (obj.open <= obj.close) ? '上涨' : '下跌';
        obj.range = [ obj.open, obj.close, obj.high, obj.low ];
        obj.time = moment.unix(obj.time).toDate()
        return obj;
      }
    });
  chart.source(dv, {
    'time': {
      type: 'timeCat',
      formatter: (time) => moment(time).format('h:mm:ss'),
      tickCount: 20,
      range: [ 0, 1 ]
    },
    'trend': {
      values: [ '上涨', '下跌' ]
    },
    'volumnto': {alias: '成交量'},
    'open': {alias: '開盤價'},
    'close': {alias: '收盤價'},
    'high': {alias: '最高價'},
    'low': {alias: '最低價'}
  });
  
  const kView = chart.view({
    end: {
      x: 1,
      y: 0.5
    }
  });
  kView.source(dv)
  kView.schema()
    .position('time*range')
    .color('trend', val => {
      return (val === '上涨')
        ? '#f04864'
        : '#2fc25b'
    })
    .shape('candle')
    .tooltip('time*open*close*high*low', (time, open, close, high, low) => {
      return {
        name: moment(time).format('YYYY/MM/DD h:ss:mm'),
        value: '<br><span style="padding-left: 16px">開盤價：' + open + '</span><br/>'
        + '<span style="padding-left: 16px">收盤價：' + close + '</span><br/>'
        + '<span style="padding-left: 16px">最高價：' + high + '</span><br/>'
        + '<span style="padding-left: 16px">最低價：' + low + '</span>'
      };
    });
  
  chart.render();
  return null
}
module.exports = create