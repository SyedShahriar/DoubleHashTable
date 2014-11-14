#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

/*****************************************
 * UW User ID:  sgshahri
 * Submitted for ECE 250
 * Semester of Submission:  (Fall) 2014
 *
 * By submitting this file, I affirm that
 * I am the author of all modifications to
 * the provided code.
 *****************************************/

#include "Exception.h"
#include "ece250.h"

enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
private:
    int count;
    int power;
    int array_size;
    T *array;
    state *array_state;
    
    int h1( T const & ) const; // first hash function
    int h2( T const & ) const; // second hash function
    
public:
    DoubleHashTable( int = 5 );
    ~DoubleHashTable();
    int size() const;
    int capacity() const;
    bool empty() const;
    bool member( T const & ) const;
    T bin( int ) const;
    
    void print() const;
    
    void insert( T const & );
    bool remove( T const & );
    void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {
    
	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}
//Deletes the memory allocated for the hash table and state of elements stored in the table.

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {
    delete [] array;
    delete [] array_state;
}

//Returns the number of elements currently in the hash table

template<typename T >
int DoubleHashTable<T >::size() const {
    // enter your implemetation here
	//return 0;
    return count;
    
}

//Returns the number of bins or slots in the hash table

template<typename T >
int DoubleHashTable<T >::capacity() const {
    // enter your implemetation here
	//return 0;
    return array_size;
}


//Returns true if no elements are in the hash table, and false otherwise

template<typename T >
bool DoubleHashTable<T >::empty() const {
    // enter your implemetation here
	//return false;
    if (size()==0)
    {
        return true;
    }
    else{
        return false;
    }
}

//First hash function(the obj is statically cast as an int); if obj's value is negative it will take the modulo M of the obj, and then add M. Otherwise it will just take modulo M of the obj. Where M is equal to the array size.

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {
	// enter your implemetation here
    //return 0;
    
    int i = static_cast<int>(obj);
    
    if ((i%array_size)<0){
        
        return i%array_size + array_size;
    }
    else{
        return i%array_size;
    }
    
}

//Secondary hash function

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {
	
    //object statically cast as an integer
    
    int i = static_cast<int>(obj);
    
    //checks if the ((i/M)%M) returns a negative value. Then checks if ((i/M)%M) returns an even value, if so it adds one and M to the function ((i/M)%M) to make the value nonnegative and odd. Otherwise it just adds M to function ((i/M)%M) to make the value non negative.
    
    if((i/array_size)%array_size<0)
    {
        if (((i/array_size)%array_size)%2==0)
        {
            return (i/array_size)%array_size + array_size+ 1;
        }
        
        else
            return (i/array_size)%array_size + array_size;
    }
    
    //Just checks if ((i/M)%M) returns an even value, if so it adds one to the function ((i/M)%M) to make the value odd.
    else if (((i/array_size)%array_size)%2==0)
    {
            return (i/array_size)%array_size + 1;
        
    }
    
    //Just returns ((i/M)%M), which produces an odd postive value.
    else
    {
        
        return (i/array_size)%array_size;
    }
    
}

//Searches for a desired element in the hash table and returns true if its in the hash table, otherwise it returns false.
template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {
	
 //instantiating the hash functions
    int probe = h1(obj);
    int offset = h2(obj);
    int k = 0;
    
    //Checking if the hash table is empty
    if (empty())
        return false;
    
    //Traversing through the hash table using the hash functions, to see if the desired element exists in the hash table. Only loops the double hash function if the calculated probe is occupied
    while (array_state[probe]==OCCUPIED)
    {
        
        //if every bin the table has been checked then, then it should return false
        if(k==array_size)
            return false;
        
        //If the state at the probe is occupied and it is equal to the object, then we have found our desired value
        if (array_state[probe]==OCCUPIED && array[probe]== obj )
        {
            return true;
        }
        //Double hashing function
        probe = (probe + offset)%array_size;
        k++;

    }
    
    return false;
   
}

//Returns the value at the desired bin of the hash table. The function is undefined if that bin is not filled.

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {
    // enter your implemetation here
    return array[n];
}

//Inserts the object into the hash table

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {
    
    //Throws an overflow exception if the hash table is full
    if(size()== array_size)
    {
        throw overflow();
    }
    
    int probe = h1(obj);
    int offset = h2(obj);
    
    //Traverses through the table only while the state of bin at the probe is occupied. When an empty bin is found the object is inserted into the hash table.
    
    while (array_state[probe]==OCCUPIED)
    {
        probe = (probe + offset)%array_size;
    }
        array[probe]= obj;
        array_state[probe]= OCCUPIED;
        
        count++;
   
    
}

//Searches for and removes a desired object from the hash table by changing its state to deleted, and returns true. If object is not found, it returns false.
template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {
    
    int probe = h1(obj);
    int offset = h2(obj);
    
    //Traverses through the hash table using the double hash function. The loop ends when every single bin has been probed by the double hash function.
        for (int k=0;k<array_size;k++)
        {
            int iprobe = (probe + k*offset)%array_size;
    
            //Deletes desired object when the object at the probe is found, and when its state is occupied
            if (array[iprobe]==obj && array_state[iprobe]==OCCUPIED)
            {
                array_state[iprobe]=DELETED;
                count--;
                return true;
            }
        
        }
    
    return false;
}

//Clears the hash table, by changing the state of each bin to empty
template<typename T >
void DoubleHashTable<T >::clear() {
   
    for ( int i=0; i<array_size; i++)
    {
        array_state[i]= EMPTY;
       
    }
    count=0;
}

template<typename T >
void DoubleHashTable<T >::print() const {

	for (int i=0; i<array_size; i++)
	{
        std::cout<<"|"<<array[i];
    }
        std::cout<<"|"<<std::endl;
    for (int j=0; j<array_size; j++)
	{
        std::cout<<"|"<<array_state[j];
    }
    std::cout<<"|";
}

#endif
