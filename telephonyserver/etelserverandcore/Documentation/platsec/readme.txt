Generating PlatSec Caps Tables for ETel
=======================================


1.
==

Update the spreadsheet API_Capability_Assignment.xls, ensuring that at minimum "New IPC Number" and "Final Capabilities" columns are filled in for the correct API worksheet.


2.
==

Run the following command:

	GenerateCapabilityList.pl "<XLS_full_path>" etel

<XLS_full_path> = Full path name for the API_Capability_Assignment.xls file.


3.
==

This will generate a ".out" text file (e.g. API_Capability_Assignment.xls.out), which will contain all the information required to correctly update the following ETel source file:

	telephony\etel\SETEL\ET_SVR.CPP


Note:
=====

Please ensure the ETel PlatSec test suite is also updated by modifying the following file:

	telephony\etel\TETEL\CapTestFramework\EtelMessage.csv

Also add entries for the new IPC(s) to \telephony\etel\EtelRecorder\playback\inc\tipclookup.h

