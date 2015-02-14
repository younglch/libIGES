/*
 * file: iges_entity.h
 *
 * Copyright 2015, Dr. Cirilo Bernardo (cirilo.bernardo@gmail.com)
 *
 * Description: Base entity of all IGES Entity classes.
 *
 * This file is part of libIGES.
 *
 * libIGES is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libIGES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef IGES_ENTITY_H
#define IGES_ENTITY_H

#include "iges_base.h"

class IGES; // Overarching data structure and parent to all entities

// NOTE: Base class for all IGES entities
class IGES_ENTITY
{
private:
    IGES_ENTITY_TYPE    entityType;         // #, Entity Type (only a select few values are allowed)
    int                 parameterData;      // P, first sequence number of associated parameterData
    int                 structure;          // 0P, index to DirEnt of the definition entity which specifies this entity's meaning
    int                 lineFontPattern;    // 0#P, 0 (def), Line font pattern number, or index to Line Font Definition (304)
    int                 level;              // 0#P, Level number for this entity, else index to Definition Levels Property (406-1)
    int                 view;               // 0P, 0 (def) or index to DirEnt for one of (a) View Entity (410) or (b) Views Visible Associativity Instance (402-3/4/19)
    int                 transform;          // 0P, 0 (def) or index to Transormation Matrix (124)
    int                 labelAssoc;         // 0P, 0 (def) or index to label Display Associativity (402-5)
    int                 statusNumber;       // #, Flags for Blank Status/Subordinate Entity Switch/Entity Use Flag/Hierarchy
    int                 lineWeightNum;      // #, System line width thickness, 0 .. Global::maxLinewidthGrad
    int                 colorNum;           // #P, 0 (def), Color ID, or index to Color Definition (314)
    int                 paramLineCount;     // #, number of associated Parameter Lines
    int                 form;               // 0#, 0 (def) or Form Number for entities with more than one form
    std::string         label;              // max. 8 character alphanumeric label
    int                 entitySubscript;    // #, 1..8 digit unsigned int associated with the label

    // pointers to be linked to other entities as necessary
    IGES_ENTITY* pStructure;
    IGES_ENTITY* pLineFontPattern;
    IGES_ENTITY* pLevel;
    IGES_ENTITY* pView;
    IGES_ENTITY* pTransform;
    IGES_ENTITY* pLabelAssoc;
    IGES_ENTITY* pColor;

    // XXX - Requires:
    // + list of parent pointers in order to manage reference counting
    //   related to the implementation of the StatusNumber

    // XXX - TO BE IMPLEMENTED
    // associate: associate pointers with other entities after reading all data; retrictions on types
    //            must be enforced to ensure data integrity and software stability
    // prepare(&index): prepare data for writing; Parameter Data is formatted using the given index;
    //                  each Entity must have been previously assigned a correct Sequence Number

protected:
    friend class IGES;
    int sequenceNumber;     // first sequence number of this Directory Entry

public:
    IGES_ENTITY(IGES* aParent);
    virtual ~IGES_ENTITY();

    // Add/DelReference is needed for management of StatusNumber
    virtual bool AddReference(IGES_ENTITY* aParentEntity) = 0;
    virtual bool DelReference(IGES_ENTITY* aParentEntity) = 0;

    // XXX - TO BE IMPLEMENTED
    // Read the DE data from the given records
    virtual bool ReadDE(IGES_RECORD* aRecord, std::ifstream& aFile) = 0;
    // virtual bool Read: read Parameter Data
    virtual bool ReadPD(std::ifstream& aFile) = 0;

    // virtual bool Write: write entity data
    // XXX - In preparation for a Write we need to perform a Cull() to destroy any orphan entities
    // then we need to re-enumerate [Reorder()] the DE and PD fields by traversing the Entity List and passing
    // a DE and PD accumulator to each in sequence.
    virtual bool WriteDE(std::ofstream& aFile) = 0;
    virtual bool WritePD(std::ofstream& aFile) = 0;

    // Set the parent object; this is required for operations such as 'Import'
    void SetParentIGES(IGES* aParent);
    // Retrieve the parent object
    IGES* GetParentIGES(void);

    IGES_ENTITY_TYPE GetEntityType(void);
    int              GetEntityForm(void);
    // Set the entity form. This can only succeed if a form has not yet been set;
    // some entities have only one form and default to that form on creation.
    virtual bool     SetEntityForm(int aForm) = 0;

    // Get/Set the Structure entity; this is unused in most entity types
    // and the defaults return false
    virtual bool GetStructure(IGES_ENTITY** aStructure);
    virtual bool SetStructure(IGES_ENTITY* aStructure);

    // Get/Set the LineFont Pattern; this may be an IGES_LINEFONT_PATTERN
    // or a LinefontPattern Entity
    virtual bool SetLineFontPattern(IGES_LINEFONT_PATTERN aPattern) = 0;
    virtual bool SetLineFontPattern(IGES_ENTITY* aPattern) = 0;
    // The Get functions return false if the argument does not match
    // the internal LineFontPattern type; in the case of type 0,
    // the GetLineFontPatternEntity() function sets the argument
    // to NULL and returns true.
    virtual bool GetLineFontPattern(IGES_LINEFONT_PATTERN& aPattern) = 0;
    bool         GetLineFontPatternEntity(IGES_ENTITY** aPattern);

    // Get/Set the Entity Level; this may be an int or a Definition Level Entity;
    // default level is 0
    virtual bool SetLevel(int aLevel) = 0;
    virtual bool SetLevel(IGES_ENTITY* aLevel) = 0;
    bool         GetLevel(int aLevel);
    bool         GetLevelEntity(IGES_ENTITY** aLevel);

    virtual bool SetView(IGES_ENTITY* aView) = 0;
    bool         GetView(IGES_ENTITY** aView);

    virtual bool SetTransform(IGES_ENTITY* aTransform) = 0;
    bool         GetTransform(IGES_ENTITY** aTransform);

    virtual bool SetLabelAssoc(IGES_ENTITY* aLabelAssoc) = 0;
    bool         GetLabelAssoc(IGES_ENTITY** aLabelAssoc);

    // Get/Set the entity Color; this may be an IGES_COLOR or a
    // Color Definition Entity
    virtual bool SetColor(IGES_COLOR aColor) = 0;
    virtual bool SetColor(IGES_ENTITY* aColor) = 0;
    bool          GetColor(IGES_COLOR& aColor);
    bool          GetColorEntity(IGES_ENTITY** aColor);

    virtual bool SetLineWeightNum(int aLineWeight) = 0;
    bool         GetLineWeightNum(int& aLineWeight) = 0;

    bool SetLabel(const std::string aLabel);
    void GetLabel(std::string& aLabel);

    bool SetEntitySubscript(int aSubscript);
    bool GetEntitySubscript(int& aSubscript);

    // StatusNum is a flag for 4 different things so we shall use separate Set/Get functions for:
    // Visibility: 0/1, 0 (default) = visible, 1 = invisible
    // Depedency: 0 (default, independent) to 3. See p.27/55+
    // EntityUse: 0..6 (Geometry, etc. See p.29/57+)
    // Hierarchy: int (0..99999), 0=default
    bool SetVisibility(bool isVisible);
    bool GetVisibility(bool& isVisible);

    virtual bool SetDependency(int aDependency) = 0;
    bool         GetDependency(int& aDependency);

    virtual bool SetEntityUse(int) = 0;
    bool         GetEntityUse(int);

    virtual bool SetHierarchy(int aHierarchy) = 0;
    bool         GetHierarchy(int& aHierarchy);
};

#endif  // IGES_ENTITY_H