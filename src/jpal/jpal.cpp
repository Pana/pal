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
#include <cstring>

#include <pal/pal.h>
#include <pal/label.h>
#include <pal/palstat.h>

#include "jpal.h"

using namespace pal;

JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_new_1pal
(JNIEnv *env, jobject obj) {
    Pal *pal = new Pal();

    std::cout << "JNI_WRAP: new pal: " << pal << std::endl;

    jclass cls = env->GetObjectClass (obj);
    jmethodID mid = env->GetMethodID (cls, "setPalAdr", "(J)V");

    env->CallVoidMethod (obj, mid, (long) pal);
}

JNIEXPORT jobject JNICALL Java_ch_heigvd_jpal_JPal_addLayer
(JNIEnv *env, jobject obj, jstring name, jdouble min_scale, jdouble max_scale , jint arrange, jint unit, jdouble priority, jboolean obstacle, jboolean active, jboolean toLabel) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    std::cout << "JNI_WRAP: pal:" << pal << std::endl;

    const char *layerName = env->GetStringUTFChars (name, NULL);

    Arrangement ar;
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

    switch (arrange) {
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

    Layer *new_layer = pal->addLayer (layerName, min_scale, max_scale, ar, un, priority, obstacle, active, toLabel);

    std::cout << "JNI_WRAP: Layer added" << std::endl;

    // Create a JPalLayer and put jpallayer_adr to (long)layer*
    jclass jpalLayer = env->FindClass ("ch/heigvd/jpal/JPalLayer");
    jmethodID jpalLayerInit = env->GetMethodID (jpalLayer, "<init>", "()V");
    jfieldID palLayerAdr = env->GetFieldID (jpalLayer, "pal_layer_adr", "J");

    jobject jpalLayerObj = env->NewObject (jpalLayer, jpalLayerInit);

    std::cout << "JNI_WRAP: layer adr: " << new_layer << std::endl;

    env->SetLongField (jpalLayerObj, palLayerAdr, (long) new_layer);

    return jpalLayerObj;
}

JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_setDPI
(JNIEnv *env, jobject obj, jint dpi) {

    if (dpi <= 0)
        return;

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    pal->setDpi (dpi);
}



JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_setPointP
(JNIEnv *env, jobject obj, jint pointp) {

    if (pointp <= 0)
        return;

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    pal->setPointP (pointp);
}


JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_setLineP
(JNIEnv *env, jobject obj, jint linep) {

    if (linep <= 0)
        return;

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    pal->setLineP (linep);
}



JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_setPolyP
(JNIEnv *env, jobject obj, jint polyp) {

    if (polyp <= 0)
        return;

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    pal->setPolyP (polyp);
}




JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPal_getDPI
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    return pal->getDpi();

}


JNIEXPORT jobject JNICALL Java_ch_heigvd_jpal_JPal_getLayers
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    std::list<Layer*> *layers = pal->getLayers();

    jclass jpalLayer = env->FindClass ("ch/heigvd/jpal/JPalLayer");
    jmethodID jpalLayerInit = env->GetMethodID (jpalLayer, "<init>", "()V");
    jfieldID jpalLayerAdr = env->GetFieldID (jpalLayer, "pal_layer_adr", "J");

    jclass listClass = env->FindClass ("java/util/LinkedList");
    jmethodID listInit = env->GetMethodID (listClass, "<init>", "()V");
    jmethodID listAdd = env->GetMethodID (listClass, "add", "(Ljava/lang/Object;)Z");

    jobject jlist = env->NewObject (listClass, listInit);

    for (std::list<Layer*>::iterator it = layers->begin();it != layers->end();it++) {
        // Create a JPalLayer and set jpallayer_adr to (long)layer*
        jobject jpalLayerObj = env->NewObject (jpalLayer, jpalLayerInit);
        env->SetLongField (jpalLayerObj, jpalLayerAdr, (long) (*it));

        // add layer into jlist
        env->CallBooleanMethod (jlist, listAdd, jpalLayerObj);
    }

    env->DeleteLocalRef (listClass);
    env->DeleteLocalRef (jpalLayer);

    return jlist;
}


JNIEXPORT jobject JNICALL Java_ch_heigvd_jpal_JPal_labeller_1all
(JNIEnv *env, jobject obj, jdouble scale, jdoubleArray bbox, jobjectArray jstats, jboolean displayAll) {

    std::cout << "JNI_WRAP: Labeller_all" << std::endl;

    double bounding_box[4];

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    env->GetDoubleArrayRegion (bbox, 0, 4, bounding_box);


    std::cout << "JNI_WRAP: Labeller is going to run:" << std::endl;

    PalStat *stats;

    std::cout << "JNI_WRAP displayAll:" << displayAll << std::endl;

    std::list<Label*> *solList = pal->labeller (scale, bounding_box, &stats, displayAll);
    std::cout << "JNI_WRAP: Labelling done... translate stats..." << std::endl;

    int nbLayers;

    nbLayers = stats->getNbLayers();

    jclass stringClass = env->FindClass ("java/lang/String");

    jobjectArray lyrsNames = env->NewObjectArray (nbLayers, stringClass, NULL);
    jintArray nbLyrObjs   = env->NewIntArray (nbLayers);
    jintArray nbLyrLObjs  = env->NewIntArray (nbLayers);

    jint *lnbo = new jint[nbLayers];
    jint *lnblo = new jint[nbLayers];

    int k;
    for (k = 0;k < nbLayers;k++) {
        env->SetObjectArrayElement (lyrsNames, k, env->NewStringUTF (stats->getLayerName (k)));
        lnbo[k] = stats->getLayerNbObjects (k);
        lnblo[k] = stats->getLayerNbLabelledObjects (k);
    }

    env->SetIntArrayRegion (nbLyrObjs, 0, nbLayers, lnbo);
    env->SetIntArrayRegion (nbLyrLObjs, 0, nbLayers, lnblo);

    delete[] lnbo;
    delete[] lnblo;

    jclass jpalStatClass = env->FindClass ("ch/heigvd/jpal/JPalStat");
    jmethodID jpalStatSet = env->GetMethodID (jpalStatClass, "set", "(II[Ljava/lang/String;[I[I)V");

    env->CallVoidMethod (jstats,
                         jpalStatSet, stats->getNbObjects(), stats->getNbLabelledObjects(), lyrsNames, nbLyrObjs, nbLyrLObjs); //env->NewObject(jpalStatClass, jpalStatInit,

    env->DeleteLocalRef (lyrsNames);
    env->DeleteLocalRef (nbLyrObjs);
    env->DeleteLocalRef (nbLyrLObjs);

    delete stats;

    std::cout << "JNI_WRAP: Labelling is finished" << std::endl;


    jclass listClass;
    jclass labelClass;

    jmethodID listInit;
    jmethodID listAdd;
    jmethodID labelInit;

    listClass   = env->FindClass ("java/util/LinkedList");
    labelClass  = env->FindClass ("ch/heigvd/jpal/JPalLabel");

    listInit   = env->GetMethodID (listClass, "<init>", "()V");
    listAdd    = env->GetMethodID (listClass, "add", "(Ljava/lang/Object;)Z");
    labelInit  = env->GetMethodID (labelClass, "<init>", "(DDDLjava/lang/String;Ljava/lang/String;)V");


    jstring layerName;
    jstring featID;

    jobject jlist = env->NewObject (listClass, listInit);
    //jobject jlabel;

    Label *label;

    for (std::list<Label*>::iterator it = solList->begin(); it != solList->end();it++) {
        label = *it;

        layerName = env->NewStringUTF (label->getLayerName());
        featID = env->NewStringUTF (label->getFeatureId());

        env->CallBooleanMethod (jlist, listAdd, env->NewObject (labelClass, labelInit, label->getOrigX(), label->getOrigY(), label->getRotation(), layerName, featID));

        delete label;
    }

    env->DeleteLocalRef (listClass);
    env->DeleteLocalRef (labelClass);


    return jlist;



}

JNIEXPORT jobject JNICALL Java_ch_heigvd_jpal_JPal_labeller
(JNIEnv *env, jobject obj, jobjectArray layers, jdoubleArray layersFactor, jdouble scale, jdoubleArray bbox, jobjectArray jstats, jboolean displayAll) {

    std::cout << "Env (labeller): " << env << std::endl;
    std::cout << "obj (labeller): " << obj << std::endl;
    int i;
    const char *str;

    char **layersN;
    double *layersF;
    int nbLayer = env->GetArrayLength (layers);
    double bounding_box[4];

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    layersN = new char*[nbLayer];
    layersF = new double[nbLayer];

    //str = env->GetStringUTFChars(name, NULL);
    //env->ReleaseStringUTFChars(name, str);

    env->GetDoubleArrayRegion (bbox, 0, 4, bounding_box);
    env->GetDoubleArrayRegion (layersFactor, 0, nbLayer, layersF);

    for (i = 0;i < nbLayer;i++) {
        jstring jstr = (jstring) env->GetObjectArrayElement (layers, i);
        str = env->GetStringUTFChars (jstr, NULL);

        layersN[i] = new char[strlen (str) +1];
        strcpy (layersN[i], str);

        std::cout << "JNI_WRAP: Layers " << i  << " : " << layersN[i] << " (" << layersF[i] << ")" << std::endl;

        env->ReleaseStringUTFChars (jstr, str);
        env->DeleteLocalRef (jstr);
    }

    std::cout << "JNI_WRAP: Labeller is going to run:" << std::endl;

    PalStat * stats;

    std::list<Label*> *solList = pal->labeller (nbLayer, layersN, layersF, scale, bounding_box, &stats, displayAll);


    std::cout << "JNI_WRAP: Labelling done... translate stats..." << std::endl;

    int nbLayers = stats->getNbLayers();

    jclass stringClass = env->FindClass ("java/lang/String");

    jobjectArray lyrsNames = env->NewObjectArray (nbLayers, stringClass, NULL);
    jintArray nbLyrObjs   = env->NewIntArray (nbLayers);
    jintArray nbLyrLObjs  = env->NewIntArray (nbLayers);

    jint *lnbo = new jint[nbLayers];
    jint *lnblo = new jint[nbLayers];

    int k;
    for (k = 0;k < nbLayers;k++) {
        env->SetObjectArrayElement (lyrsNames, k, env->NewStringUTF (stats->getLayerName (k)));
        lnbo[k] = stats->getLayerNbObjects (k);
        lnblo[k] = stats->getLayerNbLabelledObjects (k);
    }

    env->SetIntArrayRegion (nbLyrObjs, 0, nbLayers, lnbo);
    env->SetIntArrayRegion (nbLyrLObjs, 0, nbLayers, lnblo);

    delete[] lnbo;
    delete[] lnblo;

    jclass jpalStatClass = env->FindClass ("ch/heigvd/jpal/JPalStat");
    jmethodID jpalStatSet = env->GetMethodID (jpalStatClass, "set", "(II[Ljava/lang/String;[I[I)V");

    env->CallVoidMethod (jstats,
                         jpalStatSet, stats->getNbObjects(), stats->getNbLabelledObjects(), lyrsNames, nbLyrObjs, nbLyrLObjs); //env->NewObject(jpalStatClass, jpalStatInit,

    env->DeleteLocalRef (lyrsNames);
    env->DeleteLocalRef (nbLyrObjs);
    env->DeleteLocalRef (nbLyrLObjs);

    delete stats;

    std::cout << "JNI_WRAP: Labelling is finished" << std::endl;


    jclass listClass;
    jclass labelClass;

    jmethodID listInit;
    jmethodID listAdd;
    jmethodID labelInit;

    listClass   = env->FindClass ("java/util/LinkedList");
    labelClass  = env->FindClass ("ch/heigvd/jpal/JPalLabel");

    listInit   = env->GetMethodID (listClass, "<init>", "()V");
    listAdd    = env->GetMethodID (listClass, "add", "(Ljava/lang/Object;)Z");
    labelInit  = env->GetMethodID (labelClass, "<init>", "(DDDLjava/lang/String;Ljava/lang/String;)V");


    jstring layerName;
    jstring featID;

    jobject jlist = env->NewObject (listClass, listInit);
    //jobject jlabel;

    Label *label;

    for (std::list<Label*>::iterator it = solList->begin(); it != solList->end();it++) {
        label = *it;

        layerName = env->NewStringUTF (label->getLayerName());
        featID = env->NewStringUTF (label->getFeatureId());

        env->CallBooleanMethod (jlist, listAdd, env->NewObject (labelClass, labelInit, label->getOrigX(), label->getOrigY(), label->getRotation(), layerName, featID));

//std::cout << "Identifier: " << label->getLayerName() << "/" << label->getFeatureId() << std::endl;
//       std::cout << "coord:" << label->getOrigX() << "   " << label->getOrigY() << std::endl;
//       std::cout << "alpha: " << label->getRotation() << std::endl << std::endl;
        delete label;
    }

    env->DeleteLocalRef (listClass);
    env->DeleteLocalRef (labelClass);

    return jlist;
}

JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPal_getPointP
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    return pal->getPointP();
}

JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPal_getLineP
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    return pal->getLineP();
}

JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPal_getPolyP
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    return pal->getPolyP();
}


JNIEXPORT jobject JNICALL Java_ch_heigvd_jpal_JPal_getLayer
(JNIEnv *env, jobject obj, jstring layerName) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    const char *name = env->GetStringUTFChars (layerName, NULL);

    Layer * layer = pal->getLayer (name);

    // Create a JPalLayer and put jpallayer_adr to (long)layer*
    jclass jpalLayer = env->FindClass ("ch/heigvd/jpal/JPalLayer");
    jmethodID jpalLayerInit = env->GetMethodID (jpalLayer, "<init>", "()V");
    jfieldID jpalLayerAdr = env->GetFieldID (jpalLayer, "pal_layer_adr", "J");

    jobject jpalLayerObj = env->NewObject (jpalLayer, jpalLayerInit);

    std::cout << "JNI_WRAP: layer adr: " << layer << std::endl;

    env->SetLongField (jpalLayerObj, jpalLayerAdr, (long) layer);

    return jpalLayerObj;
}


/*
 * Class:     ch_heigvd_jpal_JPal
 * Method:    jpal_removeLayer
 * Signature: (Ljava/lang/String;)V;
 */
JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_removeLayer
(JNIEnv * env, jobject obj, jstring lyrName) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    const char *name = env->GetStringUTFChars (lyrName, NULL);

    pal->removeLayer (pal->getLayer (name));
}

/*
 * Class:     ch_heigvd_jpal_JPal
 * Method:    jpal_setSearchMethod
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_ch_heigvd_jpal_JPal_setSearchMethod
(JNIEnv *env, jobject obj, jint method) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    switch (method) {
    case ch_heigvd_jpal_JPal_POPMUSIC_TABU:
        pal->setSearch (POPMUSIC_TABU);
        break;
    case ch_heigvd_jpal_JPal_POPMUSIC_CHAIN:
        pal->setSearch (POPMUSIC_CHAIN);
        break;
    case ch_heigvd_jpal_JPal_POPMUSIC_TABU_CHAIN:
        pal->setSearch (POPMUSIC_TABU_CHAIN);
        break;
    case ch_heigvd_jpal_JPal_CHAIN:
        pal->setSearch (CHAIN);
        break;
    }

}

/*
 * Class:     ch_heigvd_jpal_JPal
 * Method:    jpal_getSearchMethod
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_ch_heigvd_jpal_JPal_getSearchMethod
(JNIEnv *env, jobject obj) {

    jclass cls = env->GetObjectClass (obj);
    jfieldID fid = env->GetFieldID (cls, "pal_adr", "J");
    Pal *pal = (Pal*) env->GetLongField (obj, fid);

    SearchMethod method = pal->getSearch();

    int return_value = -1;

    switch (method) {
    case POPMUSIC_TABU:
        return_value = ch_heigvd_jpal_JPal_POPMUSIC_TABU;
        break;
    case POPMUSIC_CHAIN:
        return_value = ch_heigvd_jpal_JPal_POPMUSIC_CHAIN;
        break;
    case POPMUSIC_TABU_CHAIN:
        return_value = ch_heigvd_jpal_JPal_POPMUSIC_TABU_CHAIN;
        break;
    case CHAIN:
        return_value = ch_heigvd_jpal_JPal_CHAIN;
        break;
    }

    return return_value;
}





