var addon = require('./build/Release/addon');

var test = function(cnt, use_addon, use_minimaladdon) {

    // ログインデータサンプル
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
    // s = JSON.stringify(logindata);
    // console.log("[LEN]" + s.length);

    // [計測] 1000回実行して経過時間を測定
    console.time('loop');
    if (use_addon === "minimal") {
        for(var ii = 0; ii < 1000; ii++) {
            serialize(logindata);
        }
    } else if (use_addon === "full") {
        for(var ii = 0; ii < 1000; ii++) {
            addon.Serialize(logindata);
        }
    } else {
        for(var ii = 0; ii < 1000; ii++) {
	    JSON.stringify(logindata);
        }
    }
    console.timeEnd('loop');

};

var serialize = function(logindata) {
    var bufs = [];
    bufs.push(addon.SerializePlayerData(logindata.player.id, logindata.player.name, logindata.player.exp));
    var num_items = logindata.items.length;
    const buf = Buffer.allocUnsafe(4);
    buf.writeInt32LE(num_items);
    bufs.push(buf);
    for (var ii=0; ii<num_items; ii++) {
        bufs.push(addon.SerializeItemData(logindata.items[ii].id, logindata.items[ii].itemid, logindata.items[ii].num));
    }
    return Buffer.concat(bufs);
};


var sample = [100,200,300,400,500,600,700,800,900,1000];

console.log("##USE JSON.stringify##");
for (var ii=0; ii< sample.length; ii++) {
    test(sample[ii]);
}

console.log("##USE Native Addon##");
for (var ii=0; ii< sample.length; ii++) {
    test(sample[ii], "full");
}

console.log("##USE minimal Native Addon##");
for (var ii=0; ii< sample.length; ii++) {
    test(sample[ii], "minimal");
}

// EOF
