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


#include <pal/palgeometry.h>
#include <pal/label.h>

#include <string>
#include <iostream>
#include <sstream>

#include <geos_c.h>

#include "Geom.h"

Geom::Geom (const char *wkt) {
    nb = 0;
    int i;

    GEOSWKTReader *reader = GEOSWKTReader_create();

    the_geom = GEOSWKTReader_read (reader, wkt);
    GEOSWKTReader_destroy (reader);
}

/**
* \brief get the geometry in WKB hexa format
* This method is called by Pal each time it needs a geom's coordinates
* \return WKB Hex buffer
*/
GEOSGeometry* Geom::getGeosGeometry() {
    nb++;
    return the_geom;
}


/**
* \brief Called by Pal when it doesn't need the coordinates anymore
*/
void Geom::releaseGeosGeometry (GEOSGeometry *the_geom) {
    nb--;
    return;
}



/**
* \brief Give back label to display
* Pal will call this method when labels of the feature are fixed
void Geom::addLabel(pal::Label *label){
   std::cout << "One new label" << std::endl;
}
*/


Geom::~Geom() {
    if (nb)
        std::cout << "nb should be 0, not " << nb << std::endl;

    GEOSGeom_destroy (the_geom);
}
