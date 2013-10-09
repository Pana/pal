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



#ifndef _palgeometrywrapper_h
#define _palgeometrywrapper_h

#include <jni.h>
#include <geos_c.h>
#include <pal/palgeometry.h>

class PalGeometryWrapper : public pal::PalGeometry {
    private:
        jobject jpalGeom;
        int nbAccess;
        JNIEnv *env;
        GEOSGeometry *the_geom;

    public:

        PalGeometryWrapper (JNIEnv *env, jobject jpalGeom);

        virtual GEOSGeometry *getGeosGeometry ();
        virtual void releaseGeosGeometry (GEOSGeometry *the_geom);

        virtual ~PalGeometryWrapper();
};

#endif
