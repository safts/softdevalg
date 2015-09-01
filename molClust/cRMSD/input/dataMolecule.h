#ifndef __DATARMSD_H__
#define __DATARMSD_H__

#include "../dataStruct/data.h"


void* InitStructMolecule();

void DestroyStructMolecule(void** );

double MoleculeDistance(value*, value*);

int EqualMolecule(value *, value *);

void PrintMolecule(double );

void DestroyInputMolecule();

void DestroyValueMolecule(void* );

void ReadDataMolecule(value**, FILE* );

double CRMSD(value* ,value* );

double DRMSD(value* , value* );

double optRMSD(value* , value* );

#endif
