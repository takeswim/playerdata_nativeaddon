class PLAYERDATA
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
  int  itemid;
  int  num;
};

class LOGINDATA
{
 public:
  PLAYERDATA  player;
  int         num_item;  // itemsの長さ
  ITEMDATA    items[1];
};

