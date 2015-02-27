#include "mapgen.h"
#include "architect.h"

room_type pick_mansion_room(int x1, int y1, int x2, int y2)
{
    int dx = abs(x1 - x2), dy = abs(y1 - y2), area = dx * dy;
    int shortest = (dx < dy ? dx : dy), longest = (dx > dy ? dx : dy);
    std::vector<room_type> valid;
    if (shortest >= 12) {
        valid.push_back(room_mansion_courtyard);
    }
    if (shortest >= 7 && area >= 64 && area <= 100) {
        valid.push_back(room_mansion_bedroom);
    }
    if (shortest >= 9) {
        valid.push_back(room_mansion_library);
    }
    if (shortest >= 6 && longest <= 10) {
        valid.push_back(room_mansion_kitchen);
    }
    if (longest >= 7 && shortest >= 5) {
        valid.push_back(room_mansion_dining);
    }
    if (shortest >= 6 && longest <= 10) {
        valid.push_back(room_mansion_game);
    }
    if (shortest >= 6 && longest <= 10) {
        valid.push_back(room_mansion_study);
    }
    if (shortest >= 10) {
        valid.push_back(room_mansion_pool);
    }
    if (longest <= 6 || shortest <= 4) {
        valid.push_back(room_mansion_bathroom);
    }
    if (longest >= 8 && shortest <= 6) {
        valid.push_back(room_mansion_gallery);
    }

    if (valid.empty()) {
        debugmsg("x: %d - %d, dx: %d\n\
       y: %d - %d, dy: %d", x1, x2, dx,
                 y1, y2, dy);
        return room_null;
    }

    return valid[ rng(0, valid.size() - 1) ];
}

void build_mansion_room(map *m, room_type type, int x1, int y1, int x2, int y2, mapgendata & dat)
{
    int dx = abs(x1 - x2), dy = abs(y1 - y2);
    int cx_low = (x1 + x2) / 2, cx_hi = (x1 + x2 + 1) / 2,
        cy_low = (y1 + y2) / 2, cy_hi = (y1 + y2 + 1) / 2;

    /*
     debugmsg("\
    x: %d - %d, dx: %d cx: %d/%d\n\
    x: %d - %d, dx: %d cx: %d/%d", x1, x2, dx, cx_low, cx_hi,
                                   y1, y2, dy, cy_low, cy_hi);
    */
    bool walled_south = (y2 >= SEEY * 2 - 2);

    switch (type) {

    case room_mansion_courtyard:
        dat.square_groundcover( x1, y1, x2, y2);
        if (one_in(4)) { // Tree grid
            for (int x = 1; x <= dx / 2; x += 4) {
                for (int y = 1; y <= dx / 2; y += 4) {
                    m->ter_set(x1 + x, y1 + y, t_tree);
                    m->ter_set(x2 - x, y2 - y + 1, t_tree);
                }
            }
        }
        if (one_in(3)) { // shrub-lined
            for (int i = x1; i <= x2; i++) {
                if (m->ter(i, y2 + 1) != t_door_c) {
                    m->ter_set(i, y2, t_shrub);
                }
            }
            if (walled_south && x1 <= SEEX && SEEX <= x2) {
                m->ter_set(SEEX - 1, y2, dat.groundcover());
                m->ter_set(SEEX,     y2, dat.groundcover());
            }
        }
        break;

    case room_mansion_entry:
        if (!one_in(3)) { // Columns
            for (int y = y1 + 2; y <= y2; y += 3) {
                m->ter_set(cx_low - 3, y, t_column);
                m->ter_set(cx_low + 4, y, t_column);
            }
        }
        if (one_in(6)) { // Suits of armor
            int start = y1 + rng(2, 4), end = y2 - rng(0, 4), step = rng(3, 6);
            if (!one_in(4)) { // 75% for Euro ornamental, but good weapons maybe
            for (int y = start; y <= end; y += step) {
                m->spawn_item(x1 + 1, y, "helmet_plate");
                m->spawn_item(x1 + 1, y, "armor_plate");
                if (one_in(2)) {
                    m->spawn_item(x1 + 1, y, "pike");
                } else if (one_in(3)) {
                    m->spawn_item(x1 + 1, y, "broadsword");
                    m->spawn_item(x1 + 1, y, "scabbard");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "mace");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "morningstar");
                }
                m->spawn_item(x2 - 1, y, "helmet_plate");
                m->spawn_item(x2 - 1, y, "armor_plate");
                if (one_in(2)) {
                    m->spawn_item(x2 - 1, y, "pike");
                } else if (one_in(3)) {
                    m->spawn_item(x2 - 1, y, "broadsword");
                    m->spawn_item(x2 - 1, y, "scabbard");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "mace");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "morningstar");
                }
            }
          } else if (one_in(3)) { // Then 8.25% each for useful plate
              for (int y = start; y <= end; y += step) {
                m->spawn_item(x1 + 1, y, "helmet_barbute");
                m->spawn_item(x1 + 1, y, "armor_lightplate");
                if (one_in(2)) {
                    m->spawn_item(x1 + 1, y, "mace");
                } else if (one_in(3)) {
                    m->spawn_item(x1 + 1, y, "morningstar");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "battleaxe");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "broadsword");
                    m->spawn_item(x1 + 1, y, "scabbard");
                }
                m->spawn_item(x2 - 1, y, "helmet_barbute");
                m->spawn_item(x2 - 1, y, "armor_lightplate");
                if (one_in(2)) {
                    m->spawn_item(x2 - 1, y, "mace");
                } else if (one_in(3)) {
                    m->spawn_item(x2 - 1, y, "morningstar");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "battleaxe");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "broadsword");
                    m->spawn_item(x2 - 1, y, "scabbard");
                }
            }
          } else if (one_in(2)) { // or chainmail
              for (int y = start; y <= end; y += step) {
              // No helmets with the chainmail, sorry.
                m->spawn_item(x1 + 1, y, "chainmail_suit");
                if (one_in(2)) {
                    m->spawn_item(x1 + 1, y, "mace");
                } else if (one_in(3)) {
                    m->spawn_item(x1 + 1, y, "pike");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "battleaxe");
                } else if (one_in(6)) {
                    m->spawn_item(x1 + 1, y, "broadsword");
                    m->spawn_item(x1 + 1, y, "scabbard");
                }
                m->spawn_item(x2 - 1, y, "chainmail_suit");
                if (one_in(2)) {
                    m->spawn_item(x2 - 1, y, "mace");
                } else if (one_in(3)) {
                    m->spawn_item(x2 - 1, y, "pike");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "battleaxe");
                } else if (one_in(6)) {
                    m->spawn_item(x2 - 1, y, "broadsword");
                    m->spawn_item(x2 - 1, y, "scabbard");
                }
            }
          } else { // or samurai gear
                for (int y = start; y <= end; y += step) {
                    m->spawn_item(x1 + 1, y, "helmet_kabuto");
                    m->spawn_item(x1 + 1, y, "armor_samurai");
                    if (one_in(2)) {
                        m->spawn_item(x1 + 1, y, "katana");
                    } else if (one_in(3)) {
                        m->spawn_item(x1 + 1, y, "katana");
                        m->spawn_item(x1 + 1, y, "wakizashi");
                    } else if (one_in(6)) {
                        m->spawn_item(x1 + 1, y, "katana");
                        m->spawn_item(x1 + 1, y, "wakizashi");
                        m->spawn_item(x1 + 1, y, "tanto");
                    } else if (one_in(6)) {
                        m->spawn_item(x1 + 1, y, "nodachi");
                    } else if (one_in(4)) {
                        m->spawn_item(x1 + 1, y, "bokken");
                    }

                    m->spawn_item(x2 - 1, y, "helmet_kabuto");
                    m->spawn_item(x2 - 1, y, "armor_samurai");
                    if (one_in(2)) {
                        m->spawn_item(x2 - 1, y, "katana");
                    } else if (one_in(3)) {
                        m->spawn_item(x2 - 1, y, "katana");
                        m->spawn_item(x1 + 1, y, "wakizashi");
                    } else if (one_in(6)) {
                        m->spawn_item(x2 - 1, y, "katana");
                        m->spawn_item(x1 + 1, y, "wakizashi");
                        m->spawn_item(x1 + 1, y, "tanto");
                    } else if (one_in(6)) {
                        m->spawn_item(x2 - 1, y, "nodachi");
                    } else if (one_in(4)) {
                        m->spawn_item(x2 - 1, y, "bokken");
                    }

                    if(one_in(2)) {
                        m->spawn_item(x2 - 1, y, "scabbard");
                    } if (one_in(2)) {
                        m->spawn_item(x1 + 1, y, "scabbard");
                    }
            }
          }
        }
        break;

    case room_mansion_bedroom:
        if (dx > dy || (dx == dy && one_in(2))) { // horizontal
            if (one_in(2)) { // bed on left
                square_furn(m, f_bed, x1 + 1, cy_low - 1, x1 + 3, cy_low + 1);
                m->place_items("bed", 60, x1 + 1, cy_low - 1, x1 + 3, cy_low + 1, false, 0);
            } else { // bed on right
                square_furn(m, f_bed, x2 - 3, cy_low - 1, x2 - 1, cy_low + 1);
                m->place_items("bed", 60, x2 - 3, cy_low - 1, x2 - 1, cy_low + 1, false, 0);
            }
            m->furn_set(cx_hi - 2, y1, f_bookcase);
            m->furn_set(cx_hi - 1, y1, f_counter);
            m->ter_set(cx_hi    , y1, t_console_broken);
            m->furn_set(cx_hi + 1, y1, f_counter);
            m->furn_set(cx_hi + 2, y1, f_bookcase);
            m->place_items("bedroom", 60, cx_hi - 2, y1, cx_hi + 2, y1, false, 0);

            m->furn_set(cx_hi - 2, y2, f_dresser);
            m->furn_set(cx_hi - 1, y2, f_dresser);
            m->place_items("dresser", 80, cx_hi - 2, y2, cx_hi - 1, y2, false, 0);
            if (one_in(10)) {
                m->place_items("mansion_guns", 58, cx_hi - 2, y2, cx_hi - 1, y2, false, 0);
            }

            m->furn_set(cx_hi + 1, y2, f_desk);
            m->place_items("office", 50, cx_hi + 1, y2, cx_hi + 1, y2, false, 0);

            m->furn_set(cx_hi + 2, y2, f_chair);

            m->furn_set(x1, y1, f_indoor_plant);
            m->furn_set(x1, y2, f_indoor_plant);

        } else { // vertical
            if (one_in(2)) { // bed at top
                square_furn(m, f_bed, cx_low - 1, y1 + 1, cx_low + 1, y1 + 3);
                m->place_items("bed", 60, cx_low - 1, y1 + 1, cx_low + 1, y1 + 3, false, 0);
            } else { // bed at bottom
                square_furn(m, f_bed, cx_low - 1, y2 - 3, cx_low + 1, y2 - 1);
                m->place_items("bed", 60, cx_low - 1, y2 - 3, cx_low + 1, y2 - 1, false, 0);
            }
            m->furn_set(x1, cy_hi - 2, f_bookcase);
            m->furn_set(x1, cy_hi - 1, f_counter);
            m->ter_set(x1, cy_hi, t_console_broken);
            m->furn_set(x1, cy_hi + 1, f_counter);
            m->furn_set(x1, cy_hi + 2, f_bookcase);
            m->place_items("bedroom", 80, x1, cy_hi - 2, x1, cy_hi + 2, false, 0);

            m->furn_set(x2, cy_hi - 2, f_dresser);
            m->furn_set(x2, cy_hi - 1, f_dresser);
            m->place_items("dresser", 80, x2, cy_hi - 2, x2, cy_hi - 1, false, 0);
            if (one_in(10)) {
                m->place_items("mansion_guns", 58, x2, cy_hi - 2, x2, cy_hi - 1, false, 0);
            }

            m->furn_set(x2, cy_hi + 1, f_desk);
            m->place_items("office", 50, x2, cy_hi + 1, x2, cy_hi + 1, false, 0);

            m->furn_set(x2, cy_hi + 2, f_chair);

            m->furn_set(x1, y2, f_indoor_plant);
            m->furn_set(x2, y2, f_indoor_plant);
        }
        break;

    case room_mansion_library:
        if (dx < dy || (dx == dy && one_in(2))) { // vertically-aligned bookshelves
            for (int x = x1 + 1; x <= cx_low - 2; x += 3) {
                for (int y = y1 + 1; y <= y2 - 3; y += 4) {
                    square_furn(m, f_bookcase, x, y, x + 1, y + 2);
                    m->place_items("novels",    85, x, y, x + 1, y + 2, false, 0);
                    m->place_items("manuals",   62, x, y, x + 1, y + 2, false, 0);
                    m->place_items("textbooks", 40, x, y, x + 1, y + 2, false, 0);
                    m->place_items("mansion_books", 35, x, y, x + 1, y + 2, false, 0);
                }
            }
            for (int x = x2 - 1; x >= cx_low + 2; x -= 3) {
                for (int y = y1 + 1; y <= y2 - 3; y += 4) {
                    square_furn(m, f_bookcase, x - 1, y, x, y + 2);
                    m->place_items("novels",    85, x - 1, y, x, y + 2, false, 0);
                    m->place_items("manuals",   62, x - 1, y, x, y + 2, false, 0);
                    m->place_items("textbooks", 40, x - 1, y, x, y + 2, false, 0);
                    m->place_items("mansion_books", 35, x - 1, y, x, y + 2, false, 0);
                }
            }
        } else { // horizontally-aligned bookshelves
            for (int y = y1 + 1; y <= cy_low - 2; y += 3) {
                for (int x = x1 + 1; x <= x2 - 3; x += 4) {
                    square_furn(m, f_bookcase, x, y, x + 2, y + 1);
                    m->place_items("novels",    85, x, y, x + 2, y + 1, false, 0);
                    m->place_items("manuals",   62, x, y, x + 2, y + 1, false, 0);
                    m->place_items("textbooks", 40, x, y, x + 2, y + 1, false, 0);
                    m->place_items("mansion_books", 35, x, y, x + 2, y + 1, false, 0);
                }
            }
            for (int y = y2 - 1; y >= cy_low + 2; y -= 3) {
                for (int x = x1 + 1; x <= x2 - 3; x += 4) {
                    square_furn(m, f_bookcase, x, y - 1, x + 2, y);
                    m->place_items("novels",    85, x, y - 1, x + 2, y, false, 0);
                    m->place_items("manuals",   62, x, y - 1, x + 2, y, false, 0);
                    m->place_items("textbooks", 40, x, y - 1, x + 2, y, false, 0);
                    m->place_items("mansion_books", 35, x, y - 1, x + 2, y, false, 0);
                }
            }
        }
        break;

    case room_mansion_kitchen:
        line_furn(m, f_counter, cx_hi - 2, y1 + 1, cx_hi - 2, y2 - 1);
        line_furn(m, f_counter, cx_hi,     y1 + 1, cx_hi,     y2 - 1);
        m->place_items("kitchen",  60, cx_hi - 2, y1 + 1, cx_hi, y2 - 1, false, 0);

        line_furn(m, f_fridge, cx_hi + 2, y1 + 1, cx_hi + 2, cy_hi - 1);
        m->place_items("fridge",  80, cx_hi + 2, y1 + 1, cx_hi + 2, cy_hi - 1, false, 0);

        m->furn_set(cx_hi + 2, cy_hi, f_oven);
        m->place_items("oven", 70,  cx_hi + 2, cy_hi, cx_hi + 2, cy_hi, false, 0);

        line_furn(m, f_rack, cx_hi + 2, cy_hi + 1, cx_hi + 2, y2 - 1);
        m->place_items("cannedfood",  70, cx_hi + 2, cy_hi + 1, cx_hi + 2, y2 - 1, false, 0);
        m->place_items("pasta",  70, cx_hi + 2, cy_hi + 1, cx_hi + 2, y2 - 1, false, 0);
        break;

    case room_mansion_dining:
        if (dx < dy || (dx == dy && one_in(2))) { // vertically-aligned table
            line_furn(m, f_table, cx_low, y1 + 2, cx_low, y2 - 2);
            line_furn(m, f_bench, cx_low - 1, y1 + 2, cx_low - 1, y2 - 2);
            line_furn(m, f_bench, cx_low + 1, y1 + 2, cx_low + 1, y2 - 2);
            m->place_items("dining", 78, cx_low, y1 + 2, cx_low, y2 - 2, false, 0);
        } else { // horizontally-aligned table
            line_furn(m, f_table, x1 + 2, cy_low, x2 - 2, cy_low);
            line_furn(m, f_bench, x1 + 2, cy_low - 1, x2 - 2, cy_low - 1);
            line_furn(m, f_bench, x1 + 2, cy_low + 1, x2 - 2, cy_low + 1);
            m->place_items("dining", 78, x1 + 2, cy_low, x2 - 2, cy_low, false, 0);
        }
        m->furn_set(x1, y1, f_indoor_plant);
        m->furn_set(x2, y1, f_indoor_plant);
        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);
        break;

    case room_mansion_game:
        if (dx < dy || one_in(2)) { // vertically-aligned table
            square_furn(m, f_pool_table, cx_low, cy_low - 1, cx_low + 1, cy_low + 1);
            m->place_items("pool_table", 80, cx_low, cy_low - 1, cx_low + 1, cy_low + 1,
                           false, 0);
        } else { // horizontally-aligned table
            square_furn(m, f_pool_table, cx_low - 1, cy_low, cx_low + 1, cy_low + 1);
            m->place_items("pool_table", 80, cx_low - 1, cy_low, cx_low + 1, cy_low + 1,
                           false, 0);
        }

        if (one_in(2)) {
            line_furn(m, f_sofa, x1 + 1, cy_low - 1, x1 + 1, cy_low + 1);
            m->furn_set(x1 + 1, cy_low - 2, f_table);
            m->place_items("coffee_shop", 70, x1 + 1, cy_low + 2, x1 + 1, cy_low + 2, false, 0);
            m->place_items("magazines", 50, x1 + 1, cy_low + 2, x1 + 1, cy_low + 2, false, 0);
            m->furn_set(x1 + 1, cy_low + 2, f_table);
            m->place_items("coffee_shop", 70, x1 + 1, cy_low - 2, x1 + 1, cy_low - 2, false, 0);
            m->place_items("magazines", 70, x1 + 1, cy_low - 2, x1 + 1, cy_low - 2, false, 0);
            m->place_items("mansion_books", 30, x1 + 1, cy_low - 2, x1 + 1, cy_low - 2, false, 0);
        } else {
            line_furn(m, f_sofa, cx_low - 1, y1 + 1, cx_low + 1, y1 + 1);
            m->furn_set(cx_low - 2, y1 + 1, f_table);
            m->place_items("coffee_shop", 70, cx_low - 2, y1 + 1, cx_low - 2, y1 + 1, false, 0);
            m->place_items("magazines", 50, cx_low - 2, y1 + 1, cx_low - 2, y1 + 1, false, 0);
            m->furn_set(cx_low + 2, y1 + 1, f_table);
            m->place_items("coffee_shop", 70, cx_low + 2, y1 + 1, cx_low + 2, y1 + 1, false, 0);
            m->place_items("magazines", 70, cx_low + 2, y1 + 1, cx_low + 2, y1 + 1, false, 0);
            m->place_items("mansion_books", 30, cx_low + 2, y1 + 1, cx_low + 2, y1 + 1, false, 0);
        }
        m->furn_set(x1, y1, f_indoor_plant);
        m->furn_set(x2, y1, f_indoor_plant);
        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);
        break;

    case room_mansion_pool:
        square(m, t_water_pool, x1 + 3, y1 + 3, x2 - 3, y2 - 3);

        m->furn_set(rng(x1 + 1, cx_hi - 2), y1 + 2, f_chair);
        m->furn_set(cx_hi, y1 + 2, f_table);
        m->furn_set(rng(x1 + 1, cx_hi + 2), y1 + 2, f_chair);
        m->place_items("magazines", 60, cx_hi, y1 + 2, cx_hi, y1 + 2, false, 0);

        m->furn_set(x1, y1, f_indoor_plant);
        m->furn_set(x2, y1, f_indoor_plant);
        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);
        break;

    case room_mansion_study:
        int study_y;
        if (one_in(2)) {
            study_y = y1;
        } else {
            study_y = y2;
        }
        for (int x = x1 + 1; x <= x2 - 1; x++) {
            if (x % 2 == 0) {
                m->furn_set(x, study_y, f_rack);
                if (one_in(3)) {
                    m->place_items("alcohol", 60, x, study_y, x, study_y, false, 0);
                } else if (one_in(3)) {
                    m->place_items("church", 60, x, study_y, x, study_y, false, 0);
                } else if (one_in(2)) {
                    m->place_items("mansion_guns", 60, x, study_y, x, study_y, false, 0);
                } else {
                    m->place_items("art", 60, x, study_y, x, study_y, false, 0);
                }
            }
        }

        square_furn(m, f_table, cx_low, cy_low - 1, cx_low + 1, cy_low + 1);
        m->place_items("novels", 50, cx_low, cy_low - 1, cx_low + 1, cy_low + 1,
                       false, 0);
        m->place_items("mansion_books", 40, cx_low, cy_low - 1, cx_low + 1, cy_low + 1,
                       false, 0);
        m->place_items("magazines", 60, cx_low, cy_low - 1, cx_low + 1, cy_low + 1,
                       false, 0);
        m->place_items("office", 60, cx_low, cy_low - 1, cx_low + 1, cy_low + 1,
                       false, 0);
        if (one_in(2)) {
            m->furn_set(cx_low - 1, rng(cy_low - 1, cy_low + 1), f_chair);
        } else {
            m->furn_set(cx_low + 2, rng(cy_low - 1, cy_low + 1), f_chair);
        }
        m->furn_set(x1, y1, f_indoor_plant);
        m->furn_set(x2, y1, f_indoor_plant);
        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);
        break;

    case room_mansion_bathroom:
        m->place_toilet(rng(x1 + 1, cx_hi - 1), rng(y1 + 1, cy_hi - 1));
        m->furn_set( rng(cx_hi + 1, x2 - 1), rng(y1 + 1, cy_hi - 1) , f_bathtub);
        m->furn_set( rng(x1 + 1, cx_hi - 1), rng(cy_hi + 1, y2 - 1) , f_sink);

        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);

        m->place_items("harddrugs", 20, x1 + 1, y1 + 1, x2 - 1, y2 - 1, false, 0);
        m->place_items("softdrugs", 72, x1 + 1, y1 + 1, x2 - 1, y2 - 1, false, 0);
        m->place_items("cleaning",  48, x1 + 1, y1 + 1, x2 - 1, y2 - 1, false, 0);
        break;

    case room_mansion_gallery:

        m->furn_set(x2 + 2, y2 + 2, f_rack);
        if (one_in(3)) {
            m->place_items("mansion_guns", 70, x2 + 2, y2 - 2, x2 + 2, y2 - 2, false, 0);
      } else {
        m->place_items("medieval", 40, x2 + 2, y2 + 2, x2 + 2, y2 + 2, false, 0);
      }
        m->furn_set(x2 - 2, y2 + 2, f_rack);
        m->place_items("art", 70, x2 - 2, y2 + 2, x2 - 2, y2 + 2, false, 0);
        m->furn_set(x2 + 2, y2 - 2, f_rack);
        if (one_in(3)) {
            m->place_items("mansion_guns", 70, x2 + 2, y2 - 2, x2 + 2, y2 - 2, false, 0);
      } else {
            m->place_items("art", 70, x2 + 2, y2 - 2, x2 + 2, y2 - 2, false, 0);
      }
        m->furn_set(x2 - 2, y2 - 2, f_rack);
        m->place_items("alcohol", 80, x2 - 2, y2 - 2, x2 - 2, y2 - 2, false, 0);

        square_furn(m, f_table, cx_low - 1, cy_low - 1, cx_low + 1, cy_low + 1);
        m->furn_set(x1, y1, f_indoor_plant);
        m->furn_set(x2, y1, f_indoor_plant);
        m->furn_set(x1, y2, f_indoor_plant);
        m->furn_set(x2, y2, f_indoor_plant);

        break;
    default:
        break;
    }
}

void mansion_room(map *m, int x1, int y1, int x2, int y2, mapgendata & dat)
{
    room_type type = pick_mansion_room(x1, y1, x2, y2);
    build_mansion_room(m, type, x1, y1, x2, y2, dat);
}

void science_room(map *m, int x1, int y1, int x2, int y2, int z, int rotate)
{
    int height = y2 - y1;
    int width  = x2 - x1;
    if (rotate % 2 == 1) { // Swap width & height if we're a lateral room
        int tmp = height;
        height  = width;
        width   = tmp;
    }
    for (int i = x1; i <= x2; i++) {
        for (int j = y1; j <= y2; j++) {
            m->ter_set(i, j, t_rock_floor);
        }
    }
    int area = height * width;
    std::vector<room_type> valid_rooms;
    if (height < 5 && width < 5) {
        valid_rooms.push_back(room_closet);
    }
    if (height > 6 && width > 3) {
        valid_rooms.push_back(room_lobby);
    }
    if (height > 4 || width > 4) {
        valid_rooms.push_back(room_chemistry);
    }
    if ( z != 0 && (height > 7 || width > 7) && height > 2 && width > 2) {
        valid_rooms.push_back(room_teleport);
    }
    if (height > 4 && width > 4) {
        valid_rooms.push_back(room_goo);
    }
    if (height > 7 && width > 7) {
        valid_rooms.push_back(room_bionics);
    }
    if (height > 7 && width > 7) {
        valid_rooms.push_back(room_cloning);
    }
    if (area >= 9) {
        valid_rooms.push_back(room_vivisect);
    }
    if (height > 5 && width > 4) {
        valid_rooms.push_back(room_dorm);
    }
    if (width > 8) {
        for (int i = 8; i < width; i += rng(1, 2)) {
            valid_rooms.push_back(room_split);
        }
    }

    room_type chosen = valid_rooms[rng(0, valid_rooms.size() - 1)];
    int trapx = rng(x1 + 1, x2 - 1);
    int trapy = rng(y1 + 1, y2 - 1);
    switch (chosen) {
    case room_closet:
        m->place_items("cleaning", 80, x1, y1, x2, y2, false, 0);
        break;
    case room_lobby:
        if (rotate % 2 == 0) { // Vertical
            int desk = y1 + rng(int(height / 2) - int(height / 4), int(height / 2) + 1);
            for (int x = x1 + int(width / 4); x < x2 - int(width / 4); x++) {
                m->furn_set(x, desk, f_counter);
            }
            computer *tmpcomp = m->add_computer(x2 - int(width / 4), desk,
                                                _("Log Console"), 3);
            tmpcomp->add_option(_("View Research Logs"), COMPACT_RESEARCH, 0);
            tmpcomp->add_option(_("Download Map Data"), COMPACT_MAPS, 0);
            tmpcomp->add_failure(COMPFAIL_SHUTDOWN);
            tmpcomp->add_failure(COMPFAIL_ALARM);
            tmpcomp->add_failure(COMPFAIL_DAMAGE);
            m->add_spawn("mon_turret", 1, int((x1 + x2) / 2), desk);
        } else {
            int desk = x1 + rng(int(height / 2) - int(height / 4), int(height / 2) + 1);
            for (int y = y1 + int(width / 4); y < y2 - int(width / 4); y++) {
                m->furn_set(desk, y, f_counter);
            }
            computer *tmpcomp = m->add_computer(desk, y2 - int(width / 4),
                                                _("Log Console"), 3);
            tmpcomp->add_option(_("View Research Logs"), COMPACT_RESEARCH, 0);
            tmpcomp->add_option(_("Download Map Data"), COMPACT_MAPS, 0);
            tmpcomp->add_failure(COMPFAIL_SHUTDOWN);
            tmpcomp->add_failure(COMPFAIL_ALARM);
            tmpcomp->add_failure(COMPFAIL_DAMAGE);
            m->add_spawn("mon_turret", 1, desk, int((y1 + y2) / 2));
        }
        break;
    case room_chemistry:
        if (rotate % 2 == 0) { // Vertical
            for (int x = x1; x <= x2; x++) {
                if (x % 3 == 0) {
                    for (int y = y1 + 1; y <= y2 - 1; y++) {
                        m->furn_set(x, y, f_counter);
                    }
                    if (one_in(3)) {
                        m->place_items("mut_lab", 35, x, y1 + 1, x, y2 - 1, false, 0);
                    }
                    else {
                        m->place_items("chem_lab", 70, x, y1 + 1, x, y2 - 1, false, 0);
                    }
                }
            }
        } else {
            for (int y = y1; y <= y2; y++) {
                if (y % 3 == 0) {
                    for (int x = x1 + 1; x <= x2 - 1; x++) {
                        m->furn_set(x, y, f_counter);
                    }
                    if (one_in(3)) {
                        m->place_items("mut_lab", 35, x1 + 1, y, x2 - 1, y, false, 0);
                    }
                    else {
                        m->place_items("chem_lab", 70, x1 + 1, y, x2 - 1, y, false, 0);
                    }
                }
            }
        }
        break;
    case room_teleport:
        m->furn_set(int((x1 + x2) / 2)    , int((y1 + y2) / 2)    , f_counter);
        m->furn_set(int((x1 + x2) / 2) + 1, int((y1 + y2) / 2)    , f_counter);
        m->furn_set(int((x1 + x2) / 2)    , int((y1 + y2) / 2) + 1, f_counter);
        m->furn_set(int((x1 + x2) / 2) + 1, int((y1 + y2) / 2) + 1, f_counter);
        m->add_trap(trapx, trapy, tr_telepad);
        m->place_items("teleport", 70, int((x1 + x2) / 2),
                       int((y1 + y2) / 2), int((x1 + x2) / 2) + 1,
                       int((y1 + y2) / 2) + 1, false, 0);
        break;
    case room_goo:
        do {
            m->add_trap(trapx, trapy, tr_goo);
            trapx = rng(x1 + 1, x2 - 1);
            trapy = rng(y1 + 1, y2 - 1);
        } while(!one_in(5));
        if (rotate == 0) {
            m->remove_trap(x1, y2);
            m->furn_set(x1, y2, f_fridge);
            m->place_items("goo", 60, x1, y2, x1, y2, false, 0);
        } else if (rotate == 1) {
            m->remove_trap(x1, y1);
            m->furn_set(x1, y1, f_fridge);
            m->place_items("goo", 60, x1, y1, x1, y1, false, 0);
        } else if (rotate == 2) {
            m->remove_trap(x2, y1);
            m->furn_set(x2, y1, f_fridge);
            m->place_items("goo", 60, x2, y1, x2, y1, false, 0);
        } else {
            m->remove_trap(x2, y2);
            m->furn_set(x2, y2, f_fridge);
            m->place_items("goo", 60, x2, y2, x2, y2, false, 0);
        }
        break;
    case room_cloning:
        for (int x = x1 + 1; x <= x2 - 1; x++) {
            for (int y = y1 + 1; y <= y2 - 1; y++) {
                if (x % 3 == 0 && y % 3 == 0) {
                    m->ter_set(x, y, t_vat);
                    m->place_items("cloning_vat", 20, x, y, x, y, false, 0);
                }
            }
        }
        break;
    case room_vivisect:
        if        (rotate == 0) {
            for (int x = x1; x <= x2; x++) {
                m->furn_set(x, y2 - 1, f_counter);
            }
            m->place_items("dissection", 80, x1, y2 - 1, x2, y2 - 1, false, 0);
        } else if (rotate == 1) {
            for (int y = y1; y <= y2; y++) {
                m->furn_set(x1 + 1, y, f_counter);
            }
            m->place_items("dissection", 80, x1 + 1, y1, x1 + 1, y2, false, 0);
        } else if (rotate == 2) {
            for (int x = x1; x <= x2; x++) {
                m->furn_set(x, y1 + 1, f_counter);
            }
            m->place_items("dissection", 80, x1, y1 + 1, x2, y1 + 1, false, 0);
        } else if (rotate == 3) {
            for (int y = y1; y <= y2; y++) {
                m->furn_set(x2 - 1, y, f_counter);
            }
            m->place_items("dissection", 80, x2 - 1, y1, x2 - 1, y2, false, 0);
        }
        m->add_trap(int((x1 + x2) / 2), int((y1 + y2) / 2), tr_dissector);
        if (one_in(10)) {
            m->add_spawn("mon_broken_cyborg", 1, int(((x1 + x2) / 2)+1), int(((y1 + y2) / 2)+1));
        }
        break;

    case room_bionics:
        if (rotate % 2 == 0) {
            int biox = x1 + 2, bioy = int((y1 + y2) / 2);
            mapf::formatted_set_simple(m, biox - 1, bioy - 1,
                                       "\
---\n\
|c=\n\
---\n",
                                       mapf::basic_bind("- | =", t_wall_h, t_wall_v, t_reinforced_glass_v),
                                       mapf::basic_bind("c", f_counter));
            m->place_items("bionics_common", 70, biox, bioy, biox, bioy, false, 0);

            m->ter_set(biox, bioy+2, t_console);
            computer *tmpcomp = m->add_computer(biox, bioy+2, _("Bionic access"), 2);
            tmpcomp->add_option(_("Manifest"), COMPACT_LIST_BIONICS, 0);
            tmpcomp->add_option(_("Open Chambers"), COMPACT_RELEASE_BIONICS, 3);
            tmpcomp->add_failure(COMPFAIL_MANHACKS);
            tmpcomp->add_failure(COMPFAIL_SECUBOTS);

            biox = x2 - 2;
            mapf::formatted_set_simple(m, biox - 1, bioy - 1,
                                       "\
---\n\
=c|\n\
---\n",
                                       mapf::basic_bind("- | =", t_wall_h, t_wall_v, t_reinforced_glass_v),
                                       mapf::basic_bind("c", f_counter));
            m->place_items("bionics_common", 70, biox, bioy, biox, bioy, false, 0);

            m->ter_set(biox, bioy-2, t_console);
            computer *tmpcomp2 = m->add_computer(biox, bioy-2, _("Bionic access"), 2);
            tmpcomp2->add_option(_("Manifest"), COMPACT_LIST_BIONICS, 0);
            tmpcomp2->add_option(_("Open Chambers"), COMPACT_RELEASE_BIONICS, 3);
            tmpcomp2->add_failure(COMPFAIL_MANHACKS);
            tmpcomp2->add_failure(COMPFAIL_SECUBOTS);
        } else {
            int bioy = y1 + 2, biox = int((x1 + x2) / 2);
            mapf::formatted_set_simple(m, biox - 1, bioy - 1,
                                       "\
|-|\n\
|c|\n\
|=|\n",
                                       mapf::basic_bind("- | =", t_wall_h, t_wall_v, t_reinforced_glass_h),
                                       mapf::basic_bind("c", f_counter));
            m->place_items("bionics_common", 70, biox, bioy, biox, bioy, false, 0);

            m->ter_set(biox+2, bioy, t_console);
            computer *tmpcomp = m->add_computer(biox+2, bioy, _("Bionic access"), 2);
            tmpcomp->add_option(_("Manifest"), COMPACT_LIST_BIONICS, 0);
            tmpcomp->add_option(_("Open Chambers"), COMPACT_RELEASE_BIONICS, 3);
            tmpcomp->add_failure(COMPFAIL_MANHACKS);
            tmpcomp->add_failure(COMPFAIL_SECUBOTS);

            bioy = y2 - 2;
            mapf::formatted_set_simple(m, biox - 1, bioy - 1,
                                       "\
|=|\n\
|c|\n\
|-|\n",
                                       mapf::basic_bind("- | =", t_wall_h, t_wall_v, t_reinforced_glass_h),
                                       mapf::basic_bind("c", f_counter));
            m->place_items("bionics_common", 70, biox, bioy, biox, bioy, false, 0);

            m->ter_set(biox-2, bioy, t_console);
            computer *tmpcomp2 = m->add_computer(biox-2, bioy, _("Bionic access"), 2);
            tmpcomp2->add_option(_("Manifest"), COMPACT_LIST_BIONICS, 0);
            tmpcomp2->add_option(_("Open Chambers"), COMPACT_RELEASE_BIONICS, 3);
            tmpcomp2->add_failure(COMPFAIL_MANHACKS);
            tmpcomp2->add_failure(COMPFAIL_SECUBOTS);
        }
        break;
    case room_dorm:
        if (rotate % 2 == 0) {
            for (int y = y1 + 1; y <= y2 - 1; y += 3) {
                m->furn_set(x1    , y, f_bed);
                m->place_items("bed", 60, x1, y, x1, y, false, 0);
                m->furn_set(x1 + 1, y, f_bed);
                m->place_items("bed", 60, x1 + 1, y, x1 + 1, y, false, 0);
                m->furn_set(x2    , y, f_bed);
                m->place_items("bed", 60, x2, y, x2, y, false, 0);
                m->furn_set(x2 - 1, y, f_bed);
                m->place_items("bed", 60, x2 - 1, y, x2 - 1, y, false, 0);
                m->furn_set(x1, y + 1, f_dresser);
                m->furn_set(x2, y + 1, f_dresser);
                m->place_items("dresser", 70, x1, y + 1, x1, y + 1, false, 0);
                m->place_items("dresser", 70, x2, y + 1, x2, y + 1, false, 0);
            }
        } else if (rotate % 2 == 1) {
            for (int x = x1 + 1; x <= x2 - 1; x += 3) {
                m->furn_set(x, y1    , f_bed);
                m->place_items("bed", 60, x, y1, x, y1, false, 0);
                m->furn_set(x, y1 + 1, f_bed);
                m->place_items("bed", 60, x, y1 + 1, x, y1 + 1, false, 0);
                m->furn_set(x, y2    , f_bed);
                m->place_items("bed", 60, x, y2, x, y2, false, 0);
                m->furn_set(x, y2 - 1, f_bed);
                m->place_items("bed", 60, x, y2 - 1, x, y2 - 1, false, 0);
                m->furn_set(x + 1, y1, f_dresser);
                m->furn_set(x + 1, y2, f_dresser);
                m->place_items("dresser", 70, x + 1, y1, x + 1, y1, false, 0);
                m->place_items("dresser", 70, x + 1, y2, x + 1, y2, false, 0);
            }
        }
        m->place_items("lab_dorm", 84, x1, y1, x2, y2, false, 0);
        break;
    case room_split:
        if (rotate % 2 == 0) {
            int w1 = int((x1 + x2) / 2) - 2, w2 = int((x1 + x2) / 2) + 2;
            for (int y = y1; y <= y2; y++) {
                m->ter_set(w1, y, t_wall_v);
                m->ter_set(w2, y, t_wall_v);
            }
            m->ter_set(w1, int((y1 + y2) / 2), t_door_metal_c);
            m->ter_set(w2, int((y1 + y2) / 2), t_door_metal_c);
            science_room(m, x1, y1, w1 - 1, y2, z, 1);
            science_room(m, w2 + 1, y1, x2, y2, z, 3);
        } else {
            int w1 = int((y1 + y2) / 2) - 2, w2 = int((y1 + y2) / 2) + 2;
            for (int x = x1; x <= x2; x++) {
                m->ter_set(x, w1, t_wall_h);
                m->ter_set(x, w2, t_wall_h);
            }
            m->ter_set(int((x1 + x2) / 2), w1, t_door_metal_c);
            m->ter_set(int((x1 + x2) / 2), w2, t_door_metal_c);
            science_room(m, x1, y1, x2, w1 - 1, z, 2);
            science_room(m, x1, w2 + 1, x2, y2, z, 0);
        }
        break;
    default:
        break;
    }
}

void set_science_room(map *m, int x1, int y1, bool faces_right, int turn)
{
    // TODO: More types!
    int type = rng(0, 4);
    int x2 = x1 + 7;
    int y2 = y1 + 4;
    switch (type) {
    case 0: // Empty!
        return;
    case 1: // Chemistry.
        // #######.
        // #.......
        // #.......
        // #.......
        // #######.
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                if ((i == x1 || j == y1 || j == y2) && i != x1) {
                    m->set(i, j, t_floor, f_counter);
                }
            }
        }
        m->place_items("chem_lab", 85, x1 + 1, y1, x2 - 1, y1, false, 0);
        m->place_items("chem_lab", 85, x1 + 1, y2, x2 - 1, y2, false, 0);
        m->place_items("chem_lab", 85, x1, y1 + 1, x1, y2 - 1, false, 0);
        break;

    case 2: // Hydroponics.
        // #.......
        // #.~~~~~.
        // #.......
        // #.~~~~~.
        // #.......
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                if (i == x1) {
                    m->set(i, j, t_floor, f_counter);
                } else if (i > x1 + 1 && i < x2 && (j == y1 + 1 || j == y2 - 1)) {
                    m->ter_set(i, j, t_water_sh);
                }
            }
        }
        m->place_items("chem_lab", 80, x1, y1, x1, y2, false, turn - 50);
        m->place_items("hydro", 92, x1 + 1, y1 + 1, x2 - 1, y1 + 1, false, turn);
        m->place_items("hydro", 92, x1 + 1, y2 - 1, x2 - 1, y2 - 1, false, turn);
        break;

    case 3: // Electronics.
        // #######.
        // #.......
        // #.......
        // #.......
        // #######.
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                if ((i == x1 || j == y1 || j == y2) && i != x1) {
                    m->set(i, j, t_floor, f_counter);
                }
            }
        }
        m->place_items("electronics", 85, x1 + 1, y1, x2 - 1, y1, false, turn - 50);
        m->place_items("electronics", 85, x1 + 1, y2, x2 - 1, y2, false, turn - 50);
        m->place_items("electronics", 85, x1, y1 + 1, x1, y2 - 1, false, turn - 50);
        break;

    case 4: // Monster research.
        // .|.####.
        // -|......
        // .|......
        // -|......
        // .|.####.
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                if (i == x1 + 1) {
                    m->ter_set(i, j, t_wall_glass_v);
                } else if (i == x1 && (j == y1 + 1 || j == y2 - 1)) {
                    m->ter_set(i, j, t_wall_glass_h);
                } else if ((j == y1 || j == y2) && i >= x1 + 3 && i <= x2 - 1) {
                    m->set(i, j, t_floor, f_counter);
                }
            }
        }
        // TODO: Place a monster in the sealed areas.
        m->place_items("monparts", 70, x1 + 3, y1, 2 - 1, y1, false, turn - 100);
        m->place_items("monparts", 70, x1 + 3, y2, 2 - 1, y2, false, turn - 100);
        break;
    }

    if (!faces_right) { // Flip it.
        ter_id rotated[SEEX * 2][SEEY * 2];
        std::vector<item> itrot[SEEX * 2][SEEY * 2];
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                rotated[i][j] = m->ter(i, j);
                auto items = m->i_at( i, j );
                itrot[i][j].reserve( items.size() );
                std::copy( items.begin(), items.end(), std::back_inserter(itrot[i][j]) );
                m->i_clear( i, j );
            }
        }
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                m->ter_set(i, j, rotated[x2 - (i - x1)][j]);
                m->spawn_items(i, j, itrot[x2 - (i - x1)][j]);
            }
        }
    }
}

void silo_rooms(map *m)
{
    std::vector<point> rooms;
    std::vector<point> room_sizes;
    bool okay = true;
    do {
        int x, y, height, width;
        if (one_in(2)) { // True = top/bottom, False = left/right
            x = rng(0, SEEX * 2 - 6);
            y = rng(0, 4);
            if (one_in(2)) {
                y = SEEY * 2 - 2 - y;    // Bottom of the screen, not the top
            }
            width  = rng(2, 5);
            height = 2;
            if (x + width >= SEEX * 2 - 1) {
                width = SEEX * 2 - 2 - x;    // Make sure our room isn't too wide
            }
        } else {
            x = rng(0, 4);
            y = rng(0, SEEY * 2 - 6);
            if (one_in(2)) {
                x = SEEX * 2 - 3 - x;    // Right side of the screen, not the left
            }
            width  = 2;
            height = rng(2, 5);
            if (y + height >= SEEY * 2 - 1) {
                height = SEEY * 2 - 2 - y;    // Make sure our room isn't too tall
            }
        }
        if (!rooms.empty() && // We need at least one room!
            (m->ter(x, y) != t_rock || m->ter(x + width, y + height) != t_rock)) {
            okay = false;
        } else {
            rooms.push_back(point(x, y));
            room_sizes.push_back(point(width, height));
            for (int i = x; i <= x + width; i++) {
                for (int j = y; j <= y + height; j++) {
                    if (m->ter(i, j) == t_rock) {
                        m->ter_set(i, j, t_floor);
                    }
                }
            }
            items_location used1 = "none", used2 = "none";
            switch (rng(1, 14)) { // What type of items go here?
            case  1:
            case  2:
                used1 = "cannedfood";
                used2 = "fridge";
                break;
            case  3:
            case  4:
                used1 = "tools";
                break;
            case  5:
            case  6:
                used1 = "allguns";
                used2 = "ammo";
                break;
            case  7:
                used1 = "allclothes";
                break;
            case  8:
                used1 = "manuals";
                break;
            case  9:
            case 10:
            case 11:
                used1 = "electronics";
                break;
            case 12:
                used1 = "survival_tools";
                break;
            case 13:
            case 14:
                used1 = "radio";
                break;
            }
            if (used1 != "none") {
                m->place_items(used1, 78, x, y, x + width, y + height, false, 0);
            }
            if (used2 != "none") {
                m->place_items(used2, 64, x, y, x + width, y + height, false, 0);
            }
        }
    } while (okay);

    m->ter_set(rooms[0].x, rooms[0].y, t_stairs_up);
    int down_room = rng(0, rooms.size() - 1);
    point dp = rooms[down_room], ds = room_sizes[down_room];
    m->ter_set(dp.x + ds.x, dp.y + ds.y, t_stairs_down);
    rooms.push_back(point(SEEX, SEEY)); // So the center circle gets connected
    room_sizes.push_back(point(5, 5));

    while (rooms.size() > 1) {
        int best_dist = 999, closest = 0;
        for (size_t i = 1; i < rooms.size(); i++) {
            int dist = trig_dist(rooms[0].x, rooms[0].y, rooms[i].x, rooms[i].y);
            if (dist < best_dist) {
                best_dist = dist;
                closest = i;
            }
        }
        // We chose the closest room; now draw a corridor there
        point origin = rooms[0], origsize = room_sizes[0], dest = rooms[closest];
        int x = origin.x + origsize.x, y = origin.y + origsize.y;
        bool x_first = (abs(origin.x - dest.x) > abs(origin.y - dest.y));
        while (x != dest.x || y != dest.y) {
            if (m->ter(x, y) == t_rock) {
                m->ter_set(x, y, t_floor);
            }
            if ((x_first && x != dest.x) || (!x_first && y == dest.y)) {
                if (dest.x < x) {
                    x--;
                } else {
                    x++;
                }
            } else {
                if (dest.y < y) {
                    y--;
                } else {
                    y++;
                }
            }
        }
        rooms.erase(rooms.begin());
        room_sizes.erase(room_sizes.begin());
    }
}

void build_mine_room(map *m, room_type type, int x1, int y1, int x2, int y2, mapgendata & dat)
{
    (void)dat;
    direction door_side;
    std::vector<direction> possibilities;
    int midx = int( (x1 + x2) / 2), midy = int( (y1 + y2) / 2);
    if (x2 < SEEX) {
        possibilities.push_back(EAST);
    }
    if (x1 > SEEX + 1) {
        possibilities.push_back(WEST);
    }
    if (y1 > SEEY + 1) {
        possibilities.push_back(NORTH);
    }
    if (y2 < SEEY) {
        possibilities.push_back(SOUTH);
    }

    if (possibilities.empty()) { // We're in the middle of the map!
        if (midx <= SEEX) {
            possibilities.push_back(EAST);
        } else {
            possibilities.push_back(WEST);
        }
        if (midy <= SEEY) {
            possibilities.push_back(SOUTH);
        } else {
            possibilities.push_back(NORTH);
        }
    }

    door_side = possibilities[rng(0, possibilities.size() - 1)];
    point door_point;
    switch (door_side) {
    case NORTH:
        door_point.x = midx;
        door_point.y = y1;
        break;
    case EAST:
        door_point.x = x2;
        door_point.y = midy;
        break;
    case SOUTH:
        door_point.x = midx;
        door_point.y = y2;
        break;
    case WEST:
        door_point.x = x1;
        door_point.y = midy;
        break;
    default:
        break;
    }
    square(m, t_floor, x1, y1, x2, y2);
    line(m, t_wall_h, x1, y1, x2, y1);
    line(m, t_wall_h, x1, y2, x2, y2);
    line(m, t_wall_v, x1, y1 + 1, x1, y2 - 1);
    line(m, t_wall_v, x2, y1 + 1, x2, y2 - 1);
    // Main build switch!
    switch (type) {
    case room_mine_shaft: {
        m->ter_set(x1 + 1, y1 + 1, t_console);
        line(m, t_wall_h, x2 - 2, y1 + 2, x2 - 1, y1 + 2);
        m->ter_set(x2 - 2, y1 + 1, t_elevator);
        m->ter_set(x2 - 1, y1 + 1, t_elevator_control_off);
        computer *tmpcomp = m->add_computer(x1 + 1, y1 + 1, _("NEPowerOS"), 2);
        tmpcomp->add_option(_("Divert power to elevator"), COMPACT_ELEVATOR_ON, 0);
        tmpcomp->add_failure(COMPFAIL_ALARM);
    }
    break;

    case room_mine_office:
        line_furn(m, f_counter, midx, y1 + 2, midx, y2 - 2);
        line(m, t_window, midx - 1, y1, midx + 1, y1);
        line(m, t_window, midx - 1, y2, midx + 1, y2);
        line(m, t_window, x1, midy - 1, x1, midy + 1);
        line(m, t_window, x2, midy - 1, x2, midy + 1);
        m->place_items("office", 80, x1 + 1, y1 + 1, x2 - 1, y2 - 1, false, 0);
        break;

    case room_mine_storage:
        m->place_items("mine_storage", 85, x1 + 2, y1 + 2, x2 - 2, y2 - 2, false, 0);
        break;

    case room_mine_fuel: {
        int spacing = rng(2, 4);
        if (door_side == NORTH || door_side == SOUTH) {
            int y = (door_side == NORTH ? y1 + 2 : y2 - 2);
            for (int x = x1 + 1; x <= x2 - 1; x += spacing) {
                m->place_gas_pump(x, y, rng(10000, 50000));
            }
        } else {
            int x = (door_side == EAST ? x2 - 2 : x1 + 2);
            for (int y = y1 + 1; y <= y2 - 1; y += spacing) {
                m->place_gas_pump(x, y, rng(10000, 50000));
            }
        }
    }
    break;

    case room_mine_housing:
        if (door_side == NORTH || door_side == SOUTH) {
            for (int y = y1 + 2; y <= y2 - 2; y += 2) {
                m->ter_set(x1    , y, t_window);
                m->furn_set(x1 + 1, y, f_bed);
                m->place_items("bed", 60, x1 + 1, y, x1 + 1, y, false, 0);
                m->furn_set(x1 + 2, y, f_bed);
                m->place_items("bed", 60, x1 + 2, y, x1 + 2, y, false, 0);
                m->ter_set(x2    , y, t_window);
                m->furn_set(x2 - 1, y, f_bed);
                m->place_items("bed", 60, x2 - 1, y, x2 - 1, y, false, 0);
                m->furn_set(x2 - 2, y, f_bed);
                m->place_items("bed", 60, x2 - 2, y, x2 - 2, y, false, 0);
                m->furn_set(x1 + 1, y + 1, f_dresser);
                m->place_items("dresser", 78, x1 + 1, y + 1, x1 + 1, y + 1, false, 0);
                m->furn_set(x2 - 1, y + 1, f_dresser);
                m->place_items("dresser", 78, x2 - 1, y + 1, x2 - 1, y + 1, false, 0);
            }
        } else {
            for (int x = x1 + 2; x <= x2 - 2; x += 2) {
                m->ter_set(x, y1    , t_window);
                m->furn_set(x, y1 + 1, f_bed);
                m->place_items("bed", 60, x, y1 + 1, x, y1 + 1, false, 0);
                m->furn_set(x, y1 + 2, f_bed);
                m->place_items("bed", 60, x, y1 + 2, x, y1 + 2, false, 0);
                m->ter_set(x, y2    , t_window);
                m->furn_set(x, y2 - 1, f_bed);
                m->place_items("bed", 60, x, y2 - 1, x, y2 - 1, false, 0);
                m->furn_set(x, y2 - 2, f_bed);
                m->place_items("bed", 60, x, y2 - 2, x, y2 - 2, false, 0);
                m->furn_set(x + 1, y1 + 1, f_dresser);
                m->place_items("dresser", 78, x + 1, y1 + 1, x + 1, y1 + 1, false, 0);
                m->furn_set(x + 1, y2 - 1, f_dresser);
                m->place_items("dresser", 78, x + 1, y2 - 1, x + 1, y2 - 1, false, 0);
            }
        }
        m->place_items("bedroom", 65, x1 + 1, y1 + 1, x2 - 1, y2 - 1, false, 0);
        break;
    default:
        //Supress warnings
        break;
    }

    if (type == room_mine_fuel) { // Fuel stations are open on one side
        switch (door_side) {
        case NORTH:
            line(m, t_floor, x1, y1    , x2, y1    );
            break;
        case EAST:
            line(m, t_floor, x2, y1 + 1, x2, y2 - 1);
            break;
        case SOUTH:
            line(m, t_floor, x1, y2    , x2, y2    );
            break;
        case WEST:
            line(m, t_floor, x1, y1 + 1, x1, y2 - 1);
            break;
        default:
            break;
        }
    } else {
        if (type == room_mine_storage) { // Storage has a locked door
            m->ter_set(door_point.x, door_point.y, t_door_locked);
        } else {
            m->ter_set(door_point.x, door_point.y, t_door_c);
        }
    }
}