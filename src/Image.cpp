/**
 * @file Image.cpp
 * @note To be implemented by students 
 * @author MP-DGIM, MP-IADE, MP-II (grupo B)
 * @author Estudiante 1: Francisco Javier Checa Casas / Estudiante 2: Laura Riera Ojea
 */
#include<iostream>
#include <fstream>
#include<cstring>
#include "MPTools.h"
#include "Byte.h"
#include "Image.h"
#include "Histogram.h"
using namespace std;

Image::Image() {
    _width = 0;
    _height = 0;
}

Image::Image(int width, int height) {
    Byte b;
    _height = height;
    _width = width;
    b.offByte();
//    for (int i = 0; i < _height * _width; i++) {
//        setPos(i, b);
//    }
    flatten(b);
}

int Image::height() const {
    return _height;
}

int Image::width() const {
    return _width;
}

void Image::setPixel(int x, int y, Byte v) {
    if ((y < _height)&&(x < _width)&&(y >= 0)&&(x >= 0)) {
        _data[y * _width + x] = v;
    } else {
        cerr << "ERROR: (" << y << "," << x << ")" << endl;
    }
}

int Image::getPixel(int x, int y) const {
    if ((y < _height)&&(x < _width)&&(y >= 0)&&(x >= 0)) {
        return (int) _data[y * _width + x].getValue();
    } else
        return -1;
}

void Image::setPos(int i, Byte v) {
    if ((i < _height * _width)&&(i >= 0)) {
        _data[i] = v;
    } else {
        cerr << "ERROR: (" << i << ")" << endl;
    }
}

int Image::getPos(int i) const {
    if ((i < _height * _width)&&(i >= 0))
        return (int) _data[i].getValue();
    else
        return -1;
}

void Image::flatten(Byte b) {
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            setPixel(x, y, b);
        }
    }
}

void Image::showInWindow(string title) const {
    string tempname = "data" + SLASH + ".hidden_" + title + ".pgm";
    mpSavePGMImage(tempname, (const unsigned char *) _data, _width, _height);
    mpDisplayImage(tempname, title);

}

string Image::inspect() const {
    string result = "";
    result = std::to_string(_width) + "x" + std::to_string(_height) + " " + std::to_string(mphash((const unsigned char*) _data, _height * _width));
    return result;
}

Histogram Image::getHistogram() const {
    Histogram result;
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            result.setLevel(getPixel(x, y), result.getLevel(getPixel(x, y)) + 1);
        }
    }
    return result;
}

int Image::readFromFile(const char fichero[]) {
    int h, w, aux;
    Byte b;
    ifstream fi;
    char c1, c2;

    cout << endl << "...Reading image from " << fichero << endl;

    fi.open(fichero);
    if (!fi) {
        return IMAGE_ERROR_OPEN;
    }
    fi >> c1 >> c2;
    if (c1 != 'P') {
        return IMAGE_ERROR_FORMAT;
    }
    if (c2 != '2') {
        return IMAGE_ERROR_FORMAT;
    }
    fi >> w >> h >> aux;
    if (w * h > IMAGE_MAX_SIZE) {
        return IMAGE_TOO_LARGE;
    }
    _height = h;
    _width = w;
    cout << w << "x" << h << endl;
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            fi >> aux;
            this->setPixel(x, y, aux);
        }
    }
    if (!fi) {
        return IMAGE_ERROR_DATA;
    }
    fi.close();
    return IMAGE_DISK_OK;

}

int Image::saveToFile(const char fichero[]) const {
    ofstream fo;
    cout << endl << "...Saving image into " << fichero << endl;
    if (height() == 0 || width() == 0) {
        return IMAGE_DISK_OK;
    }
    fo.open(fichero);
    if (!fo) {
        return IMAGE_ERROR_OPEN;
    }
    fo << "P2" << endl;
    fo << width() << " " << height() << endl;
    fo << "255" << endl;
    for (int y = 0; y < height(); y++) {
        for (int x = 0; x < width(); x++) {
            fo << (int) this->getPixel(x, y) << " ";
        }
    }
    if (!fo) {
        return IMAGE_ERROR_DATA;
    }
    fo.close();
    return IMAGE_DISK_OK;
}

Image Image::depictsHistogram() const {
    int w = 256, h = 160, b = 1, c = 9, a = h - (b + c) - 2;
    int normal;
    Image result(w, h);
    Histogram histogram = getHistogram();
    int max = histogram.getMaxLevel();
    result.flatten(Byte::MAX_BYTE_VALUE);
    for (int x = 0; x < 256; x++) {
        for (int y = 0; y < c; y++) {
            result.setPixel(x, h - 1 - y, x);
        }
        normal = (histogram.getLevel(x) * a) / max;
        for (int y = 0; y <= normal; y++) {
            result.setPixel(x, h - c - b - y - 1, 0);
        }
    }
    return result;
}

void Image::extractObjects(Image imageSet [], int &nimages, int maximages, double tolerance) const {
    Image aux(width(), height());
    Histogram histogram = getHistogram();
    int start, end, max = histogram.getMaxLevel(), count;

    nimages = 0;
    start = -1;
    end = -1; // inicialize end point 

    for (int i = histogram.size()-1; i >= -1; i--) {
        if ( histogram.getLevel(i) *1.0 / max >= tolerance && i >= 0 ) {
            if (end < 0)       // enter in a new group
                end = i;
            
        } else if (end >= 0 && nimages < maximages) { // else  when it gets out of the group && room for another image
            start = i;                                // close the interval
            aux.flatten(0);                           //
            for (int y = 0; y < height(); y++) {
                for (int x = 0; x < width(); x++) {
                    if (start <= getPixel(x, y) && getPixel(x, y) <= end) {
                        aux.setPixel(x, y, getPixel(x, y));
                    }
                }
            }
            cout << "Found object " << nimages << " in [" << start << "," << end << "]" << endl;
            if (nimages < maximages) {
                imageSet[nimages] = aux;
                nimages++;
            }
            end = -1;  // inicialize end point 
        }
    }
}

Image Image::threshold(int t) const{
    
    Image image(width(),height());
    
    if((t<0)||(t>255)){
        t=getHistogram().getBalancedLevel();
    }
    
    for (int i=0; i<width(); i++){
        for (int j=0; j<height(); j++){
            if (getPixel(i,j)<=t){
                image.setPixel(i,j,0);
            }
            else{
                image.setPixel(i,j,255);
            }
        }
    }
    
    return image;
}

Image Image::copyArea(int x, int y, int w, int h) const{
    
    Image image_cutted(w/2,h/2);
    
    if ((x>0) && (y>0)){
        if (((w*h)>Image::IMAGE_MAX_SIZE) && (w>=0 && h>=0)){
            for ( int i=x; i<=w; i++){
                for (int j=y; j<=h; j++){
                    if ((i<_width) && (j<_height)){
                        image_cutted.setPixel(i,j,getPixel(i,j));
                    }    
                }
            }
           
        }
        return image_cutted;
    }    
    return image_cutted;
}

void Image::pasteArea(int x, int y, const Image from, int toneup, int merge){
    
    if ((x>=0) && (y>=0)){
        for ( int i=x; i<this->width(); i++){
            int aux=0;
            for ( int j=y; j<this->height(); j++){
                if ( from.getPixel(i-x,aux)>toneup){
                    this->setPixel(i,j,(merge*from.getPixel(i-x,aux)+(100-merge)*getPixel(i,j)));
                }
                aux++;
            }
        }
    }
}