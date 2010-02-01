#!/usr/bin/perl -w

# Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
# This script generates TEFUnit boiler plate code for testing the CTSY.
# 
#

use Data::Dumper;
use File::Copy;
use Cwd;
use Win32::OLE;
use strict;

my $usage = "Usage: $0 <full path to Excel worksheet containing new API info. Type $0 help for more information.\n";
@ARGV == 1 or die $usage;

my $help=<<HELP

When new APIs are added to the CTSY, component tests of a certain format
need to be created to test them. This script is used to generate skeleton
CTSY component test code.

Read the readme.txt file for instructions on how to use this script.

HELP
;

die $help if $ARGV[0] eq "help";

#######################################################################
# Build directory structure
#######################################################################

die 'ERROR: The script will create a directory called "test", delete the copy of test that already exists here to be able to run the script.' if (-d 'test');

mkdir ('test', "0777");
#mkdir ('test/common', "0777");
#mkdir ('test/common/inc', "0777");
#mkdir ('test/common/src', "0777");
mkdir ('test/component', "0777");
mkdir ('test/component/scripts', "0777");
mkdir ('test/component/group', "0777");
mkdir ('test/component/inc', "0777");
mkdir ('test/component/src', "0777");

#######################################################################
# Global variables and functions used for each generation step
#######################################################################

my %func_units;     # Key->Value = Func. unit name->Func. unit code
my %apis;           # Key->Value = Func. unit name->Array of APIs
					# to be test in that func. unit test suite
my $component_test_suite_name = 'te_ctsy_component';
my %total_num_tests; # Count of total number of tests broken down by Functional Unit
					 # Key->Value = Func. unit name->Num tests

#######################################################################
# Subroutines
#######################################################################

# Returns the test class name given the functional unit name
sub get_test_class_name
{
	my ($func_unit) = shift;    # First argument is functional unit name.
	return "CCTsy${func_unit}FU";
}

# Returns the current year.
sub get_current_year
{
	my @thisyear = gmtime;
	return ($thisyear[5]+1900);
}

# Returns the copyright notice.
sub get_copyright_notice
{
	my $filename = shift; # 1st argument = filename
	my $copyright =<<COPYRIGHT
//
// <<FILENAME>>
//
// Copyright (c) Symbian Software Ltd. <<CURRENT_YEAR>>.  All rights reserved.
//

COPYRIGHT
;
	$filename = lc($filename); # Make sure filename is lowercase.
	$copyright =~ s/<<FILENAME>>/$filename/;
	my $year = get_current_year;
	$copyright =~ s/<<CURRENT_YEAR>>/$year/;
	return $copyright;
}

# Return the test description given the API name and the test number
# Tests are:
# Test 1 - Test the API
# Test 2 - Test cancel
# Test 3 - Test bad parameter data
# Test 4 - Test multiple completion for notifier APIs
# Test 5 - Timeout test
sub get_test_description
{
	my $api = shift; # 1st arg. = api name
	my $j = shift; # 2nd arg. = test number
	my $test_desc = 'Test support in CTSY for ';
	$test_desc .= "$api" if $j == 1;
	$test_desc .= "cancelling of $api" if $j == 2;
	$test_desc .= "$api with bad parameter data" if $j == 3;
	$test_desc .= "multiple client requests to $api" if $j == 4;
	$test_desc .= "$api with timeout" if $j == 5;
	return $test_desc;
}

# Just like sub get_test_description but with a different sentence
# This sentence is used in the test tags
sub get_test_actions
{
	my $api = shift; # 1st arg. = api name
	my $j = shift; # 2nd arg. = test number
	my $test_actions = 'Invokes ';
	$test_actions .= "$api" if $j == 1;
	$test_actions .= "cancelling of $api" if $j == 2;
	$test_actions .= "$api with bad parameter data" if $j == 3;
	$test_actions .= "multiple client requests to $api" if $j == 4;
	$test_actions .= "$api and tests for timeout" if $j == 5;
	return $test_actions;
}

# Get extra contexts
# These extra phrases are used in the test tags
sub get_extra_phrase
{
	my $class = shift; # The class the API is in
	my $set_num = shift; # Set number (e.g voice, data, fax for calls and lines)
	#my $test_num = shift; # The test number
	my $phrase = '';
	if ($class =~ /^(RCall|RMobileCall)$/)
	{
		$phrase .= ' for voice calls' if $set_num == 1;
		$phrase .= ' for data calls' if $set_num == 2;
		$phrase .= ' for fax calls' if $set_num == 3;
	}
	elsif ($class =~ /^(RLine|RMobileLine)$/)
	{
		$phrase .= ' for voice lines' if $set_num == 1;
		$phrase .= ' for data lines' if $set_num == 2;
		$phrase .= ' for fax lines' if $set_num == 3;
	}
	elsif ($class eq 'RMobilePhoneBookStore')
	{
		$phrase .= ' for ADN phonebook' if $set_num == 1;
		$phrase .= ' for FDN phonebook' if $set_num == 2;
		$phrase .= ' for SDN phonebook' if $set_num == 3;
		$phrase .= ' for VMBX phonebook' if $set_num == 4;
		$phrase .= ' for BDN phonebook' if $set_num == 5;
		$phrase .= ' for MBDN phonebook' if $set_num == 6;
	}
	return $phrase;
}

# This subroutine recursively walks through a directory and if there is
# a file present in a directory, copies the distribution.policy file
# into that directory.
# e.g. scan_dir(<full path of dir to walk>, <full path of distribution.policy file>);
sub scan_dir
{
	# Recursively scan through each directory
	my $current_dir = shift; # 1st arg is current directory we are scanning
	my $policy_file_location = shift; # 2nd arg is full path to distribution.policy template
	opendir(DIR, $current_dir) or die $!;
	my @current_dir_contents = readdir(DIR);
	closedir(DIR);

	foreach my $name (@current_dir_contents)
	{
		next if $name =~ /\.\.?$/; # Skip . and ..

		my $full_path = $current_dir.'/'.$name;
		if (-d $full_path)
		{
			scan_dir($full_path, $policy_file_location);
			#next;
			opendir (CURRENT, $full_path) or die $!;
			my (@current) = readdir(CURRENT);
			closedir (CURRENT);
			foreach my $file (@current)
			{
			    #print "$policy_file_location and $full_path" if (-f $full_path.'/'.$file);
				copy($policy_file_location, $full_path.'/distribution.policy') and last if (-f $full_path.'/'.$file);
  			}
		}
	}
}

#######################################################################
# Read the Excel Spreadsheet file
#######################################################################

my $excel = Win32::OLE->GetActiveObject('Excel.Application')
			|| Win32::OLE->new('Excel.Application', 'Quit');
my $workbook = $excel->Workbooks->Open($ARGV[0]);
$excel->{DisplayAlerts} = 0; # Don't display pop up dialogs

# Read functional units

my $func_units_worksheet = $workbook->Worksheets("FunctionalUnits");

my $COL_FUNC_UNIT_NAME = 1;
my $COL_FUNC_UNIT_NO_SPACES = 2; # Check if we really need this
my $COL_FUNC_UNIT_CODE = 3;

my $row = 1;
my $cell = '';
$cell = $func_units_worksheet->Cells($row, $COL_FUNC_UNIT_NAME)->{'Value'}; # First row
while (defined $cell && $cell ne '')
{
	# $, = "\t";
	# print $worksheet->Cells($row, $COL_FUNC_UNIT_NAME)->{'Value'},
	#		$worksheet->Cells($row, $COL_FUNC_UNIT_NO_SPACES)->{'Value'},
	#		$worksheet->Cells($row, $COL_FUNC_UNIT_CODE)->{'Value'}, "\n";
	my $func_unit = $func_units_worksheet->Cells($row, $COL_FUNC_UNIT_NAME)->{'Value'};
	$func_unit =~ s/\s//g;
	$func_units{$func_unit} = $func_units_worksheet->Cells($row, $COL_FUNC_UNIT_CODE)->{'Value'};
	$row++;
	$cell = $func_units_worksheet->Cells($row, $COL_FUNC_UNIT_NAME)->{'Value'};
}

# Read new APIs
my $COL_API_CLASS = 1; my $API_CLASS_ARRAY_INDEX = 0;
my $COL_API_NAME = 2; my $API_NAME_ARRAY_INDEX = 1;
my $COL_API_CODE = 3; my $API_CODE_ARRAY_INDEX = 2;
my $COL_API_FUNCTIONAL_UNIT = 4; my $API_FUNC_UNIT_ARRAY_INDEX = 3;

$row = 3; # Row 1 is example API in spreadsheet
my $new_apis_worksheet = $workbook->Worksheets("NewApis");
$cell = '';
$cell = $new_apis_worksheet->Cells($row, $COL_API_CLASS)->{'Value'};
while (defined $cell && $cell ne '')
{
    my $api_class = $new_apis_worksheet->Cells($row, $COL_API_CLASS)->{'Value'};
    my $api_name = $new_apis_worksheet->Cells($row, $COL_API_NAME)->{'Value'};
    my $api_code = $new_apis_worksheet->Cells($row, $COL_API_CODE)->{'Value'};
    my $api_func_unit = $new_apis_worksheet->Cells($row, $COL_API_FUNCTIONAL_UNIT)->{'Value'};
    $api_func_unit =~ s/\s//g;
    #$, = "\t";
	#print $api_class, $api_name, $api_code, $api_func_unit, "\n";
	
	if (!defined $func_units{$api_func_unit})
	{
		die "\n\ERROR: Functional Unit '$api_func_unit' specified for API $api_class\:\:$api_name does not exist. Pick a functional unit from the FunctionalUnits worksheet in the spreadsheet for this API, correct the details for this API and try again.\n";
	}
	
	# Check if we have encountered this functional unit before
	if (! defined $apis{$api_func_unit})
	{
		$apis{$api_func_unit} = (); # Initialise the empty array
	}

	push @{$apis{$api_func_unit}}, [$api_class, $api_name, $api_code, $api_func_unit];

	$row++;
	$cell = $new_apis_worksheet->Cells($row, $COL_API_CLASS)->{'Value'};
} # while ($cell ne '')

#######################################################################
# Generate The Functional Unit Files
#######################################################################

# This section generates code specific to each functional unit.
# Three files are generated:
#   - The header file for the TEFUnit test which will test this unit.
#   - The cpp file for the above.
#   - A script file which runs each test step in each functional unit test.

# Tags found in the functional unit template
my $func_unit_uppercase_tag = '<<FUNC_UNIT_NAME_UPPER_CASE>>';
my $func_unit_lowercase_tag = '<<FUNC_UNIT_NAME_LOWER_CASE>>';
my $func_unit_name_tag = '<<FUNC_UNIT_NAME>>';
my $test_steps_declaration_tag = '<<TEST_STEPS_DECLARATION>>';
my $add_test_steps_tag = '<<ADD_TEST_STEPS>>';
my $actual_test_cases_tag = '<<ACTUAL_TEST_CASES>>';

# Read in test server template file (header and cpp file)
open FUNC_UNIT_HEADER_FILE, "<cctsyfuncunit_templ.h.templ" or die $!;
undef $/;
my $func_unit_header_template = <FUNC_UNIT_HEADER_FILE>;
close FUNC_UNIT_HEADER_FILE;

open FUNC_UNIT_CPP_FILE, "<cctsyfuncunit_templ.cpp.templ" or die $!;
my $func_unit_cpp_template = <FUNC_UNIT_CPP_FILE>;
close FUNC_UNIT_CPP_FILE;

foreach my $func_unit_name (keys %apis)
{
	my $test_step_code;

	my $f_uppercase = "\U$func_unit_name";
	my $f_lowercase = "\L$func_unit_name";
	my $output_file = lc(get_test_class_name($func_unit_name));

	###### Do the header file first ######
	my $output = $func_unit_header_template;

	# Do copyright notice
	$output = get_copyright_notice($output_file.'.h').$output;

	$output =~ s/$func_unit_uppercase_tag/$f_uppercase/g;
	$output =~ s/$func_unit_lowercase_tag/$f_lowercase/g;
	$output =~ s/$func_unit_name_tag/$func_unit_name/g;

	# Generate the test step declarations
	# Cycle through each API to be tested for the current functional unit
	for my $i (0..$#{$apis{$func_unit_name}})
	{
	    # Work out how many tests there needs to be for this API
	    my $num_tests = 5; # Default is 5: test API, test cancel, test bad params, multiple client, timeout

		# Special case for RCall, RMobileCall, RLine, RMobileLine
		# Each individual test needs to be done 3 times for voice, data and fax calls/lines
		if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] =~ /^(RCall|RMobileCall|RLine|RMobileLine)$/)
		{
            $num_tests = 15; # 5 tests x 3 lines
		}

		# Put special case for RMobilePhoneBookStore here
	    if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] eq 'RMobilePhoneBookStore')
	    {
	        $num_tests = 30; # 5 tests x 6 phonebooks
		}

        $total_num_tests{$func_unit_name} += $num_tests; # Add number of tests to total

	    for my $j (1..$num_tests)
	    {
            $test_step_code .= 	"\tvoid Test$apis{$func_unit_name}[$i][1]000$j();\n";
		}

	} # for my $i (0..$#{$apis{$func_unit_name}})

	$output =~ s/$test_steps_declaration_tag/$test_step_code/;

	# Write the header file for the current functional unit test

	open OUTPUT_HEADER_FILE, ">test/component/inc/${output_file}.h" or die $!;
	print OUTPUT_HEADER_FILE $output;
	close OUTPUT_HEADER_FILE;

	########### Do the CPP file ###########
	$output = $func_unit_cpp_template;

	# Do copyright notice
	$output = get_copyright_notice($output_file.'.cpp').$output;

	$output =~ s/$func_unit_uppercase_tag/$f_uppercase/g;
	$output =~ s/$func_unit_lowercase_tag/$f_lowercase/g;
	$output =~ s/$func_unit_name_tag/$func_unit_name/g;

	my $add_test_step_code = '';
	my $test_case_functions_code = '';

	# Generate the code which needs to go through each API in the functional unit
	for my $i (0..$#{$apis{$func_unit_name}})
	{
	    my $num_sets = 1; # Default
	    $num_sets = 3 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] =~ /^(RCall|RMobileCall|RLine|RMobileLine)$/); # 3 types of line, voice, data, fax
	    $num_sets = 6 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] eq 'RMobilePhoneBookStore'); # 6 phonebooks

	    for my $current_set (1..$num_sets)
	    {
	        my $num_tests_in_set = 5;
		    for my $j (1..$num_tests_in_set) # $j is the test number (1=test API, 2=test cancel, 3=test bad params, 4=multiple client completion), 5=timeout
		    {
				my $current_test_num = $j + $num_tests_in_set * ($current_set - 1);

				# ADD_TEST_STEP bit
				$add_test_step_code .= "\tADD_TEST_STEP(Test$apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX]000$current_test_num);\n";

				# Test step functions
				# Test tags
				my $func_unit_code = $func_units{$func_unit_name};
				my $api_code = $apis{$func_unit_name}[$i][$API_CODE_ARRAY_INDEX];
				my $etel_class = $apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX ];
				my $api_name = $apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX];

				#print "func_unit_name = $func_unit_name func_unit_code = $func_unit_code, api_code = $api_code, etel_class = $etel_class, api_name = $api_name\n\n";

				my $test_desc = get_test_description("$etel_class\:\:$api_name", $j) . get_extra_phrase($etel_class, $current_set);
				my $test_actions = get_test_actions("$etel_class\:\:$api_name", $j) . get_extra_phrase($etel_class, $current_set);

				my $test_tags_template =<<TEST_TAGS
/**
\@SYMTestCaseID BA-CTSY-$func_unit_code-$api_code-000$current_test_num
\@SYMComponent  telephony_ctsy
\@SYMTestCaseDesc $test_desc
\@SYMTestPriority High
\@SYMTestActions $test_actions
\@SYMTestExpectedResults Pass
\@SYMTestType CT
*/
TEST_TAGS
;

				$test_case_functions_code .= "\n$test_tags_template".
			   		'void '.get_test_class_name($func_unit_name)."::Test$apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX]000$current_test_num()\n".
			 	   "\t{\n\n";
				if ($j == 1)
				{
				    my $add_test_code_here = '';

				    $add_test_code_here .=<<TEST_CODE_COMMENTS

 	//-------------------------------------------------------------------------
	// TEST A: failure to dispatch request to LTSY
 	//-------------------------------------------------------------------------

	// Remove the test above if this request does not get passed down to LTSY

	//-------------------------------------------------------------------------
	// TEST B: failure on completion of pending request from LTSY->CTSY
 	//-------------------------------------------------------------------------

	// Remove the test above if this is not relevant for this API

 	//-------------------------------------------------------------------------
	// TEST C: Successful completion request of
	// $etel_class\:\:$api_name when result is not cached.
 	//-------------------------------------------------------------------------

	// Remove the test above if this is not relevant for this API

 	//-------------------------------------------------------------------------
	// TEST D: $etel_class\:\:$api_name again, this time CTSY
	// will get result from the cache.
 	//-------------------------------------------------------------------------

	// Remove the test above if this is not relevant for this API

 	//-------------------------------------------------------------------------
	// TEST E: Unsolicited completion of $etel_class\:\:$api_name
	// from LTSY.
 	//-------------------------------------------------------------------------

	// Remove the test above if this is not relevant for this API

 	//-------------------------------------------------------------------------
	// TEST F: Coverage test(s) for $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// After writing all the other tests for this API, including test 002, 003 etc.
	// Use the code coverage tool to check for significant areas of TSY code which
	// have not been covered by the tests you have already written and add new tests
	// here to reach those areas. Then remove this comment.

TEST_CODE_COMMENTS
;

					$test_case_functions_code .=<<TEST_CODE
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	$add_test_code_here

	AssertMockLtsyStatus();
	CleanupStack::PopAndDestroy(2, this); // data, this

TEST_CODE
;

				}
				elsif ($j == 2)
				{
					$test_case_functions_code .= "// This test should test cancellation of $apis{$func_unit_name}[$i][1]\n".
												 "// If this API does not have a cancel, the test step should be completely removed.\n\n";

	            	$test_case_functions_code .=<<TEST_CODE
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	TRequestStatus mockLtsyStatus;
	iMockLTSY.NotifyTerminated(mockLtsyStatus);

	RBuf8 data;
	CleanupClosePushL(data);

 	//-------------------------------------------------------------------------
	// Test cancelling of $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// Remove the test above if this is not relevant for this API

	// Wait for completion of iMockLTSY.NotifyTerminated
	// User::WaitForRequest(mockLtsyStatus);
	CleanupStack::PopAndDestroy(2); // data, this

TEST_CODE
;
				}
				elsif ($j == 3)
				{
					$test_case_functions_code .= "// This test should test sending bad parameter data for $apis{$func_unit_name}[$i][1]\n".
												 "// If this API does not have any parameters, then remove this test completely.\n\n";
        	    	$test_case_functions_code .=<<TEST_CODE
	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	//-------------------------------------------------------------------------
	// Test A: Test passing wrong version of parameters to
	// $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// Remove the test above if it is not relevant for this API

	//-------------------------------------------------------------------------
	// Test B: Test passing wrong descriptor size to parameter in
	// $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// Remove the test above if it is not relevant for this API

	//-------------------------------------------------------------------------
	// Test C: Test passing out of bounds parameters to
	// $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

 	// Remove the test above if it is not relevant for this API

	// Done !
	CleanupStack::PopAndDestroy(2, this); // data, this

TEST_CODE
;
				}
				elsif ($j == 4)
				{
					#$test_case_functions_code .= "// This test should test multi-client requests to notifier $apis{$func_unit_name}[$i][1]\n\n";

					$test_case_functions_code .=<<TEST_FUNC_CODE

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	// Open second client
	RTelServer telServer2;
	TInt ret = telServer2.Connect();
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(telServer2);

	RMobilePhone phone2;
	ret = phone2.Open(iTelServer,KMmTsyPhoneName);
	ASSERT_EQUALS(KErrNone, ret);
	CleanupClosePushL(phone2);

	//-------------------------------------------------------------------------
	// Test A: Test multiple clients requesting $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// If this API is flow controlled by Etel or if this API is
	// synchronous, remove this test completely.

	// To write this test:
	// If this API has parameters which have different versions, then each
	// client in this test should use a different version of parameter.
	// Otherwise, they should use the same version.
	// All versions of parameters should be used. i.e. If there are three
	// versions of a parameter, there should be three clients, one using
	// each one. If the parameter does not have different versions, use
	// two clients each using the same version.

	// Done !
	CleanupStack::PopAndDestroy(2, this); // data, this

TEST_FUNC_CODE
;

				}
				elsif ($j == 5)
				{
					$test_case_functions_code .=<<TEST_FUNC_CODE

	OpenEtelServerL(EUseExtendedError);
	CleanupStack::PushL(TCleanupItem(Cleanup,this));
	OpenPhoneL();

	RBuf8 data;
	CleanupClosePushL(data);

	//-------------------------------------------------------------------------
	// Test A: Test timeout of $etel_class\:\:$api_name
 	//-------------------------------------------------------------------------

	// If this API does not have a timeout, then remove this test completely.

	// Done !
	CleanupStack::PopAndDestroy(2, this); // data, this

TEST_FUNC_CODE
;

				}
				$test_case_functions_code .= "\t}\n\n";
			} # for my $j (1..4)
	    } # for my $current_set...
	} # for my $i (0..$#{$apis{$func_unit_name}})

	$output =~ s/$add_test_steps_tag/$add_test_step_code/;
	$output =~ s/$actual_test_cases_tag/$test_case_functions_code/;

	# Write the cpp file for the current functional unit test
    $output_file = lc(get_test_class_name($func_unit_name));
	open OUTPUT_CPP_FILE, ">test/component/src/${output_file}.cpp" or die $!;
	print OUTPUT_CPP_FILE $output;
	close OUTPUT_CPP_FILE;
} # foreach my $func_unit_name (keys %apis)

#######################################################################
# Generate The Script Files
#######################################################################

#
# The script that runs through everything
#
print "Note: If you have added a new functional unit, don't forget to update ctsy/test/component/scripts/te_ctsycomponent_all.script
and create a te_ctsycomponent_<functional unit name>funcunit_all.script (refer to the existing 'all.script' files for the format.";

#
# Script to run timeout tests
#
open SCRIPT_FILE, ">test/component/scripts/te_ctsycomponent_timeout_all.script" or die $!;
print SCRIPT_FILE   "LOAD_SUITE $component_test_suite_name\n\n";
foreach my $func_unit_name (keys %apis)
{
	for my $i (0..$#{$apis{$func_unit_name}})
	{
		my $func_unit_code = $func_units{$func_unit_name};
		my $api_code = $apis{$func_unit_name}[$i][$API_CODE_ARRAY_INDEX];
		my $etel_class = $apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX];
		my $api_name = $apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX];

	    my $num_sets = 1; # Default
	    $num_sets = 3 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] =~ /^(RCall|RMobileCall|RLine|RMobileLine)$/);
	    $num_sets = 6 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] eq 'RMobilePhoneBookStore');

	    for my $current_set (1..$num_sets)
	    {
		my $current_test_num = 5 * $current_set;

		#print "api = $etel_class\:\:$api_name current_test_num = $current_test_num\n";

		my $test_desc = get_test_description("$etel_class\:\:$api_name", $current_test_num / $current_set) . get_extra_phrase($etel_class, $current_set);
		my $test_actions = get_test_actions("$etel_class\:\:$api_name", $current_test_num / $current_set) . get_extra_phrase($etel_class, $current_set);

				# Test case ID format: BA-CTSY-<Func.unit code>-<API code>-<index>
		   	 	my $test_case_id = "BA-CTSY-$func_units{$func_unit_name}-$apis{$func_unit_name}[$i][$API_CODE_ARRAY_INDEX]-000$current_test_num";

				my $test_tags =<<TEST_TAGS;
START_TESTCASE $test_case_id
//!\@SYMTestCaseID $test_case_id
//!\@SYMComponent telephony_ctsy
//!\@SYMTestCaseDesc $test_desc
//!\@SYMTestPriority High
//!\@SYMTestActions $test_actions
//!\@SYMTestExpectedResults Pass
//!\@SYMTestType CT
TEST_TAGS
;
				print SCRIPT_FILE   $test_tags;
				print SCRIPT_FILE   "RUN_TEST_STEP 300 $component_test_suite_name ".
				    "TEFUnit.".get_test_class_name($func_unit_name).'.Test'.$apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX]."000$current_test_num\n";
				print SCRIPT_FILE "END_TESTCASE $test_case_id\n\n";
		}
	}
}
close SCRIPT_FILE;

#
# Script that explicitly calls each individual step in a functional unit
#
foreach my $func_unit_name (keys %apis)
{
	open SCRIPT_FILE, ">test/component/scripts/te_ctsycomponent_".lc($func_unit_name)."_individual.script" or die $!;
	print SCRIPT_FILE   "LOAD_SUITE $component_test_suite_name\n\n";

	for my $i (0..$#{$apis{$func_unit_name}})
	{
		# Test tags - useful reference to store these in variables here
		my $func_unit_code = $func_units{$func_unit_name};
		my $api_code = $apis{$func_unit_name}[$i][$API_CODE_ARRAY_INDEX];
		my $etel_class = $apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX];
		my $api_name = $apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX];

	    my $num_sets = 1; # Default
	    $num_sets = 3 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] =~ /^(RCall|RMobileCall|RLine|RMobileLine)$/);
	    $num_sets = 6 if ($apis{$func_unit_name}[$i][$API_CLASS_ARRAY_INDEX] eq 'RMobilePhoneBookStore');

	    for my $current_set (1..$num_sets)
	    {
	        my $num_tests_in_set = 4; # We don't want the time out test to be part of main script
		    for my $j (1..$num_tests_in_set) # $j is the test number (1=test API, 2=test cancel, 3=test bad params, 4=multiple client completion)
		    {
				my $current_test_num = $j + $num_tests_in_set * ($current_set - 1);

				my $test_desc = get_test_description("$etel_class\:\:$api_name", $j) . get_extra_phrase($etel_class, $current_set);
				my $test_actions = get_test_actions("$etel_class\:\:$api_name", $j) . get_extra_phrase($etel_class, $current_set);

				# Test case ID format: BA-CTSY-<Func.unit code>-<API code>-<index>
		   	 	my $test_case_id = "BA-CTSY-$func_units{$func_unit_name}-$apis{$func_unit_name}[$i][$API_CODE_ARRAY_INDEX]-000$current_test_num";

				my $test_tags =<<TEST_TAGS;
START_TESTCASE $test_case_id
//!\@SYMTestCaseID $test_case_id
//!\@SYMComponent telephony_ctsy
//!\@SYMTestCaseDesc $test_desc
//!\@SYMTestPriority High
//!\@SYMTestActions $test_actions
//!\@SYMTestExpectedResults Pass
//!\@SYMTestType CT
TEST_TAGS
;
				print SCRIPT_FILE   $test_tags;
				print SCRIPT_FILE   "RUN_TEST_STEP 100 $component_test_suite_name ".
				    "TEFUnit.".get_test_class_name($func_unit_name).'.Test'.$apis{$func_unit_name}[$i][$API_NAME_ARRAY_INDEX]."000$current_test_num\n";
				print SCRIPT_FILE "END_TESTCASE $test_case_id\n\n";
			} # for my $j (1..$num_tests_in_set)
		} # for my $current_set (1..$num_sets)
	}
	close SCRIPT_FILE;
}

