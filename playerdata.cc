// addon.cc
#include <node.h>
#include <node_buffer.h>

#include <nan.h>

#include "./playerdata.h"

namespace demo {

using namespace v8;
/*    class PLAYERDATA
    {
public:
        char id[64];
        char name[32];
        int  exp;
    };

    class ITEMDATA
    {
public:
         char id[64];
         int itemid;
         int num;
    };

   class LOGINDATA
    {
public:
        PLAYERDATA player;
        int  num_item;
        ITEMDATA items[1];
    };
*/

NAN_METHOD(Serialize)
{
  Isolate* isolate = Isolate::GetCurrent();

  //
  Local<String> key_id     = String::NewFromUtf8(isolate, "id");
  Local<String> key_name   = String::NewFromUtf8(isolate, "name");
  Local<String> key_exp    = String::NewFromUtf8(isolate, "exp");
  Local<String> key_itemid = String::NewFromUtf8(isolate, "itemid");
  Local<String> key_num    = String::NewFromUtf8(isolate, "num");

  Handle<Object> login  = Handle<Object>::Cast(info[0]);
  Handle<Object> player = Handle<Object>::Cast(login->Get(String::NewFromUtf8(isolate, "player")));
  Handle<Array> items   = Handle<Array>::Cast(login->Get(String::NewFromUtf8(isolate, "items")));

  size_t sizeof_buf = sizeof(LOGINDATA) + sizeof(ITEMDATA)*items->Length();
  char* buf = new char[sizeof_buf];
  LOGINDATA* logindata = (LOGINDATA*)buf;

  player->Get(key_id)->ToString()->WriteOneByte((uint8_t*)logindata->player.id);
  player->Get(key_name)->ToString()->WriteUtf8(logindata->player.name);
  logindata->player.exp = player->Get(key_exp)->Uint32Value();

  logindata->num_item = items->Length();
  for (unsigned int ii=0; ii<items->Length(); ii++) {
    Handle<Object> item = Handle<Object>::Cast(items->Get(ii));         //  5.753ms->11.993ms|6.24
    item->Get(key_id)->ToString()->WriteOneByte((uint8_t*)logindata->items[ii].id);  // 11.993ms->27.806ms|15.813.998
    logindata->items[ii].itemid = item->Get(key_itemid)->Uint32Value(); // 11.993ms->24.539ms|12.546
    logindata->items[ii].num    = item->Get(key_num)->Uint32Value();    // 24.539ms->35.388ms|10.849
  }

  info.GetReturnValue().Set(Nan::CopyBuffer((char*)buf, sizeof_buf).ToLocalChecked());
  delete[] buf;
}

NAN_METHOD(Deserialize)
{
  char* buf = node::Buffer::Data(info[0]->ToObject());
  LOGINDATA* logindata = (LOGINDATA*) buf;
  info.GetReturnValue().Set(Nan::New(logindata->player.name).ToLocalChecked());
}

NAN_MODULE_INIT(Init)
{
  NAN_EXPORT(target, Serialize);
  NAN_EXPORT(target, Deserialize);
}

NODE_MODULE(addon, Init)

}  // namespace demo

