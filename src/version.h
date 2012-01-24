#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.01";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 2;
	static const long BUILD = 1744;
	static const long REVISION = 9671;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3376;
	#define RC_FILEVERSION 0,2,1744,9671
	#define RC_FILEVERSION_STRING "0, 2, 1744, 9671\0"
	static const char FULLVERSION_STRING[] = "0.2.1744.9671";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 44;
	

}
#endif //VERSION_H
