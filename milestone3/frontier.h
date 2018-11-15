
typedef struct frontier{
  bool is_parent;
  byte pos;
  byte dircost;
  frontier* front;
  frontier* left;
  frontier* right;
  frontier* back;
}frontier;

frontier* init_frontier(byte x,byte y, int dir){
  frontier* front = (frontier*) malloc(sizeof(frontier));
  front->is_parent = false;
  front->pos = x<<4 + y;
  front->dircost = byte(dir)<<6;
  front->front = NULL;
  front->left = NULL;
  front->right = NULL;
  front->back = NULL;
  return front;
}

void destroy_frontier(frontier* head){
  if(head->front != NULL){
    destroy_frontier(head->front);
  }
  if(head->left != NULL){
    destroy_frontier(head->left);
  }
  if(head->right != NULL){
    destroy_frontier(head->right);
  }
  if(head->back != NULL){
    destroy_frontier(head->back);
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
void add_frontier(frontier* head, int dir){
  head->is_parent = true;
  switch (dir){
    case 0: //front
      frontier* front = init_frontier(get_x(head),get_y(head),0);
      set_cost(front,get_cost(head) + 0);
      head->front = front;
    break;
    case 1: //left
    frontier* left = init_frontier(get_x(head),get_y(head),0);
      set_cost(left,get_cost(head) + 1);
      head->left = left;
    break;
    case 2: //right
    frontier* right = init_frontier(get_x(head),get_y(head),0);
      set_cost(right,get_cost(head) + 2);
      head->right = right;
    break;
    case 3: //back
    frontier* back = init_frontier(get_x(head),get_y(head),0);
      set_cost(back, get_cost(head) + 3);
      head->back = back;
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
    if(head->front!=NULL){
      if((get_x(head)==x)&&(get_y(head)==y-1)){
        return head->front;
      }
      else{
        find_frontier(head->front, get_x(head), get_y(head)+1);
      }
    }
    if(head->left!=NULL){
      if((get_x(head)==x+1)&&(get_y(head)==y)){
        return head->left;
      }
      else{
        find_frontier(head->left, get_x(head)-1, get_y(head));
      }
    }
    if(head->right!=NULL){
      if((get_x(head)==x-1)&&(get_y(head)==y)){
        return head->right;
      }
      else{
        find_frontier(head->right, get_x(head)+1, get_y(head));
      }
    }
    if(head->back!=NULL){
      if((get_x(head)==x)&&(get_y(head)==y+1)){
        return head->back;
      }
      else{
        find_frontier(head->back, get_x(head), get_y(head)-1);
      }
    }
  }
}
