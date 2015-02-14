/*
 * file: entity180.h
 *
 * Copyright 2015, Dr. Cirilo Bernardo (cirilo.bernardo@gmail.com)
 *
 * Description: IGES Entity 180: Boolean Tree, Section 4.46, p.209(237+)
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

#ifndef ENTITY_180_H
#define ENTITY_180_H

#include "iges_entity.h"

// NOTE:
// The boolean operators are in Postfix Notation. Examples:
// + DE1, DE2, DIFFERENCE,
// + DE1, DE2, DIFFERENCE, DE3, DIFFERENCE, DE4, DIFFERENCE (A board with holes can be built this way)
// Since the operators are BOOLEAN, if M is the number of Operands, the number of Operators must be M - 1.
// The specification implies a stack of operands and operators; it should be possible to parse these into
// a list of some sort.
//
// The associated parameter data are:
// + N: Number: Number of items (DE pointers + Operators)
// + LIST: List of DEs (negative) and Operators (1:Union, 2:Intersection, 3:Difference)
//
// Forms:
//  0: All operands are Primitives, Solid Instances, or other Boolean Trees
//  1: At least one operand is a Manifold BREP Object
//
// Unused DE items:
// + Structure
//
// Access functions:
// Use Push(pointer/integer) and Pop() to manipulate the list; provide an iterator.
// The Push(pointer) function must check the type of data pushed and change the Form
// to 1 if necessary.
//

class IGES_ENTITY_180 : IGES_ENTITY
{
private:
    // XXX - TO BE IMPLEMENTED

public:
    // XXX - TO BE IMPLEMENTED

}

#endif  // ENTITY_180_H