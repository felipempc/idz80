#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "11";
	static const char YEAR[] = "2011";
	static const char UBUNTU_VERSION_STYLE[] = "11.11";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 1;
	static const long BUILD = 1699;
	static const long REVISION = 9454;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3287;
	#define RC_FILEVERSION 0,1,1699,9454
	#define RC_FILEVERSION_STRING "0, 1, 1699, 9454\0"
	static const char FULLVERSION_STRING[] = "0.1.1699.9454";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 99;
	

}
#endif //VERSION_H
