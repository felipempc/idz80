#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "12";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.03";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 3;
	static const long BUILD = 1843;
	static const long REVISION = 10227;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3549;
	#define RC_FILEVERSION 0,3,1843,10227
	#define RC_FILEVERSION_STRING "0, 3, 1843, 10227\0"
	static const char FULLVERSION_STRING[] = "0.3.1843.10227";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 43;
	

}
#endif //VERSION_H
