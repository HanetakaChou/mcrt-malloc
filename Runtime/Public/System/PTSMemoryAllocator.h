#ifndef PT_SYSTEM_MEMORYALLOCATOR_H
#define PT_SYSTEM_MEMORYALLOCATOR_H

#include "../PTCommon.h"
#include "PTSCommon.h"

extern "C" PTSYSTEMAPI PTBOOL PTCALL PTSMemoryAllocator_Initialize();

extern "C" PTSYSTEMAPI void * PTCALL PTSMemoryAllocator_Alloc(uint32_t Size);

extern "C" PTSYSTEMAPI void PTCALL PTSMemoryAllocator_Free(void *pVoid);

extern "C" PTSYSTEMAPI void * PTCALL PTSMemoryAllocator_Realloc(void *pVoid, uint32_t Size);

extern "C" PTSYSTEMAPI void * PTCALL PTSMemoryAllocator_Alloc_Aligned(uint32_t Size, uint32_t Alignment);

extern "C" PTSYSTEMAPI void PTCALL PTSMemoryAllocator_Free_Aligned(void *pVoid);

#include <memory>
#include <new>

template<typename T>
class PTSCPP_allocator 
{
public:
	typedef typename std::allocator<T>::value_type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	template<class U> 
	struct rebind 
	{
		typedef PTSCPP_allocator<U> other;
	};

	inline PTSCPP_allocator() throw() 
	{

	}

	inline PTSCPP_allocator(const PTSCPP_allocator&) throw() 
	{

	}

	template<typename U> 
	inline PTSCPP_allocator(const PTSCPP_allocator<U>&) throw()
	{

	}

	inline pointer address(reference x) const 
	{ 
		return &x;
	}

	inline const_pointer address(const_reference x) const
	{
		return &x; 
	}

	inline pointer allocate(size_type n, const void * = 0)
	{
		pointer p = static_cast<pointer>(::PTSMemoryAllocator_Alloc(static_cast<uint32_t>(sizeof(value_type)*n)));
		if (!p)
			throw std::bad_alloc();
		return p;
	}

	inline void deallocate(pointer p, size_type)
	{
		::PTSMemoryAllocator_Free(p);
	}

	inline size_type max_size() const throw()
	{
		size_type absolutemax = static_cast<size_type>(-1) / sizeof(value_type);
		return (absolutemax > 0 ? absolutemax : 1);
	}

	inline void construct(pointer p, const value_type& value)
	{ 
		::new((void*)(p)) value_type(value); 
	}
	
	void destroy(pointer p) 
	{ 
		p->~value_type(); 
	}
};

template<>
class PTSCPP_allocator<void> {
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	typedef void value_type;
	template<class U> 
	struct rebind 
	{
		typedef PTSCPP_allocator<U> other;
	};
};

template<typename T, typename U>
inline bool operator==(const PTSCPP_allocator<T>&, const PTSCPP_allocator<U>&)
{ 
	return true;
}

template<typename T, typename U>
inline bool operator!=(const PTSCPP_allocator<T>&, const PTSCPP_allocator<U>&) 
{ 
	return false;
}

#endif