// Copyright (c) Scott Bilas, 2000. All rights reserved worldwide.
//
// This software is provided "as is" without express or implied
// warranties. You may freely copy and compile this source into
// applications you distribute provided that the copyright text below
// is included in the resulting source code, for example:
// "Portions Copyright (c) Scott Bilas, 2000"

#ifndef COMRADE_CORELIB_SINGLETON
#define COMRADE_CORELIB_SINGLETON

#include <cassert>

using namespace std;

namespace Comrade 
{

namespace Corelib
{

/** \brief A singleton class template.
 *
 * This singleton class template is an easy and safe method to provide access to
 * a C++ class singleton while retaining control over when it is instantiated and
 * destroyed.
 *
 * To make a class a singleton, follow these steps:
 *	- Publicly derive your class \c MyClass from \c Singleton \c &lt;MyClass&gt;
 *	- Make sure that you&apos;re constructing an instance of \c MyClass somewhere in the 
 *	  system before using it. How you instantiate it doesn&apos;t matter. You can let
 *	  the compiler worry about it by making it a global or local static, or you 
 *	  can worry about it yourself via new and delete through an owner class.
 *	  Regardless of how and when you construct the instance, it will get tracked
 *	  and may be used as a singleton through a common interface by the rest of the system.
 *	- Call \c MyClass::get_singleton() to use the object from anywhere in the system.
 */
 
template <typename T> class Singleton
{
	private:
		static T* 	ms_Singleton;
	public:
		Singleton()
		{
			assert(!ms_Singleton);
			ms_Singleton = static_cast <T*> (this);
		}
		~Singleton()
		{
			assert(ms_Singleton);
			ms_Singleton = 0;
		}
		static T& get_singleton()
		{
			assert(ms_Singleton);
			return *ms_Singleton;
		}
		static T* get_singleton_ptr()
		{
			return ms_Singleton;
		}
};

template <typename T> T* Singleton <T>::ms_Singleton = 0;

} // namespace Corelib

} // namespace Comrade

#endif
