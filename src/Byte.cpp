/**
 * @file Byte.cpp
 * @brief Operators for bit level
 * @note To be implemented by students 
 * @author MP-DGIM, MP-IADE, MP-II (grupo B)
 * @author Estudiante 1: Francisco Javier Checa Casas / Estudiante 2: Laura Riera Ojea
 */
#include <iostream>
#include <string>
#include "MPTools.h"
#include "Byte.h"

using namespace std;

const Byte Byte::MAX_BYTE_VALUE(255);
const Byte Byte::MIN_BYTE_VALUE(0);

int Byte::getValue() const{
    return(int) _data;
}

void Byte::setValue(unsigned char v){
    _data=v;
}

void Byte::onBit(int pos){
    _data |= (1 << pos);
}

void Byte::offBit(int pos) {
    _data &= ~(1 << pos);
}

bool Byte::getBit(int pos) const{
    unsigned char mask = 0x00;
    mask=mask<<pos;
    mask=mask&_data;
    
    if (_data == 0){
        return(false);
    }
    else{
        return(true);
    }
}

string Byte::to_string() const{
    string cadena="00000000";
    for(int i=0;i<=7;i++){
        if(Byte::getBit(i)==true){
            cadena[7-i]='1';
        }else{
            cadena[7-i]='0';
        }
    }
    return cadena;
}

void Byte::onByte(){
    _data=255;
}

void Byte::offByte(){
    _data=0;
}

void Byte::encodeByte(bool v[]){
    for(int i=MIN_BIT;i<MAX_BIT;i++){
        if(v[7-i]==true){
            Byte::onBit(i);
        }else{
            Byte::offBit(i);
        }
    }  
}

void Byte::decodeByte(bool v[]){
    for(int i=MIN_BIT;i<MAX_BIT;i++){
        if(Byte::getBit(i)==true){
            v[7-i]=true;
        }else{
            v[7-i]=false;
        }
    }
}

void Byte::decomposeByte(int posits[],int n){
    int j=0;
    for(int i=MIN_BIT;i<MAX_BIT;i++){
        if(Byte::getBit(i)==true){
            posits[j]=i;
            n++;
            j++;
        }
        
    }
}

void Byte::shiftRByte(int n){
    _data=_data>>n;
}

void Byte::shiftLByte(int n){
    _data=_data<<n;
}

void Byte::mergeByte(Byte merge, int percentage){
    percentage=percentage%101;
    _data= (_data*(100-percentage)+merge.getValue()*percentage)/100;
}