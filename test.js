var addon = require('./build/Release/addon');

var test = function(cnt, use_addon) {

  var logindata = {
    player: {
      id: "550e8400-e29b-41d4-a716-446655440000",
      name: "test",
      exp: 1
    },
    items: []
  };
  console.log("[CNT]" + cnt);
  for(var ii = 0; ii < cnt; ii++) {
    logindata.items.push({id:"550e8400-e29b-41d4-a716-446655440000", itemid:ii, num: ii})
  }
  s = JSON.stringify(logindata);
  // console.log("[LEN]" + s.length);

  console.time('loop');
  for(var ii = 0; ii < 1000; ii++) {
    if (use_addon) {
      s = addon.Serialize(logindata);
    } else {
      JSON.stringify(logindata);
    }
  }
  console.timeEnd('loop');
};

var sample = [100,200,300,400,500,600,700,800,900,1000];
for (var ii=0; ii< sample.length; ii++) {
  test(sample[ii], false);
}

console.log("##USE ADDON##");

for (var ii=0; ii< sample.length; ii++) {
  test(sample[ii], true);
}

