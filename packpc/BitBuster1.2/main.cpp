
//Bitbuster 1.2
//Compressor

#include "CCompress.h"


#define USEWINFIND


#ifdef USEWINFIND
#include <string>
#include <stdio.h>
#include <io.h>
#endif
#include <time.h>

using std::cout;
using std::endl;


#ifdef USEWINFIND
char* replace_char(char* str, char find, char replace)
{
	char* current_pos = strchr(str, find);

	while (current_pos)
	{
		*current_pos = replace;
		current_pos = strchr(current_pos, find);
	}

	return str;
}

char* concat(const char* s1, const char* s2)
{
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);
	char* result = (char*)malloc(len1 + len2 + 1);

	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1);

	return result;
}
#endif


int main(int argc, char* argv[])
{
	CCompress compressor;

#ifdef USEWINFIND
	_finddata_t search_result;
	intptr_t file_handle;
#endif
	clock_t start, finish;

	//give an error message if no arguments supplied
	if (argc == 1)
	{
		cout << "Nothing to compress!!";

		return 0;
	}

	start = clock();


	for (int argi = 1; argi < argc; ++argi)
	{
#ifdef USEWINFIND
		replace_char(argv[argi], '/', '\\');

		//search for first file matching argument
		file_handle = _findfirst(argv[argi], &search_result);

		//if no results
		if (file_handle == -1L)
		{	//display message
			cout << "File " << argv[argi] << " not found!" << endl;
		}
		else
		{
			do {
				//compress file if it's not a subdirectory
				if (!(search_result.attrib & _A_SUBDIR))
				{	//and file is not compressed already
					if (strstr(search_result.name, ".pck") == NULL)
					{
						char* lastSeparator = strrchr(argv[argi], '\\');
						
						if (lastSeparator)
						{
							*(lastSeparator+1) = 0;
							char* file = concat(argv[argi], search_result.name);

							int result = compressor.Start(file);

							free(file);

							if (result)
								return 1;
						}
						else
						{
							if (compressor.Start(search_result.name))
								return 1;
						}
					}
				}
				//search more files
			} while (_findnext(file_handle, &search_result) == 0);
		}

		//release resources used by search
		_findclose(file_handle);

#else
		if (compressor.Start(argv[argi]))
			return 1;
#endif
	}

	finish = clock();

	cout << "Time elapsed:" << (finish - start) / CLOCKS_PER_SEC << endl;

	return 0;
}
