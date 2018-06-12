/**
 * @file hash.hpp
 * @version 1.0
 * @since Jun, 06.
 * @date May, 08.
 * @author Oziel Alves (ozielalves@ufrn.edu.br)
 * @author Max Willian (maxwilliam780@gmail.com)
 * @title The Hash Project
 */

#ifndef _HASH_H_
#define _HASH_H_

#include <math.h>
#include <string>
#include <vector>
#include <forward_list>
#include <iterator>
#include <iostream>
using size_type = size_t;

template <typename KeyType,
		  typename DataType,
		  typename KeyHash = std::hash<KeyType> ,
		  typename KeyEqual = std::equal_to<KeyType> >
class HashTable{

	class HashEntry{

		public:

			//! @brief HashEntry constructor.
			HashEntry(KeyType k_, DataType d_):m_key(k_),m_data(d_)

			{/*Empty*/};

			KeyType m_key; //!< Stores the key for an entry.

			DataType m_data;//!< Stores the data for an entry.
	};

	public:

		using Entry = HashEntry;

		//! @brief Hash Table constructor. 
		HashTable(void): tablesize(101),currentSize(0){ Lists.resize(101);}

		//! @brief Table constructor.
		explicit HashTable(size_type size): tablesize(next_prime(size)), currentSize(0)
		{Lists.resize(next_prime(size));}
		
		//void makeEmpty( );
		
		/** @brief Inserts Elements(data_item_) realated to a "key_" on the table.
		 *  @return True if the insertion was successful; False otherwise.
		 */ 
		bool insert(const KeyType & key_ , const DataType & data_item_ ){
		
			auto & whichList = Lists[ hashFunc(key_) % tablesize]; //!< Elements list
			Entry new_entry (key_, data_item_ ); //!< Take the function parameters as the Hash entry
			auto itr = whichList.begin(); //!< Iterator to the List's begin
			auto itr_b = whichList.before_begin(); //!< Iterator to the position before the list's begin 
			auto end = whichList.end(); //!< Iterator to the list's end
			for (; itr != end; ++itr) {
				
				itr_b++;
				if (true == equalFunc((*itr).m_key , new_entry.m_key)){
					return false;
				}
			}

			whichList.insert_after(itr_b, new_entry);
			//whichList.push_front(new_entry);

				if(++currentSize > tablesize ) 
					rehash( );

				return true;
		}

		/** @brief Recovers the information related to the "key_".
		 *  @return True if it finds the information; False otherwise.
		 */
		bool retrieve(const KeyType & key_,  DataType & data_item_) const{

			
			auto & whichList = Lists[ hashFunc(key_) % tablesize]; //!< Elements list
			for (auto i = whichList.begin(); i != whichList.end(); ++i) {
				if (true == equalFunc((*i).m_key , key_)){
					data_item_ = (*i).m_data;
					return true;
				}
			}
			return false;

		}
		
		//! @brief Prints the Hash Table.
		void print() const
		{
			if(!Lists.empty())
			{
				for (auto Lists_iter = Lists.begin(); Lists_iter != Lists.end(); ++Lists_iter) {
					if(!(*Lists_iter ).empty())
					for (auto j = (*Lists_iter ).begin(); j != (*Lists_iter ).end(); ++j) {
						std::cout << (*j).m_data;
					}
				}

			}
		} 

		//! @brief Clear all the memory related to the lists.
		void clear(){

			for(auto l : Lists){

				l.clear();
			}

		}
		
		//! @return True if the Hash Table is empty; False otherwise.
		bool empty ( void ) const{
			return currentSize == 0;
		}

		//! @return The curretn total of elements in the table.
		size_type count (void) const{
			return currentSize;
		}

		//! @return The total of elements a table supports.
		size_type capacity (void) const{
			return tablesize;
		}

		/** @brief Removes an specific element related to the "key_".
		 *  @return True if the key_ was found; False otherwise.
		 */
		bool remove(const KeyType & key_){

			auto & whichList = Lists[ hashFunc(key_) % tablesize]; //!<
			auto itr_back = whichList.before_begin(); //!<
			for (auto i = whichList.begin(); i != whichList.end(); ++i) {
		//		std::cout << *i << std::endl;
				if (true == equalFunc((*i).m_key , key_ )){
					whichList.erase_after(itr_back);
					--currentSize;

					return true;
				}
				itr_back++;
			}	
				return false;
		}	

	private:

		std::vector<std::forward_list<HashEntry>> Lists; //!< vector  with List of elements (collisions)
		
		size_type currentSize; //!< Total of elements in the table
		size_type tablesize; //!< Hash table current size
	
		// auxiliary functions
		KeyHash hashFunc; 
		KeyEqual equalFunc;

		//! @brief Helps the rehash finding the next prime number after the table size.
		size_type next_prime(size_type number){

			for (int i = 2; i < sqrt(number); ++i) {
				if(number % i == 0) return next_prime(number+1);
			}
			return number;
		}
		
		//! @brief Resizes the Hash Table.	
		void rehash(){

			//std::cout << "calls to rehash " << std::endl;
			std::vector<std::forward_list<HashEntry>> oldLists = Lists;
			
			tablesize =  next_prime( 2 * tablesize ); //!< Create new double-sized, empty table 
			
			Lists.resize(tablesize) ;
			currentSize = 0;
		
			for( auto & thisList : Lists )
				thisList.clear( ); 
			// Copy table over
			for( auto & copyLists : oldLists )
				for( auto & x : copyLists )
					insert(x.m_key, x.m_data);
		}	
};

#endif
