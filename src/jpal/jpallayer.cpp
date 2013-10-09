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


#include <jni.h>
#include <iostream>
#include <pal/layer.h>

#include "jpal.h"
#include "jpallayer.h"
#include "palgeometrywrapper.h"

using namespace pal;
 
JNIEXPORT jstring JNICALL Java_ch_heigvd_jpal_JPalLayer_getName
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return env->NewStringUTF (layer->getName());
}



JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setActive
(JNIEnv *env, jobject obj, jboolean active) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    layer->setActive (active);
}

JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setToLabel
(JNIEnv *env, jobject obj, jboolean toLabel) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    layer->setToLabel (toLabel);
}


JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setObstacle
(JNIEnv *env, jobject obj, jboolean obstacle) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    layer->setObstacle (obstacle);
}





JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setMinScale
(JNIEnv *env, jobject obj, jdouble min_scale) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);


    layer->setMinScale (min_scale);
}




JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setMaxScale
(JNIEnv *env, jobject obj, jdouble max_scale) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    layer->setMaxScale (max_scale);
}




JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setPriority
(JNIEnv *env, jobject obj, jdouble priority) {
    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    layer->setPriority (priority);
}


JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_registerFeature
(JNIEnv *env, jobject obj, jstring id, jobject userGeom, jdouble label_x, jdouble label_y) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);


    jobject back_ptr = env->NewGlobalRef (userGeom);

    PalGeometryWrapper *wrapper = new PalGeometryWrapper (env, back_ptr); 

    layer->registerFeature (feat_id, wrapper, label_x, label_y);

    env->ReleaseStringUTFChars (id, feat_id);
}

JNIEXPORT jdouble JNICALL Java_ch_heigvd_jpal_JPalLayer_getFeatureLabelWidth
(JNIEnv *env, jobject obj, jstring id) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);

    double size = layer->getFeatureLabelWidth (feat_id);

    env->ReleaseStringUTFChars (id, feat_id);

    return size;
}



JNIEXPORT jdouble JNICALL Java_ch_heigvd_jpal_JPalLayer_getFeatureLabelHeight
(JNIEnv *env, jobject obj, jstring id) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);

    double size = layer->getFeatureLabelHeight (feat_id);

    env->ReleaseStringUTFChars (id, feat_id);

    return size;
}


JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setFeatureLabelSize
(JNIEnv *env, jobject obj, jstring id, jdouble label_x, jdouble label_y) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);

    layer->setFeatureLabelSize (feat_id, label_x, label_y);

    env->ReleaseStringUTFChars (id, feat_id);
}


JNIEXPORT jboolean JNICALL Java_ch_heigvd_jpal_JPalLayer_isObstacle
(JNIEnv *env, jobject obj) {
    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->isObstacle();

}


JNIEXPORT jboolean JNICALL Java_ch_heigvd_jpal_JPalLayer_isActive
(JNIEnv *env, jobject obj) {
    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->isActive();

}


JNIEXPORT jboolean JNICALL Java_ch_heigvd_jpal_JPalLayer_isToLabel
(JNIEnv *env, jobject obj) {
    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->isToLabel();

}



JNIEXPORT jdouble JNICALL Java_ch_heigvd_jpal_JPalLayer_getMinScale
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);


    return layer->getMinScale();
}




JNIEXPORT jdouble JNICALL Java_ch_heigvd_jpal_JPalLayer_getMaxScale
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->getMaxScale();
}


JNIEXPORT jdouble JNICALL Java_ch_heigvd_jpal_JPalLayer_getPriority
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->getPriority();
}

JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPalLayer_getArrange
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->getArrangement();
}

JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setArrange
(JNIEnv *env, jobject obj, jint align) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    Arrangement ar;

    switch (align) {
    case ch_heigvd_jpal_JPal_P_LINE:
        ar = P_LINE;
        break;
    case ch_heigvd_jpal_JPal_P_LINE_AROUND:
        ar = P_LINE_AROUND;
        break;
    case ch_heigvd_jpal_JPal_P_POINT:
        ar = P_POINT;
        break;
    case ch_heigvd_jpal_JPal_P_HORIZ:
        ar = P_HORIZ;
        break;
    case ch_heigvd_jpal_JPal_P_FREE:
    default:
        ar = P_FREE;
        break;
    }
    layer->setArrangement (ar);
}


/*
 * Class:     ch_heigvd_jpal_JPalLayer
 * Method:    jpal_setFeatureDistlabel
 * Signature: (Ljava/lang/String;DD)V
 */
JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setFeatureDistlabel
(JNIEnv *env, jobject obj, jstring id, jint distlabel) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);

    layer->setFeatureDistlabel (feat_id, distlabel);

    env->ReleaseStringUTFChars (id, feat_id);
}


/*
 * Class:     ch_heigvd_jpal_JPalLayer
 * Method:    jpal_getFeatureDistlabel
 * Signature: (Ljava/lang/String;)D
 */
JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPalLayer_getFeatureDistlabel
(JNIEnv *env, jobject obj, jstring id) {


    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    const char *feat_id = env->GetStringUTFChars (id, NULL);

    int dl = layer->getFeatureDistlabel (feat_id);

    env->ReleaseStringUTFChars (id, feat_id);

    return dl;
}


/*
 * Class:     ch_heigvd_jpal_JPalLayer
 * Method:    getUnit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPalLayer_getLabelUnit
(JNIEnv *env , jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    return layer->getLabelUnit();

}

/*
 * Class:     ch_heigvd_jpal_JPalLayer
 * Method:    setUnit
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPalLayer_setLabelUnit
(JNIEnv *env, jobject obj, jint unit) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_layer_adr", "J");
    Layer *layer = (Layer*) env->GetLongField (obj, fid);

    Units un;

    switch (unit) {
    case ch_heigvd_jpal_JPal_UNIT_PIXEL:
        un  = PIXEL;
        break;
    case ch_heigvd_jpal_JPal_UNIT_METER:
        un = METER;
        break;
    default:
        un = PIXEL;
        break;
    }

    layer->setLabelUnit (un);
}



