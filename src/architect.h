/* 
 * File:   architect.h
 * Author: angle
 *
 * Created on February 25, 2015, 2:58 PM
 */

#ifndef ARCHITECT_H
#define	ARCHITECT_H

#include "overmap.h"
#include "mapgen.h"
#include <vector>
//This class is for generating coherent multi-tile structures, such as 
//labs, malls, mansions, hospitals, prisons, etc.

struct room_detail {
    room_type type;
    int x, y, width, height;
    
};

void science_room(map *m, int x1, int y1, int x2, int y2, int z, int rotate);
void set_science_room(map *m, int x1, int y1, bool faces_right, int turn);
void silo_rooms(map *m);
void build_mine_room(map *m, room_type type, int x1, int y1, int x2, int y2, mapgendata & dat);

room_type pick_mansion_room(int x1, int y1, int x2, int y2);
void build_mansion_room(map *m, room_type type, int x1, int y1, int x2, int y2, mapgendata & dat);
void mansion_room(map *m, int x1, int y1, int x2, int y2, mapgendata & dat); // pick & build

class architect {
    std::vector<complex_map_tile*> tiles;
    
    std::vector<room_detail> rooms;
    
    
};

#endif	/* ARCHITECT_H */

