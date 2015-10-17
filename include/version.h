#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "28";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2012";
	static const char UBUNTU_VERSION_STYLE[] = "12.06";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 0;
	static const long MINOR = 3;
	static const long BUILD = 1871;
	static const long REVISION = 10379;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3585;
	#define RC_FILEVERSION 0,3,1871,10379
	#define RC_FILEVERSION_STRING "0, 3, 1871, 10379\0"
	static const char FULLVERSION_STRING[] = "0.3.1871.10379";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 71;
	

}
#endif //VERSION_H
