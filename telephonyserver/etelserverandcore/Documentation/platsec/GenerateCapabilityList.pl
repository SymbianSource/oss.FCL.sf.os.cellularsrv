# Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
# Script to generate API Policing data structures from CSV file
# Usage:
# cvs files as input:  GenerateCapabilityList.pl caps.csv  [caps2.csv] [caps3.csv] ...
# excel file as input:  GenerateCapabilityList.pl caps.xls etel|phbksync|c32
# output - file with extension out added to the name of the first argument
# use strict;
# 
#

my $dest_file_ext = ".out";

# following hash maps second input parameter to list of spreadsheets in the execel file to be processed
my %inputParms =
	(
	"etel" => "etel etelmm etelpckt etelsat etelcdma customapi",
	"phbksync" => "phbksync",
	"c32" => "c32",
	);

my $capsColumnName = "Final Capabilities";
my $ipcValueColumnName = "New IPC Number";

my @spreadSheets = ();

# All valid capabilities
my %Capabilities =
	(
	"TCB" => 1,
	"CommDD" => 1,
	"PowerMgmt" => 1,
	"MultimediaDD" => 1,
	"ReadDeviceData" => 1,
	"WriteDeviceData" => 1,
	"DRM" => 1,
	"TrustedUI" => 1,
	"ProtServ" => 1,
	"DiskAdmin" => 1,
	"NetworkControl" => 1,
	"AllFiles" => 1,
	"SwEvent" => 1,
	"NetworkServices" => 1,
	"LocalServices" => 1,
	"ReadUserData" => 1,
	"WriteUserData" => 1,
	"Location" => 1
	);

sub validate_capabilities
	{
	my ($caps) = @_;
	no warnings;  # silence spurious -w undef complaints
	if (@$caps eq 0)
		{
		return 0;
		}

	$cap = $caps->[0];

	if ((@$caps == 1) and (($cap =~ /KCapabilityNone/)
	    or ($cap =~ /None/) or ($cap =~ /CustomCheckStart/)
	    or ($cap =~ /CustomCheckEnd/) or ($cap =~ /CustomCheckSingle/)))
		{
		if($cap =~ /KCapabilityNone/)
			{
			$cap=substr( $caps->[$0], 11 );
			$caps->[$0]=$cap;
			}

		return 1; # magic "capabilities"
		}

	for ($i = 0; $i < @$caps; $i++)
		{
		if($caps->[$i] =~ /^KCapability/)# old csv format
			{
			$cap=substr( $caps->[$i], 11 );
			$caps->[$i]=$cap;
			#printf "\n KCapability found: $cap";
			}
		else
			{
			$cap=$caps->[$i];
			}

		#printf "\n ->$cap<-";

		if($Capabilities{$cap} ne 1)
			{
			return 0;
			}
		}

	return 1;
	}

my %ipc_table = ();
my $count = 0;
my $ipc_num = 0;
my $numOfFiles = @ARGV;
my $inputMode = "";
my %policies = ();

# parse arguments in order to figure out type of input files
if ($numOfFiles  > 0)
	{
	$source_file = $ARGV[0];

	foreach $i (0 .. $#ARGV)
		{
		if ($inputMode eq "") # first argument
			{
			if ($ARGV[$i] =~ /$\.csv/)
				{
				$inputMode = "csv";
				}
			elsif ($ARGV[$i] =~ /$\.xls/) # excel
				{
				$inputMode = "excel";
				}
			else
				{
				die " Unexpected argument  $ARGV[$i] !\n";
				}
			}
		else  # verify consistency of follow-up arguments
			{
			if (($inputMode eq "csv") and !($ARGV[$i] =~ /$\.csv/) )
				{
				die " Unexpected mixed arguments inputMode: $inputMode file:$ARGV[$i] \n";
				}
			elsif (($inputMode eq "excel") and ($i > 1))
				{
				die " Unexpected arguments $ARGV[$i] \n";
				}
			elsif (($inputMode eq "excel") and ($i eq 1))
				{
				if($inputParms{$ARGV[$i]} ne "")
					{
					@spreadSheets= split('\s+' ,$inputParms{$ARGV[$i]});
					}
				else
					{
					die "Sheet not supported: $ARGV[$i]";
					}
				}
			}
		} # foreach argument in command line

	if (($inputMode eq "excel") and ($#ARGV == 0))
		{
		die "\nArgument missing - name of the server!\n";
		}

	if ($inputMode eq "csv") # process input csv files
		{ # TODO - Fix hashing into %policies (currently, this script is broken when used with csv becauase of this)
		my $nameOfInputFile = "";
		while (<ARGV>)
			{
			if ($ARGV ne $nameOfInputFile)
				{
				$nameOfInputFile=$ARGV;
				print "\nProcessing $nameOfInputFile";
				}

			if (/(.*),([0-9]+),(.*)$/)
				{
				$count++;
				m/(.*),([0-9]+),(.*)$/;

				$ipc_num = $2;
				$capability = $3;
				$capability =~ s/,(.*)//;

				$capability =~ s/^\s+//;
				$capability =~ s/\s+$//; # remove leading & trailing white spaces

				if($ipc_table{$ipc_num} ne "")
					{
					print "\nWarning - ipc $ipc_num is already defined with capability $ipc_table{$ipc_num} - $capability to be assigned .";
					}
				else
					{
					my @capsArray = sort split('\s+', $capability);

					if (validate_capabilities(\@capsArray ) != 1)
						{
						die "Capabilities are not valid for IPC #$ipc_num ! ";
						}

					my $sortedCaps="";
					for (my $i = 0; $i < scalar @capsArray; $i++)
						{
						$sortedCaps=$sortedCaps." $capsArray[$i]";
						}

					$ipc_table{$ipc_num} = $sortedCaps;
					#print "\nipc $ipc_num is capability $ipc_table{$ipc_num} !";
					}
				}
			}
		$ipc_num++;
		} # csv files, processed

	elsif ($inputMode eq "excel") # process input excel file - set of sheets defined by second parameter
		{
		use Win32::OLE;
		# Establish MS Excel Access
		eval {$main::excel= Win32::OLE->GetActiveObject('Excel.Application')};

		die "Excel is not installed" if $@;

		unless (defined $main::excel)
			{
			$main::excel = Win32::OLE->new('Excel.Application', sub {$_[0]->Quit;})
			  or die "Cant start Excel";
			}
		print "Excel access established\n";

		$main::excel->{DisplayAlerts} = 0; # turn off pesky alerts

		# Open Our Capability Book & Spreadsheet
		$main::excelbook = $main::excel->Workbooks->Open({FileName => $source_file, ReadOnly=>1})
		  or die "Can't open $source_file";
		print "Excel spreadsheet $source_file opened\n";

		#################################
		for $spreadSheet (@spreadSheets)
			{
			print "\n\nProcessing sheet $spreadSheet\n";
			$main::excelsheet = $main::excelbook->Worksheets($spreadSheet)
			  or die "Can't open sheet $spreadSheet";

			$main::excelsheet->Activate();
			my $numRows = $main::excelsheet->UsedRange->Rows->{Count};
			my $numColumns = $main::excelsheet->UsedRange->Columns->{Count};

			my $IpcCol = 0;
			my $CapsCol = 0;

			for (my $col = 1; ($col <= $numColumns) and (($IpcCol == 0) or ($CapsCol == 0)); $col++)
				{
				$nameOfCol = $main::excelsheet->Cells(1,$col)->{'Value'};

				if ($nameOfCol =~ /$ipcValueColumnName/)
					{
					$IpcCol = $col;
					}
				elsif ($nameOfCol =~ /$capsColumnName/)
					{
					$CapsCol = $col;
					}
				} # end finding out relevant columns

			for (my $row = 2; ($row <= $numRows); $row++) # Start at row = 2 because anything above is header.
				{
				$ipc_num = $main::excelsheet->Cells($row,$IpcCol)->{'Value'};
				#printf "\n row: $row - ipc: $ipc_num";
				if($ipc_num ne "")
					{
					$capability = $main::excelsheet->Cells($row,$CapsCol)->{'Value'};
					$capability =~ s/,(.*)//;

					$capability =~ s/^\s+//;
					$capability =~ s/\s+$//; # remove leading & trailing white spaces

					if($ipc_table{$ipc_num} ne "")
						{
						print "\nWarning - ipc $ipc_num is already defined with capability $ipc_table{$ipc_num} - $capability to be assigned.";
						}
					else
						{
						my @capsArray = sort { $a cmp $b } split('\s+', $capability);

						if (validate_capabilities(\@capsArray ) != 1)
							{
							die "Capabilities are not valid for IPC #$ipc_num!";
							}
						my $sortedCaps = "";
						foreach my $seperateCap (@capsArray)
							{ # TODO - remove leading white space
							$sortedCaps .= " $seperateCap";
							}
						$ipc_table{$ipc_num} = $sortedCaps;

						if (!($sortedCaps =~ /CustomCheck/)) # Create a policy entry for each new combination of caps
							{ # TODO move this down to where ranges are generated
							if ($policies{$sortedCaps})
								{
								$policies{$sortedCaps}++; # TODO no need to increment number.
								}
							else
								{
								$policies{$sortedCaps} = 1;
								}
							}
						}

					$ipc_num++;
					}
				} # end for all rows

			$main::excelsheet->Close();
			} # end for workSheets

		#################################
		$main::excel->ActiveWorkbook->Close(0);
		$main::excel->Quit();
		} # excel spreadsheet
	else
		{
		die "$source_file is neither csv nor xls file !";
		}

	# at this point following hash is generated, where $ipc_table{k} is unique, but not sorted yet
	#    ipc1     caps1
	#    ipc2     caps2
	#    ...
	#    ipcn     capsn
	@ipcs = sort {$a <=> $b} keys %ipc_table;

	if(@ipcs == 0)
		{
		die "\n There are no ipcs defined!\n";
		}

	# generate hash that contains ranges of ipcs with same capability :
	# ipc1   count1
	# ipc2   count2
	# ...
	# ipcm   count m

	# number of ranges is less or ( in the worst case) equal to number of input ipcs
	# ipcs are sorted in incremental order

	open(OUTPUT, ">$source_file$dest_file_ext")
	  or die "Can't create output file: $!";

	# generate ranges
	my @range_sizes = ();
	my @range_starts = ();

	my $customCheckOn = 0;
	$count = -1; # initial count of ipcs in the range

	# prepare for start
	push @range_starts, $ipcs[0];
	my $caps = $ipc_table{$ipcs[0]};
	$prev_caps = $caps;
	$prev_ipc = $ipcs[0]-1;

	for $ipc (@ipcs)
		{
		$caps = $ipc_table{$ipc};
		$count++;

		# enforce pairs CustomCheckStart/CustomCheckEnd
		if(($customCheckOn == 1) and !($caps =~ /CustomCheckEnd/))
			{
			die "Inconsistent use of CustomCheckEnd at ipc #$ipc \n";
			}

		if ($caps =~ /CustomCheckStart/ and $customCheckOn == 1)
			{
			die "Inconsistent use of CustomCheck at ipc #$ipc; CustomCheckStart already at previous IPC\n";
			}
		elsif ($caps =~ /CustomCheckEnd/ and $customCheckOn == 0)
			{
			die "Inconsistent use of CustomCheck at ipc #$ipc; CustomCheckEnd without a start\n";
			}
		elsif ($caps =~ /CustomCheckStart/)
			{
			$customCheckOn = 1;
			if ($ipc ne $ipcs[0])  # custom range is not the first
				{
				push (@range_sizes, $count); # complete previous range
				# start new range
				push (@range_starts, $ipc);
				}

			$prev_ipc = $ipc;
			$prev_caps = $caps;
			$count = 0;
			next;
			}
		elsif( $caps =~ /CustomCheckEnd/)
			{
			$customCheckOn = 0;
			$count = $ipc-$prev_ipc;

			if($count == 0)
				{
				$count = 1;
				}

			$prev_ipc = $ipc;
			next;
			}

		# end of the range if caps differ of non-sequental ipcs
		if (($caps ne $prev_caps) or ($prev_ipc ne ($ipc-1)))
			{
			push (@range_sizes, $count);

			# start new range
			push (@range_starts, $ipc);
			$count = 0;
			}

		# continue to iterate through range
		$prev_ipc = $ipc;
		$prev_caps = $caps;
		} # for all IPCs

	$count++;
	push @range_sizes, $count;

	# now generate output c++ structures

	$range_id = 0;
	$curr_ipc = $ipcs[0];
	$range_start = $curr_ipc;
	print OUTPUT "\n\nconst TInt iRanges[] = \n\t{\n";

	$startRange=0;
	$numOfRanges=0;
	$i=0;
	$lastIpc= $range_starts[(scalar @range_starts)-1];

	for $startIpcs (@range_starts)
		{
		if($startRange != $startIpcs)
			{
			$endRange = $startIpcs-1;
			print OUTPUT "\t$startRange,\t\t//range is $startRange-$endRange inclusive\n";
			$numOfRanges++;
			}

		$endRange = $startIpcs + $range_sizes[$i]-1;

		if($endRange != $startIpcs)
			{
			print OUTPUT "\t$startIpcs,\t\t//range is $startIpcs-$endRange inclusive\n";
			}
		else
			{
			if( ($customCheckOn == 1) and ($lastIpc eq $startIpcs ))
				{
				print OUTPUT "\t$startIpcs,\t\t//range is $startIpcs-KMaxTInt\n";
				}
			else
				{
				print OUTPUT "\t$startIpcs,\t\t//range is $startIpcs\n";
				}
			}
		$numOfRanges++;
		$startRange = $endRange + 1;
		$i++;
		} # for each range of IPCs

	if(!($customCheckOn == 1))
		{
		print OUTPUT "\t$startRange,\t\t//range is $startRange-KMaxTInt inclusive\n";
		$numOfRanges++;
		}
	print OUTPUT "\t};\n";

	print OUTPUT "\nconst TUint iRangeCount = $numOfRanges;\n";

	# Number the policies in the order they will be placed in the server's const static array
	my $ii = 0;
	# Sort the policies table for later printing into an array
	foreach (sort keys %policies)
		{
		# Ensure array indicies are given correctly to all policies.
		$policies{$_} = $ii++;
		}

	print OUTPUT "\n\nconst TUint8 iElementsIndex[] = \n\t{\n";
	$startRange=0;
	$i=0;

	for $startIpc (@range_starts)
		{
		if($startRange != $startIpc)
			{
			$endRange = $startIpc-1;
			print OUTPUT "\tCPolicyServer::ENotSupported,\n"; # range not covered by API policing
			$numOfRanges++;
			}

		if(($ipc_table{$startIpc} =~ /CustomCheckSingle/))
			{
			print OUTPUT "\tCPolicyServer::ECustomCheck,\n"; # range to be custom checked
			}
		elsif(!($ipc_table{$startIpc} =~ /CustomCheck/))
			{
			print OUTPUT "\t$policies{$ipc_table{$startIpc}},\n";
			}
		else
			{
			print OUTPUT "\tCPolicyServer::ECustomCheck,\n"; # range to be custom checked
			}

		$endRange = $startIpc + $range_sizes[$i]-1;
		$startRange=$endRange+1;
		$i++;
		} # For each range print link to policy table

	if( !($customCheckOn == 1) )
		{
		print OUTPUT "\tCPolicyServer::ENotSupported,\n";  # last range - up to KIntMax
		}
	print OUTPUT "\t};\n";

	print OUTPUT "\n\nconst CPolicyServer::TPolicyElement iElements[] = \n\t{\n";
	$startRange = 0;
	$currentRange = 0;
	$range_id = 0;

	foreach (sort keys %policies)
		{
		my @capsArray = split ('\s+', $_);
		my $numberCaps=scalar @capsArray-1;

		print OUTPUT "\t{ _INIT_SECURITY_POLICY_C$numberCaps( ";
		for (my $i = 1; $i < $numberCaps; $i++) # Should start at zero, but would need to change lines 283-286 first; i.e. get rid of space at beginning of $capsArray[i].
			{
			if($capsArray[$i] =~ /None/)
				{
				print OUTPUT "ECapability_$capsArray[$i], ";
				}
			else
				{
				print OUTPUT "ECapability$capsArray[$i], ";
				}
			} # foreach capability, output a security caps flag

		if($capsArray[$numberCaps] =~ /None/)
			{
			print OUTPUT "ECapability_$capsArray[$numberCaps])";
			}
		else
			{
			print OUTPUT "ECapability$capsArray[$numberCaps])";
			}

		print OUTPUT ", CPolicyServer::EFailClient},\n";
		}

	print OUTPUT "\t};\n";

	print OUTPUT "\n\nconst CPolicyServer::TPolicy iPolicy = \n\t{\n";
	print OUTPUT "\tCPolicyServer::EAlwaysPass,\n";
	print OUTPUT "\tiRangeCount,\n";
	print OUTPUT "\tiRanges,\n";
	print OUTPUT "\tiElementsIndex,\n";
	print OUTPUT "\tiElements\n";
	print OUTPUT "\t};\n";
	close(OUTPUT);
	}
else
	{
	die "Bad arguments !";
	}
