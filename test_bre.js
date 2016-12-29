const bre = require("bre");
const encoder = require("bre/lib/text-encoders/buffer");
bre.setTextEncoder(encoder);

const PLAYERDATA = bre.defineObjectRecord("PlayerData", [
    {type: "string(64)", name: "id"},
    {type: "string(32)", name: "name"},
    {type: "int32",      name: "exp"},
]);

const ITEMLENGTH = bre.defineObjectRecord("ItemLength", [
    {type: "int32",      name: "num"},
]);

const ITEMDATA = bre.defineObjectRecord("ItemData", [
    {type: "string(64)", name: "id"},
    {type: "int32",      name: "itemid"},
    {type: "int32",      name: "num"},
]);

var make_logindata = function(logindata) {
    const buf_player = Buffer.alloc(100);
    const player = PLAYERDATA.view(buf_player, 0);
    const buf_numitem = Buffer.alloc(4);
    const num_item = ITEMLENGTH.view(buf_numitem, 0);

    player.id = logindata.player.id;
    player.name = logindata.player.name;
    player.exp = logindata.player.exp;

    num_item.num = logindata.items.length;

    const buf_items = Buffer.alloc(logindata.items.length * 72);
    for(var ii=0; ii<logindata.items.length; ii++) {
        const item = ITEMDATA.view(buf_items, ii);
        item.id = logindata.items[ii].id;
        item.itemid = logindata.items[ii].itemid;
        item.num = logindata.items[ii].num;
    } 

    var bufs = [buf_player, buf_numitem, buf_items];
    return Buffer.concat(bufs);
};


var test = function(cnt, use_addon) {

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
    for(var ii = 0; ii < 1000; ii++) {
        make_logindata(logindata);
    }
    console.timeEnd('loop');

};

var sample = [100,200,300,400,500,600,700,800,900,1000];

for (var ii=0; ii< sample.length; ii++) {
    test(sample[ii], false);
}

// EOF
