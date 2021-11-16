#pragma once

#include <cassert>


namespace Util {
  

	template<typename T, size_t Capacity> 
	class FixedVector {
	public:


		void push_back(const T& t) {
			assert(numElements < Capacity);
			data[numElements++] = t;
		}


		size_t size() const {
			return numElements;
		}

		
		bool contains(const T& t) const {
			for( size_t i = 0; i < numElements; i++ ) {
				if( data[i] == t ) {
					return true;
				}
			}
			return false;
		}


		/**
		 * @warning Elements in the list are NOT destroyed (destructor is not called)!.
		 * 			It shouldn't be a problem for our use-case however
		 */
		void clear() {
			numElements = 0;
		}


		const T& operator[](size_t index) const {
			assert(index < numElements);
			return data[index];
		}


		T& operator[](size_t index) {
			assert(index < numElements);
			return data[index];
		}
		
		
	private:

		T data[Capacity];

        size_t numElements = 0;

	};

}