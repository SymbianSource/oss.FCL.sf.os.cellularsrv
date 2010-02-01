The script generate_new_component_tests.pl can be used to generate new component tests to test the Common TSY.

It will generate the correct test tags, boiler place TEFUnit code and test scripts which can be pasted into the existing
component test suite.

To use the script:

1. Open the spreadsheet "New_CTSY_Component_Tests.xls". Read the worksheet called "HowToUseThisSheet" and follow the
instructions carefully to fill in the spreadhsheet.

2. Close Excel (this step must be follow for the script to work). 

At the command prompt type:

perl generate_new_component_tests.pl <FULL path to the Excel spreadsheet>

A directory called 'test' will be created in the current directory. Inside this directory are directories which
mirror those in the component test suite. Sections of code (rather than the whole from) from the files in this 
directory can be copy and pasted into the file of the same name in the existing test suite:

group
inc		- This contains the test header files
		  The test step function declarations can be copy and pasted into the corresponding header file		
		  in the existing test suite.

src		- This contains the test src files
		  The ADD_TEST_STEP macros and all the code under "Actual test cases"
		  the generated can be copy and pasted into the corresponding source file. These are the test cases.
		
scripts		- This contains the test scripts
		  Copy and paste the test script code and add them to the existing test scripts.
		  Note that the timeout tests are all kept together in a separate timeout script file 
		  te_ctsycomponent_timeout_all.script as these take a long time to run.

3.  		  After the code has been generated, remove what has been added to the "NewApis" worksheet.
		  Ensure that the new New_CTSY_Component_Tests.xls is submitted into Perforce. 

IMPORTANT!

Check that the sheet has not been modified by another user as XLS files cannot be merged easily!
Do NOT submit the generated boilerplate code in into P4!

		  
