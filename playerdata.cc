#include <node.h>
#include <node_buffer.h>

#include <nan.h>

#include "./playerdata.h"

namespace demo {

  using namespace v8;

  NAN_METHOD(Serialize)
  {
    Isolate* isolate = Isolate::GetCurrent();

    // Getで使うkeyを事前にとっておく(使用頻度高いので)
    Local<String> key_id     = String::NewFromUtf8(isolate, "id");
    Local<String> key_name   = String::NewFromUtf8(isolate, "name");
    Local<String> key_exp    = String::NewFromUtf8(isolate, "exp");
    Local<String> key_itemid = String::NewFromUtf8(isolate, "itemid");
    Local<String> key_num    = String::NewFromUtf8(isolate, "num");

    Handle<Object> login  = Handle<Object>::Cast(info[0]);
    Handle<Object> player = Handle<Object>::Cast(login->Get(String::NewFromUtf8(isolate, "player")));
    Handle<Array>  items  = Handle<Array>::Cast(login->Get(String::NewFromUtf8(isolate, "items")));

    // バッファ確保して出力先ポインタにする
    size_t sizeof_buf = sizeof(LOGINDATA) + sizeof(ITEMDATA)*items->Length();
    char* buf = new char[sizeof_buf];
    LOGINDATA* logindata = (LOGINDATA*)buf;

    // playerの各値をセット
    player->Get(key_id)->ToString()->WriteOneByte((uint8_t*)logindata->player.id);
    player->Get(key_name)->ToString()->WriteUtf8(logindata->player.name);
    logindata->player.exp = player->Get(key_exp)->Uint32Value();

    // itemsの各値をセット
    logindata->num_item = items->Length();
    for (unsigned int ii=0; ii<items->Length(); ii++) {
      Handle<Object> item = Handle<Object>::Cast(items->Get(ii));         //  5.753ms->11.993ms|6.24ms
      item->Get(key_id)->ToString()->WriteOneByte((uint8_t*)logindata->items[ii].id);  // 11.993ms->27.806ms|15.813.998ms
      logindata->items[ii].itemid = item->Get(key_itemid)->Uint32Value(); // 11.993ms->24.539ms|12.546ms
      logindata->items[ii].num    = item->Get(key_num)->Uint32Value();    // 24.539ms->35.388ms|10.849ms
    }

    info.GetReturnValue().Set(Nan::CopyBuffer((char*)buf, sizeof_buf).ToLocalChecked());
    delete[] buf;
  }

  NAN_METHOD(SerializePlayerData)
  {
    PLAYERDATA playerdata;
    // playerの各値をセット
    info[0]->ToString()->WriteOneByte((uint8_t*)playerdata.id);
    info[1]->ToString()->WriteUtf8(playerdata.name);
    playerdata.exp = info[2]->Uint32Value();

    info.GetReturnValue().Set(Nan::CopyBuffer((char*)&playerdata, sizeof(playerdata)).ToLocalChecked());
  }

  NAN_METHOD(SerializeItemData)
  {
    ITEMDATA item;
    // itemの各値をセット
    info[0]->ToString()->WriteOneByte((uint8_t*)item.id);
    item.itemid = info[1]->Uint32Value();
    item.num    = info[2]->Uint32Value();

    info.GetReturnValue().Set(Nan::CopyBuffer((char*)&info, sizeof(info)).ToLocalChecked());
  }

  // FIXME: 時間足りないので対応しない
  NAN_METHOD(Deserialize)
  {
    char* buf = node::Buffer::Data(info[0]->ToObject());
    LOGINDATA* logindata = (LOGINDATA*) buf;
    info.GetReturnValue().Set(Nan::New(logindata->player.name).ToLocalChecked());
  }

  NAN_MODULE_INIT(Init)
  {
    NAN_EXPORT(target, Serialize);
    NAN_EXPORT(target, SerializePlayerData);
    NAN_EXPORT(target, SerializeItemData);
    NAN_EXPORT(target, Deserialize);
  }

  NODE_MODULE(addon, Init)
  
}  // namespace demo

