/*
 *   libpal - Automated Placement of Labels Library     http://pal.heig-vd.ch
 *
 *
 *   Copyright (C) 2007, 2008 MIS-TIC, HEIG-VD (University of Applied Sciences Western Switzerland)
 *   Copyright (C) 2008, 2009 IICT-SYSIN, HEIG-VD (University of Applied Sciences Western Switzerland)
 *
 *
 * This file is part of libpal.
 *
 * libpal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libpal. If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <time.h>

#include <cstdlib>
#include <cfloat>
#include <cstring>
#include <list>
#include <cmath>

#include <iostream>
#include <sstream>

#include <exception>

#include <pal/pal.h>
#include <pal/layer.h>
#include <pal/label.h>

#include "Geom.h"

#define EXPECTED_VERSION 1

void getLine (std::istream &is, char **line) {
    if (*line) {
        delete[] *line;
        *line = NULL;
    }

    if (is.eof()) {
        return;
    }

    std::ostringstream *oss = NULL;

    while (is.good()) {
        char c = is.get();

        if (oss == NULL) {
            oss = new std::ostringstream();
        }

        *oss << c;
        if (c == '\n') {
            break;
        }
    }


    if (oss) {
        *line = new char[strlen (oss->str().c_str()) +1];
        strcpy (*line, oss->str().c_str());
        delete oss;

        if (strlen (*line) == 1 && *line[0] == '\n') {
            delete[] *line;
            *line = NULL;
        }
    }
}


char *basefilename(char *filename){
    char *p;
    // check for unix-like path
    p = strrchr(filename,  '/');
    if (!p){ 
        // Check for windows-like pale
        p = strrchr(filename, '\\');

        if (!p){
            return (filename);
        }
    }

    return (p+1);
}

int main (int argc, char **argv) {


    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " problemfile scale {CHAIN, POP_TABU, POP_CHAIN, POP_TABU_CHAIN}" << std::endl;
        return -1;
    }


    char *filename = new char[strlen (argv[1]) + 1];
    strcpy (filename, argv[1]);

    char *basename = basefilename(filename);

    double scale = strtod (argv[2], NULL);

    std::filebuf fb;
    fb.open (filename, std::ios::in);
    std::istream is (&fb);

    double label_x = -1;
    double label_y = -1;


    char *methodString = new char[strlen (argv[3]) + 1];
    strcpy (methodString, argv[3]);

    pal::SearchMethod method;

    if (strcmp (methodString, "POP_TABU") == 0) {
        method = pal::POPMUSIC_TABU;
    } else if (strcmp (methodString, "POP_TABU_CHAIN") == 0) {
        method = pal::POPMUSIC_TABU_CHAIN;
    } else if (strcmp (methodString, "POP_CHAIN") == 0) {
        method = pal::POPMUSIC_CHAIN;
    } else if (strcmp (methodString, "CHAIN") == 0) {
        method = pal::CHAIN;
    } else {
        std::cerr << "Unknown search method..." << std::endl;
        exit (-1);
    }


    // Read global info
    int point_p = -1, line_p = -1, poly_p = -1;
    int dpi = -1;
    int version = -1;

    pal::Units map_unit = pal::METER;

    char *line = NULL;
    getLine (is, &line);
    while (line && (strncmp (line, "%LAYER ", 7) != 0)) {
        if (strncmp (line, "%VERSION ", 9) == 0) {
            version = atoi (&line[9]);
        } else if (strncmp (line, "%DPI ", 5) == 0) {
            dpi = atoi (&line[5]);
        } else if (strncmp (line, "%P_POINT ", 8) == 0) {
            point_p = atoi (&line[8]);
        } else if (strncmp (line, "%P_LINE ", 7) == 0) {
            line_p = atoi (&line[7]);
        } else if (strncmp (line, "%P_POLYGON ", 11) == 0) {
            poly_p = atoi (&line[11]);
        } else if (strncmp (line, "%MAP_UNIT ", 10) == 0) {
            fprintf (stderr, "MAP_UNIT:\n");
            if (strcmp(&line[10], "METER\n") == 0){
                fprintf(stderr, "  METER\n");
                map_unit = pal::METER;
            } else if (strcmp(&line[10], "FOOT\n") == 0){
                fprintf(stderr, "  FOOT\n");
                map_unit = pal::FOOT;
            } else if (strcmp(&line[10], "DEGREE\n") == 0){
                fprintf(stderr, "  DEGREE\n");
                map_unit = pal::DEGREE;
            }
        }
        getLine (is, &line);
    }

    if (version < EXPECTED_VERSION) {
        std::cout << "Sorry, version not match... (found " << version << ", " << EXPECTED_VERSION << " expected)" << std::endl;
    }

    pal::Pal *pal = new pal::Pal();

    if (point_p > 0) {
        std::cout << "point p : " << point_p << std::endl;
        pal->setPointP (point_p);
    }
    if (line_p > 0) {
        std::cout << "line p : " << line_p << std::endl;
        pal->setLineP (line_p);
    }
    if (poly_p > 0) {
        std::cout << "poly p : " << poly_p << std::endl;
        pal->setPolyP (poly_p);
    }
    if (dpi > 0) {
        std::cout <<  "DPI : " << dpi << std::endl;
        pal->setDpi (dpi);
    }


    pal->setSearch (method);

    pal->setMapUnit(map_unit);

    std::list<Geom*> *the_geoms = new std::list<Geom*>();

    do {
        char *name = NULL;
        bool isObstacle = false;
        pal::Arrangement arrange;
        pal::Units unit;
        double priority = -10;

        bool have_unit = false;
        bool have_arrange = false;

        while (strncmp (line, "%FEATURES", 9) != 0) {

            if (strncmp ("%LAYER ", line, 7) == 0) {
                name = new char[strlen (&line[7]) + 1];
                strcpy (name, &line[7]);
                if (name[strlen (name)-1] == '\n') {
                    name[strlen (name)-1] = '\0';
                }
                std::cout << "Layer name: " << name << std::endl;
            } else if (strncmp ("%ARRANGE ", line, 9) == 0) {
                if (strncmp (&line[9], "P_POINT", 7) == 0) {
                    have_arrange = true;
                    arrange = pal::P_POINT;
                } else if (strncmp (&line[9], "P_LINE", 6) == 0) {
                    have_arrange = true;
                    arrange = pal::P_LINE;
                } else if (strncmp (&line[9], "P_LINE_AROUND", 13) == 0) {
                    have_arrange = true;
                    arrange = pal::P_LINE_AROUND;
                } else if (strncmp (&line[9], "P_HORIZ", 7) == 0) {
                    have_arrange = true;
                    arrange = pal::P_HORIZ;
                } else if (strncmp (&line[9], "P_FREE", 6) == 0) {
                    have_arrange = true;
                    arrange = pal::P_FREE;
                }
            } else if (strncmp ("%OBSTACLE", line, 9) == 0) {
                isObstacle = true;
            } else if (strncmp ("%UNIT ", line, 6) == 0) {
                if (strncmp (&line[6], "METER", 6) == 0) {
                    have_unit = true;
                    unit = pal::METER;
                } else if (strncmp (&line[6], "PIXEL", 5) == 0) {
                    have_unit = true;
                    unit = pal::PIXEL;
                }
            } else if (strncmp ("%PRIORITY ", line, 10) == 0) {
                priority = strtod (&line[10], NULL);
            }

            getLine (is, &line);
        }

        if (priority < -1) {
            std::cout << "Priority not set for layer " << name << ": use default value (0.5)" << std::endl;
            priority = 0.5;
        }

        if (!have_arrange) {
            std::cout << "Arrange mode not specified for layer " << name << ", use default" << std::endl;
            arrange = pal::P_HORIZ;
        }

        if (!have_unit) {
            std::cout << "Label units not dspecified for layer " << name << "using PIXEL" << std::endl;
            unit = pal::PIXEL;
        }

        pal::Layer * layer = pal->addLayer (name, -1, -1, arrange, unit, priority, isObstacle, true, true);
        delete[] name;
        int it = 1;
        do {
            getLine (is, &line);

            if (strncmp (line, "%LAYER", 6) != 0 && strncmp (line, "%MAP_EXTENT", 11) != 0) {
                sscanf (line, "%lf %lf", &label_x, &label_y);


                //std::cout << "label size: " << label_x << " " << label_y << std::endl;
                getLine (is, &line);

                try{
                    Geom *geom = new Geom (line);

                    char *gid = new char[int (ceil (log (it))) +2];
                    sprintf (gid, "%d", it);

                    the_geoms->push_back (geom);

                    layer->registerFeature (gid, geom, label_x, label_y);

                    delete[] gid;
                } catch (std::exception &e){
                   std::cout << e.what() << ": " << line << std::endl;
                }

            }
            it++;
        } while (strncmp (line, "%MAP_EXTENT", 11) != 0 && strncmp (line, "%LAYER ", 7) != 0);

    } while (strncmp (line, "%MAP_EXTENT", 11) != 0);

    //getLine (is, &line);

    double xmin, xmax, ymin, ymax;

    if (strncmp (line, "%MAP_EXTENT", 11) == 0) {
        sscanf (&line[11], "%lf %lf %lf %lf", &xmin, &ymin, &xmax, &ymax);
        std::cout << "Extent from file : " << xmin << " " << ymin << " " << xmax << " " << ymax << std::endl;
    } else {
        std::cout << "Map extent is missing" << std::endl;
        exit (-1);
    }


    /*
    xmin -= 2 * (label_x + distlabel);
    xmax += 2 * (label_x + distlabel);
    ymin -= 2 * (label_y + distlabel);
    ymax += 2 * (label_y + distlabel);
    */

    delete[] line;

    pal::PalStat *stats;

    double bbox[4] = {xmin, ymin, xmax, ymax};

    std::cerr << basename << "\t";

    std::list<pal::Label*> * labels = pal->labeller (scale, bbox, &stats, false);

    while (labels->size()>0){
        delete labels->front();
        labels->pop_front();
    }

    delete stats;

    delete labels;


    delete pal;

    delete[] methodString;
    delete[] filename;


    while (the_geoms->size()>0){
        delete the_geoms->front();
        the_geoms->pop_front();
    }
    delete the_geoms;

    return 0;
}
