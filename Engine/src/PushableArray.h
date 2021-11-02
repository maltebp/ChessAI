#pragma once

#include <cassert>


namespace Util {
  
  
	template<typename T> 
	class PushableArray {
	public:


		void push_back(const T& t) {
			assert(numElements < capacity());
			data()[numElements] = t;
			numElements++;
		}


        const T& operator[](size_t index) const {
			assert(index < numElements);
			return data()[index];
		}


		T& operator[](size_t index) {
			assert(index < numElements);
			return data()[index];
		}

		
		size_t size() const {
			return numElements;
		}
		

		virtual size_t capacity() const = 0;


	protected:

		virtual T* data() = 0;

	private:

        size_t numElements = 0;

	};


	
	template<typename T, size_t Capacity>
	class SizedPushableArray : public PushableArray<T> {
	public:

		size_t capacity() const override {
			return Capacity;
		}

	protected:

		T* data() override {
			return arr;
		}
    
    private:

        T arr[Capacity];

	};
}