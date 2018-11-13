
typedef struct frontier_t{
  bool is_parent;
  byte pos;
  byte dircost;
  frontier* north;
  frontier* west;
  frontier* east;
  frontier* south;
}frontier;

frontier* init_frontier(x,y,dir1,dir0){
  front = (frontier*) malloc(sizeof(frontier));
  front->is_parent = false;
  front->pos = x<<4 + y;
  front->dircost = byte(2*dir1+dir0)<<6;
  front->north = NULL;
  front->west = NULL;
  front->east = NULL;
  front->south = NULL;
  return front;
}

void add_frontier(frontier* front, x,y,dir1,dir0){
  if
}

byte get_x(frontier* front){
  return byte(front->pos&&0xf0)>>4;
}

byte get_y(frontier* front){
  return byte(front->pos&&0xf);
}
 
byte get_dircost(frontier* front){
  return front->dircost;
}

void set_x(frontier* front,x){
  front->pos = (front->pos&&0xf)||(x<<4);  
}

void set_y(frontier* front,y){
  front->pos = (front->pos&&0xf0)||y;
}

void set_cost(frontier* front,cost){
  front->dircost = ((front->dircost)&&0xc0)||(cost&&0x3f);
}

frontier* find_frontier(frontier* head, x, y){
  if(!head->is_parent){
    return NULL;
  }
  else{
    if(head->north!=NULL){
      if((get_x(head)==x)&&(get_y(head)==y-1)){
        return head->north;
      }
      else{
        find_frontier(head->north, get_x(head), get_y(head)+1);
      }
    }
    if(head->west!=NULL){
      if((get_x(head)==x+1)&&(get_y(head)==y)){
        return head->north;
      }
      else{
        find_frontier(head->west, get_x(head)-1, get_y(head));
      }
    }
    if(head->east!=NULL){
      if((get_x(head)==x-1)&&(get_y(head)==y)){
        return head->north;
      }
      else{
        find_frontier(head->east, get_x(head)+1, get_y(head));
      }
    }
    if(head->south!=NULL){
      if((get_x(head)==x)&&(get_y(head)==y+1)){
        return head->south;
      }
      else{
        find_frontier(head->south, get_x(head), get_y(head)-1);
      }
    }
  }
}

