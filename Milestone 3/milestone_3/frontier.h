struct Frontier{
  bool is_parent;
  uint16_t info;
  frontier north;
  frontier west;
  frontier east;
  frontier south;
};
