//
// This file is a part of pomerol - a scientific ED code for obtaining 
// properties of a Hubbard model on a finite-size lattice 
//
// Copyright (C) 2010-2011 Andrey Antipov <antipov@ct-qmc.org>
// Copyright (C) 2010-2011 Igor Krivenko <igor@shg.ru>
//
// pomerol is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// pomerol is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pomerol.  If not, see <http://www.gnu.org/licenses/>.


/** \file src/StatesClassification.h
** \brief Declaration of BlockNumber, QuantumNumbers and StatesClassification classes
**
** \author Andrey Antipov (antipov@ct-qmc.org)
** \author Mikhail Aleynikov (alejnikov89@mail.ru)
** \author Igor Krivenko (igor@shg.ru)
*/


#ifndef __INCLUDE_STATESCLASSIFICATION_H
#define __INCLUDE_STATESCLASSIFICATION_H

#include "Misc.h"
#include "HDF5Storage.h"
#include "Index.h"
#include "IndexClassification.h"
#include "Symmetrizer.h"

namespace Pomerol{

extern struct QuantumNumbers;
/** All blocks with current QuantumNumber are treated as non-existent */
extern const QuantumNumbers ERROR_QUANTUM_NUMBERS;

struct BlockNumber;


/** InnerFockState labels the states inside of the block. Has no physical meaning. */
typedef unsigned long int InnerFockState;

/** This class handles all information about Fock states. 
 *  It makes a classification of Fock states into blocks.
 */
class StatesClassification {
    
    /** Total number of states = 2^(IndexInfo.size()) */
    FockState StateSize;            
    /** Total number of modes of the system. Equal to IndexInfo.size() */
    ParticleIndex IndexSize;                

    /** A map between all BlockNumbers and QuantumNumbers */
    std::map<BlockNumber,QuantumNumbers> BlockToQuantum;
    /** A map between all QuantumNumbers and BlockNumbers */
    std::map<QuantumNumbers,BlockNumber> QuantumToBlock;

    /** A reference to an IndexClassification object */
    const IndexClassification &IndexInfo;
    /** A reference to a Symmetrizer object. This will be used for classification of the states. */
    const Symmetrizer &Symm;
public:        
    /** Constructor
     * \param[in] IndexInfo A reference to an IndexClassification object
     */
    StatesClassification(const IndexClassification& IndexInfo, const Symmetrizer &Symm):IndexInfo(IndexInfo) {};

    /** Perform a classification of all FockStates */
    void compute();

    /** get total number of Quantum States ( 2^IndexInfo.size() ) */
    const FockState getNumberOfStates() const;

    /** get a vector of all FockStates with a given set of QuantumNumbers
     * \param[in] in A set of quantum numbers to get a vector of FockStates 
     */
    const std::vector<FockState>& getFockStates( QuantumNumbers in ) const;

    /** get a FockState, corresponding to an internal InnerFockState
     * \param[in] QuantumNumbers of block in which the InnerFockState is located
     * \param[in] m InnerFockState for which the correspondence is required
     */
    const FockState getFockState( QuantumNumbers in, InnerFockState m) const;
    /** get InnerFockState of a given FockState. Since FockState is associated with
     * the Block number no explicit BlockNumber or QuantumNumbers is required 
     * \param[in] state FockState for which the correspondence is required
     */
    const InnerFockState getInnerState( FockState state) const;

    /** Returns a number of Block which corresponds to given Quantum Numbers 
     * \param[in] in A set of QuantumNumbers to find corresponding BlockNumber
     */
    BlockNumber getBlockNumber(QuantumNumbers in) const;

    /** Returns QuantumNumbers for a given BlockNumber
     * \param[in] in A BlockNumber to find a set of corresponding QuantumNumbers
     */
    QuantumNumbers getBlockInfo(BlockNumber in) const;
    /** Returns total amount of non-vanishing blocks */
    BlockNumber NumberOfBlocks() const;

    /** Returns QuantumNumbers of a given FockState 
     * \param[in] in A FockState for which the QuantumNumbers are requested
     */
    QuantumNumbers getStateInfo(FockState in) const;
    /** Returns BlockNumber of a given FockState 
     * \param[in] in A FockState for which the BlockNumber is requested
     */
    BlockNumber getBlockNumber(FockState in) const;

    /** Checks that a block with a given QuantumNumbers does not vanish 
     * \param[in] in A set of QuantumNumbers to check
     */
    bool checkQuantumNumbers(QuantumNumbers in) const;
};


/** This class represents a number of a current block of FockStates which have
 * the same quantum numbers. If such block can't exist ( can't be created by anyone else )
 * it's value is assigned to an ERROR_BLOCK_NUMBER.
 * The classification of blocks is now done by StatesClassification class
 */
struct BlockNumber {
    /** The number of current block */
    int number;
    /** Empty constructor */
    BlockNumber(){};
    /** Copy constructor from int number
     * \param[in] number A number to copy from 
     */
    BlockNumber(int number):number(number){};
    /** Type conversion to integer */
    operator int() const {return number;}
    /** post-increment operator */
    BlockNumber& operator ++(int unused){number++; return *this;}
    /** Returns true if such block exists */
    bool isCorrect(){return number >= 0;}
    /** Operator < */
    bool operator<(const BlockNumber& rhs) const ;
    /** Operator > */
    bool operator==(const BlockNumber& rhs) const;
};

/** All blocks with this number are treated as nonexistent */
const BlockNumber ERROR_BLOCK_NUMBER = -1;



} // end of namespace Pomerol
#endif // endif :: #ifndef __INCLUDE_STATESCLASSIFICATION_H
