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


#include <iostream>
#include <geos_c.h>
#include <jni.h>
#include <cstring>

#include "palgeometrywrapper.h"


PalGeometryWrapper::PalGeometryWrapper (JNIEnv *env, jobject jpalGeom) : jpalGeom(jpalGeom), env(env) {
    nbAccess = 0;
}

PalGeometryWrapper::~PalGeometryWrapper(){
    env->DeleteGlobalRef(jpalGeom);
}

GEOSGeometry *PalGeometryWrapper::getGeosGeometry (){

    std::cout << "getgeosGeometry !" << std::endl;

    std::cout << "geom jobject :" << jpalGeom << std::endl;
    std::cout << "3nv :" << env << std::endl;
    std::cout << "wkb fromn java :";
    std::cout.flush();
    jclass jpalgeometry = env->FindClass("ch/heigvd/jpal/JPalGeometry"); 
    std::cout << "jclass :" << jpalgeometry << std::endl;
    jmethodID jgetWKB = env->GetMethodID (jpalgeometry, "getWKBHexGeometry", "()Ljava/lang/String;");
    std::cout << "jmethodID" << jgetWKB << std::endl;
    jstring jWKB = (jstring)env->CallObjectMethod(jpalGeom, jgetWKB);
    std::cout << "jWKB: " << jWKB << std::endl;

    const char *wkb = env->GetStringUTFChars (jWKB, NULL);

    std::cout << "ok" << std::endl;

    std::cout << "convert to GEOSGeometry: ";
    GEOSWKBReader *reader = GEOSWKBReader_create();

    //the_geom = GEOSGeomFromWKB_buf((const unsigned char *)wkb, strlen(wkb));
    the_geom = GEOSWKBReader_readHEX(reader, (const unsigned char *)wkb, strlen(wkb));

    GEOSWKBReader_destroy(reader);

    std::cout << "ok" << std::endl;

    nbAccess++;
    if (nbAccess > 1){
        std::cout << "Concurent acces should never occurs !!!" << std::endl;
    }

    env->ReleaseStringUTFChars (jWKB, wkb);

    std::cout << "finifiniok" << std::endl;
    return the_geom;
}

void PalGeometryWrapper::releaseGeosGeometry (GEOSGeometry *the_geom){
    nbAccess--;
    if (nbAccess != 0){
        std::cout << " nbAcces should be 0" << std::endl;
    }

    if (the_geom != this->the_geom){
        std::cout << "Not the same geometry !!!" << std::endl;
    }

    GEOSGeom_destroy (this->the_geom);
    this->the_geom = NULL;
}

