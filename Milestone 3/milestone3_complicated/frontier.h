/* 
 *  the frontier data structure is a 4-tree that contains the position in the maze,
 *  the best direction to go in at the point, 
*/
typedef struct frontier{
  bool is_parent;
  byte pos;
  byte dircost;
  bool done;
  frontier* head;
  frontier* north;
  frontier* south;
  frontier* east;
  frontier* west;
}frontier;

frontier* init_frontier(byte x,byte y, int dir, int cost){
  frontier* front = (frontier*) malloc(sizeof(frontier));
  front->is_parent = false;
  front->pos = x<<4 + y;
  front->done = false;
  front->dircost = byte(dir)<<6 + byte(cost);
  front->head = NULL;
  front->north = NULL;
  front->south = NULL;
  front->east  = NULL;
  front->west  = NULL;
  return front;
}

void destroy_frontier(frontier* head){
  if(head->north != NULL){
    destroy_frontier(head->north);
  }
  if(head->south != NULL){
    destroy_frontier(head->south);
  }
  if(head->east != NULL){
    destroy_frontier(head->east);
  }
  if(head->west != NULL){
    destroy_frontier(head->west);
  }
  free(head);
}
byte get_x(frontier* front){
  return byte(front->pos&&0xf0)>>4;
}

byte get_y(frontier* front){
  return byte(front->pos&&0xf);
}
 
byte get_cost(frontier* front){
  return (front->dircost)&&0x3f;
}
int get_dir(frontier* front){
  return int(front->dircost>>6);
}

frontier* get_head(frontier* front){
  return front->head;
}

bool get_done(frontier* front){
  return front->done;
}

void set_x(frontier* front,byte x){
  front->pos = (front->pos&&0xf)||(x<<4);  
}

void set_y(frontier* front,byte y){
  front->pos = (front->pos&&0xf0)||y;
}

void set_cost(frontier* front,byte cost){
  front->dircost = ((front->dircost)&&0xc0)||(cost&&0x3f);
}

void set_dir(frontier* front, int dir){
  front->dircost = ((front->dircost)&&0x3f)||(byte(dir<<6));
}

void set_head(frontier* front, frontier* head){
  front->head = head;
}

void set_done(frontier* front){
  front->done = true;
}
void add_frontier(frontier* head, int dir, bool wall, int cost){
  head->is_parent = true;
  switch (dir){
    case 0: //north
      frontier* north = init_frontier(get_x(head),get_y(head),0,0);
      if(wall){
        set_cost(north,0x3f);
      }
      else{
        set_cost(north,get_cost(head) + cost);
      }
      head->north = north;
      north->head = head;
    break;
    case 1: //left
    frontier* south = init_frontier(get_x(head),get_y(head),0,0);
      if(wall){
        set_cost(south,0x3f);
      }
      else{
        set_cost(south,get_cost(head) + cost);
      }
      south->head = head;
      head->south = south;
    break;
    case 2: //right
    frontier* east = init_frontier(get_x(head),get_y(head),0,0);
      if(wall){
        set_cost(east,0x3f);
      }
      else{
        set_cost(east,get_cost(head) + cost);
      }
      east->head = head;
      head->east = east;
      
    break;
    case 3: //back
    frontier* west = init_frontier(get_x(head),get_y(head),0,0);
      if(wall){
        set_cost(west,0x3f);
      }
      else{
        set_cost(west,get_cost(head) + cost);
      }
      west->head = head;
      head->west = west;
    break;
    default:
    break;
  }
}

frontier* find_frontier(frontier* head, byte x, byte y){
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
    if(head->south!=NULL){
      if((get_x(head)==x+1)&&(get_y(head)==y)){
        return head->south;
      }
      else{
        find_frontier(head->south, get_x(head)-1, get_y(head));
      }
    }
    if(head->east!=NULL){
      if((get_x(head)==x-1)&&(get_y(head)==y)){
        return head->east;
      }
      else{
        find_frontier(head->east, get_x(head)+1, get_y(head));
      }
    }
    if(head->west!=NULL){
      if((get_x(head)==x)&&(get_y(head)==y+1)){
        return head->west;
      }
      else{
        find_frontier(head->west, get_x(head), get_y(head)-1);
      }
    }
  }
}
