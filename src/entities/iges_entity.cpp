/*
 * file: iges_entity.cpp
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

// Note: This base class must never be instantiated.

#include <error_macros.h>
#include <iges_entity.h>

IGES_ENTITY::IGES_ENTITY(IGES* aParent)
{
    // master IGES object; contains globals and manages entity I/O
    parent = aParent;

    // first sequence number of this Directory Entry (0: default = invalid)
    sequenceNumber = 0;

    // Entity Type, default = NULL Entity
    entityType = ENT_NULL;

    // first sequence number of associated parameterData (0: default = invalid)
    parameterData = 0;

    // index to DirEnt of the definition entity which specifies this entity's meaning
    structure = 0;

    // line font pattern number, or index to Line Font Definition (304), default = 0
    lineFontPattern = 0;

    // level number for this entity, or index to Definition Levels Property (406-1), default = 0
    level = 0;

    // 0 (default) or index to DirEnt for one of (a) View Entity (410)
    // or (b) Views Visible Associativity Instance (402-3/4/19)
    view = 0;

    // 0 (default) or index to Transormation Matrix (124)
    transform = 0;

    // 0 (default) or index to label Display Associativity (402-5)
    labelAssoc = 0;

    // Status Number: Blank Status (default 0: visible == true)
    visible = true;

    // Status Number: Subordinate Entity Switch (default 0 = independent)
    depends = STAT_INDEPENDENT;

    // Status Number: Entity Use (default 0 = Geometry)
    use = STAT_USE_GEOMETRY;

    // Status Number: Hierarchy (default 0 = all DE attributes apply to subordinates)
    hierarchy = STAT_HIER_ALL_SUB;

    // system line width thickness, 0 .. Global::maxLinewidthGrad
    // (default: 0 = use Receiving System's default)
    lineWeightNum = 1;

    // 0 (default = NO COLOR), Color ID (IGES_COLOR), or index to Color Definition (314)
    colorNum = 0;

    // number of associated Parameter Lines, 0 = invalid
    paramLineCount = 0;

    // 0 or Form Number for entities with more than one form
    form = -1;

    // 1..8 digit unsigned int associated with the label
    entitySubscript = 0;

    // pointers which may be linked to other entities:
    pStructure = NULL;
    pLineFontPattern = NULL;
    pLevel = NULL;
    pView = NULL;
    pTransform = NULL;
    pLabelAssoc = NULL;
    pColor = NULL;

    return;
}   // IGES_ENTITY::IGES_ENTITY(IGES*)


IGES_ENTITY::~IGES_ENTITY()
{
    // XXX - TO BE IMPLEMENTED
}   // IGES_ENTITY::~IGES_ENTITY()


bool IGES_ENTITY::SetParentIGES(IGES* aParent)
{
    if( !aParent)
        return false;

    parent = aParent;
    return true;
}


IGES* IGES_ENTITY::GetParentIGES(void)
{
    return parent;
}


size_t IGES_ENTITY::GetNRefs(void)
{
    return refs.size();
}


IGES_ENTITY_TYPE IGES_ENTITY::GetEntityType(void)
{
    return entityType;
}


int IGES_ENTITY::GetEntityForm(void)
{
    return form;
}


bool SetStructure(IGES_ENTITY* aStructure)
{
    return false;
}


bool GetStructure(IGES_ENTITY** aStructure)
{
    *aStructure = NULL;
    return false;
}


bool IGES_ENTITY::GetLineFontPattern(IGES_LINEFONT_PATTERN& aPattern)
{
    if( lineFontPattern & 0x80000000 )
    {
        aPattern = LINEFONT_NONE;
        return false;
    }

    aPattern = (IGES_LINEFONT_PATTERN)lineFontPattern;
    return true;
}


bool IGES_ENTITY::GetLineFontPatternEntity(IGES_ENTITY** aPattern)
{
    *aPattern = NULL;

    if( 0 == (lineFontPattern & 0x80000000) )
    {
        if( 0 == lineFontPattern )
            return true;
        else
            return false;

    }

    if( pLineFontPattern == NULL )
    {
        ERRMSG << "\n + [BAD DATA] invalid line pattern\n";
        return false;
    }

    *aPattern = pLineFontPattern;
    return true;
}


bool IGES_ENTITY::GetLevel(int aLevel)
{
    if( level & 0x80000000 )
    {
        aLevel = 0;
        return false;
    }

    aLevel = level;
    return true;
}


bool IGES_ENTITY::GetLevelEntity(IGES_ENTITY** aLevel)
{
    *aLevel = NULL;

    if( 0 == (level & 0x80000000) )
    {
        if( level == 0 )
            return true;
        else
            return false;

    }

    if( !pLevel )
    {
        ERRMSG << "\n + [BAD DATA] invalid level pointer\n";
        return false;
    }

    *aLevel = pLevel;
    return true;
}


bool IGES_ENTITY::GetView(IGES_ENTITY** aView)
{
    *aView = NULL;

    if( 0 == (view & 0x80000000) )
        return true;

    if( !pView )
    {
        ERRMSG << "\n + [BAD DATA] invalid view pointer\n";
        return false;
    }

    *aView = pView;
    return true;
}


bool IGES_ENTITY::GetTransform(IGES_ENTITY** aTransform)
{
    *aTransform = NULL;

    if( 0 == (transform & 0x80000000) )
        return true;

    if( !pTransform )
    {
        ERRMSG << "\n + [BAD DATA] invalid transform pointer\n";
        return false;
    }

    *aTransform = pTransform;
    return true;
}


bool IGES_ENTITY::GetLabelAssoc(IGES_ENTITY** aLabelAssoc)
{
    *aLabelAssoc = NULL;

    if( 0 == (labelAssoc & 0x80000000) )
        return true;

    if( !pLabelAssoc )
    {
        ERRMSG << "\n + [BAD DATA] invalid Label Association pointer\n";
        return false;
    }

    *aLabelAssoc = pLabelAssoc;
    return true;
}


bool IGES_ENTITY::GetColor(IGES_COLOR& aColor)
{
    if( colorNum & 0x80000000 )
    {
        aColor = COLOR_NONE;
        return false;
    }

    aColor = (IGES_COLOR)colorNum;
    return true;
}


bool IGES_ENTITY::GetColorEntity(IGES_ENTITY** aColor)
{
    *aColor = NULL;

    if( 0 == (colorNum & 0x80000000) )
    {
        if( 0 == colorNum )
            return true;
        else
            return false;

    }

    if( pColor == NULL )
    {
        ERRMSG << "\n + [BAD DATA] invalid color entity\n";
        return false;
    }

    *aColor = pColor;
    return true;
}


bool IGES_ENTITY::GetLineWeightNum(int& aLineWeight)
{
    aLineWeight = lineWeightNum;
    return true;
}


bool IGES_ENTITY::SetLabel(const std::string aLabel)
{
    label = aLabel.substr(0, 8);
    return true;
}


void IGES_ENTITY::GetLabel(std::string& aLabel)
{
    aLabel = label;
    return;
}


bool IGES_ENTITY::SetEntitySubscript(int aSubscript)
{
    if( aSubscript >= 0 && aSubscript <= 99999999 )
    {
        entitySubscript = aSubscript;
        return true;
    }

    return false;
}


bool IGES_ENTITY::GetEntitySubscript(int& aSubscript)
{
    aSubscript = entitySubscript;
    return true;
}


bool IGES_ENTITY::SetVisibility(bool isVisible)
{
    visible = isVisible;
    return true;
}


bool IGES_ENTITY::GetVisibility(bool& isVisible)
{
    isVisible = visible;
    return true;
}


bool IGES_ENTITY::GetDependency(IGES_STAT_DEPENDS& aDependency)
{
    aDependency = depends;
    return true;
}


bool IGES_ENTITY::GetEntityUse(IGES_STAT_USE& aUseCase)
{
    aUseCase = use;
    return true;
}


bool IGES_ENTITY::GetHierarchy(IGES_STAT_HIER& aHierarchy)
{
    aHierarchy = hierarchy;
    return true;
}
