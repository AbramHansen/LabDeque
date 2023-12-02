/***********************************************************************
 * Header:
 *    DEQUE
 * Summary:
 *    Our custom implementation of a deque
 *      __       ____       ____         __
 *     /  |    .'    '.   .'    '.   _  / /
 *     `| |   |  .--.  | |  .--.  | (_)/ /
 *      | |   | |    | | | |    | |   / / _
 *     _| |_  |  `--'  | |  `--'  |  / / (_)
 *    |_____|  '.____.'   '.____.'  /_/
 *
 *    This will contain the class definition of:
 *        deque                 : A class that represents a deque
 *        deque::iterator       : An iterator through a deque
 * Author
 *    Abram Hansen, Sam Heaven
 ************************************************************************/

#pragma once

// Debug stuff
#include <cassert>
#include <utility>

#include <iostream>

class TestDeque;    // forward declaration for TestDeque unit test class

namespace custom
{

/******************************************************
 * DEQUE
 *         iaFront
 * ia =   0   1   2   3   4
 *      +---+---+---+---+---+
 *      |   | A | B | C |   |
 *      +---+---+---+---+---+
 * id =       0   1   2
 * iaFront = 1
 * numElements = 3
 * numCapacity = 5
 *****************************************************/
template <class T>
class deque
{
   friend class ::TestDeque; // give unit tests access to the privates
public:

   // 
   // Construct
   //
   deque() 
   { 
      data = new T[1];
      numCapacity = 1;
      this->iaFront = 0;
      this->numElements = 0;
   }
   deque(int newCapacity);
   deque(const deque <T> & rhs);
   ~deque()
   { 
   }

   //
   // Assign
   //
   deque<T> & operator = (const deque <T> & rhs);

   //
   // Iterator
   //
   class iterator;
   iterator begin()
   {
      return iterator();
   }
   iterator end()
   {
      return iterator();
   }

   //
   // Access
   //
   T& front();
   T& back();
   const T & front() const;
   const T & back()  const;
   const T & operator[](size_t index) const;
   T& operator[](size_t index);

   // 
   // Insert
   //
   void push_front(const T& t);
   void push_back(const T& t);

   //
   // Remove
   //
   void clear()
   { 
      numElements = 0;
      iaFront = 0;
   }
   void pop_front();
   void pop_back();

   // 
   // Status
   //
   size_t size() const 
   { 
      return numElements; 
   }
   bool empty() const 
   { 
      return numElements == 0; 
   }

   
private:
   
   // fetch array index from the deque index
   int iaFromID(int id) const
   {
      int ia;
      //assert(0 <= id && id < numElements);
      //assert(0 <= iaFront && iaFront < numCapacity);
      ia = (id + iaFront) % numCapacity;
      //assert(0 <= ia && ia < numCapacity);
      return ia;
   }
   void resize(int newCapacity = 0);

   // member variables
   T * data;           // dynamically allocated data for the deque
   size_t numCapacity; // the size of the data array
   size_t numElements; // number of elements in the deque
   int iaFront;        // the index of the first item in the array
};



/**********************************************************
 * DEQUE ITERATOR
 * Forward and reverse iterator through a deque, just call
 *********************************************************/
template <typename T>
class deque <T> ::iterator
{
   friend class ::TestDeque; // give unit tests access to the privates
public:
   //
   // Construct
   //
   iterator()
   {
      this->pDeque = nullptr;
      id = 0;
   }
   iterator(custom::deque<T> *pDeque, int id)
   {
      this->pDeque = pDeque;
      this->id = id;
   }
   iterator(const iterator& rhs)
   {
      this->pDeque = rhs.pDeque;
      this->id = rhs.id;
   }

   //
   // Assign
   //
   iterator& operator = (const iterator& rhs)
   {
      this->pDeque = rhs.pDeque;
      this->id = rhs.id;
      return *this;
   }

   //
   // Compare
   //
   bool operator == (const iterator& rhs) const { return id == rhs.id; }
   bool operator != (const iterator& rhs) const { return id != rhs.id; }

   // 
   // Access
   //
   const T & operator * () const
   {
      return pDeque->data[pDeque->iaFromID(id)];
   }
   T& operator * () 
   {
      return pDeque->data[pDeque->iaFromID(id)];
   }

   // 
   // Arithmetic
   //
   int operator - (iterator it) const
   {
      return id - it.id;
   }
   iterator& operator += (int offset)
   {
      id += offset;
      return *this;
   }
   iterator& operator ++ ()
   {
      ++id;
      return *this;
   }
   iterator operator ++ (int postfix)
   {
      iterator temp(*this);
      ++id;
      return temp;
   }
   iterator& operator -- ()
   {
      --id;
      return *this;
   }
   iterator  operator -- (int postfix)
   {
      iterator temp(*this);
      --id;
      return temp;
   }

private:

   // Member variables
   int id;             // deque index
   deque<T> *pDeque;
};


/****************************************************
 * DEQUE : CONSTRUCTOR - non-default
 ***************************************************/
template <class T>
deque <T> :: deque(int newCapacity)
{
   data = new T[newCapacity];
   numCapacity = newCapacity;
   numElements = 0;
}

/****************************************************
 * DEQUE : CONSTRUCTOR - copy
 ***************************************************/
template <class T>
deque <T> :: deque(const deque <T> & rhs)
{
   this->data = rhs.data;
   this->numCapacity = rhs.numCapacity;
   this->numElements = rhs.numElements;
   this->iaFront = rhs.iaFront;
}


/****************************************************
 * DEQUE : ASSIGNMENT OPERATOR
 ***************************************************/
template <class T>
deque <T> & deque <T> :: operator = (const deque <T> & rhs)
{
   if (rhs.numElements > this->numCapacity)
      resize(rhs.numElements);

   for (int i = 0; i < rhs.numElements; ++i)
   {
      this->data[i] = rhs.data[rhs.iaFromID(i)];
   }

   this->numElements = rhs.numElements;
   this->iaFront = 0;

   return *this;
}

/**************************************************
 * DEQUE :: FRONT
 * Fetch the item that is at the beginning of the deque
 *************************************************/
template <class T>
const T & deque <T> :: front() const 
{
   return data[iaFront];
}
template <class T>
T& deque <T> ::front()
{
   return data[iaFront];
}

/**************************************************
 * DEQUE :: BACK
 * Fetch the item that is at the end of the deque
 *************************************************/
template <class T>
const T & deque <T> :: back() const 
{
   return data[iaFromID(numElements - 1)];
}
template <class T>
T& deque <T> ::back()
{
   return data[iaFromID(numElements - 1)];
}

/**************************************************
 * DEQUE :: SUBSCRIPT
 * Fetch the item in the deque
 *************************************************/
template <class T>
const T& deque <T> ::operator[](size_t index) const
{
   return data[iaFromID(index)];
}
template <class T>
T& deque <T> ::operator[](size_t index)
{
   return data[iaFromID(index)];
}

/*****************************************************
 * DEQUE : POP_BACK
 *****************************************************/
template <class T>
void deque <T> :: pop_back()
{
   numElements--;
}

/*****************************************************
 * DEQUE : POP_FRONT
 *****************************************************/
template <class T>
void deque <T> :: pop_front()
{
   numElements--;
   iaFront++;
   if (iaFront == numCapacity)
      iaFront = 0;
}

/******************************************************
 * DEQUE : PUSH_BACK
 ******************************************************/
template <class T>
void deque <T> :: push_back(const T & t) 
{
   if (numElements == numCapacity)
      resize(numCapacity * 2);
   data[iaFromID(numElements++)] = t;
}

/******************************************************
 * DEQUE : PUSH_FRONT
 ******************************************************/
template <class T>
void deque <T> :: push_front(const T & t) 
{
   if (numElements == numCapacity)
      resize(numCapacity * 2);
   iaFront--;
   if (iaFront < 0)
      iaFront = numCapacity - 1;
   data[iaFront] = t;
   numElements++;
}

/****************************************************
 * DEQUE :: RESIZE
 * Resize the deque so the numCapacity matches the newCapacity
 ***************************************************/
template <class T>
void deque<T>::resize(int newCapacity)
{
    T *newData = new T[newCapacity];

    // Copy elements to the new array
    for (size_t i = 0; i < numElements; ++i)
    {
        newData[i] = data[iaFromID(i)];
    }

    delete[] data;
    data = newData; 
    numCapacity = newCapacity;
    iaFront = 0; 

}

} // namespace custom
