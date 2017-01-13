
class Role
{
  public:
    char name[32];
    unsigned short tile_id;
    unsigned short pos_x;
    unsigned short pos_y;
    unsigned short hp, mp, sp;  /* sp is strength points */
};

class Player : public Role
{
}
