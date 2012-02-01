#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.02";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 3;
	static const long BUILD = 1839;
	static const long REVISION = 10205;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3530;
	#define RC_FILEVERSION 0,3,1839,10205
	#define RC_FILEVERSION_STRING "0, 3, 1839, 10205\0"
	static const char FULLVERSION_STRING[] = "0.3.1839.10205";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 39;
	

}
#endif //VERSION_H
