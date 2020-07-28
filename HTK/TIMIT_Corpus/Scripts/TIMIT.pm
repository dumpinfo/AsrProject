package   TIMIT;
require   Exporter;

use       ConfigVar;

@ISA    = qw (Exporter);
@EXPORT = qw (
	      create_MLF
	      doit
	     );

# Default values
%Default     = {
		RootDir      => "~/Research/Corpora/TIMIT_Corpus/TIMIT",
		SampleRate   => "16",
		corpus       => "TIMIT_CORPUS",
                print        => "",
	       };

# CommandLine options
@CommandLine = qw (RootDir=s
		   SampleRate=s
		   corpus=s
		   print
		  );

sub create_MLF {
  local ($debug, $ver, $corpus, $root_dir, $out_dir, $sample_rate);
  local ($dir, $DialectDir, $spkr, $file, $inTIMITwordFile, $outHTKlabFile, $outHTKwordFile);
  local ($num1, $num2, $TIMITlab, $TIMITwrdLab, $HTKwrdLab);
  local (@AllDialects,  @AllSpkrs, @AllFiles, @ABegin, @AEnd, @AtimitLab);
  local (%HTKLabMap, %CORPUS);

  print stderr "create_MLF: Executing ParseConfig ";
  ParseConfig (\@CommandLine, \%Default, CORPUS);
  print stderr "... Done\n";

  %CORPUS        = %ConfigVar::CORPUS;

  foreach $key (sort %CORPUS) {
    print stderr "Key: $key \t=> Value: $CORPUS{$key}\n";
  }

  $debug         = $CORPUS{debug};
  $ver           = $CORPUS{ver};
  $corpus        = $CORPUS{CorpusName};
  $root_dir      = $CORPUS{RootDir};
  $out_dir       = $CORPUS{OutputDir};
  $sample_rate   = $CORPUS{SampleRate};
  @HTKmonoPhones = @{$CORPUS{HTKmonoPhones}};
  %HTKLabMap     = %{$CORPUS{LabMap}};
  %HSteps        = %{$CORPUS{HSteps}};
  #
  # Changinf working dir to out_dir
  #
  chdir ${out_dir};

  $outMLF        = "${out_dir}/${corpus}${ver}_test\.mlf";
  open FP_MLF, ">$outMLF"
    or die "ERROR: Unable to open file $outMLF for writing: $!\n";
  print(FP_MLF "#!MLF!#\n");
  $outWrdMLF        = "${out_dir}/${corpus}_wrd${ver}_test\.mlf";
  open FP_WRD_MLF, ">$outWrdMLF"
    or die "ERROR: Unable to open file $outWrdMLF for writing: $!\n";
  print(FP_WRD_MLF "#!MLF!#\n");

  $dir = "$root_dir/TEST";

  print stderr "OPENNING $dir .....\n";

  opendir DIR1, $dir
    or die "ERROR: Unable to open ROOT DIR $dir for reading: $!\n";

  print stderr "SEARCHING $dir .....\n";

  @AllDialects = grep !/^\.\.?$/, readdir DIR1;

  foreach $DialectDir (@AllDialects) {
    opendir DIR2, "$dir/$DialectDir"
      or die "ERROR: Unable to open DIR $dir/$DialectDir for reading: $!\n";

    print stderr "SEARCHING $dir/$DialectDir .....\n";

    @AllSpkrs = grep !/^\.\.?$/, readdir DIR2;

    foreach $spkr (@AllSpkrs) {
      opendir DIR3, "$dir/$DialectDir/$spkr"
	or die "ERROR: Unable to open DIR $dir/$DialectDir/$spkr for reading: $!\n";

      print stderr "SEARCHING $dir/$DialectDir/$spkr.....\n";

      @AllFiles = grep !/^\.\.?$/, readdir DIR3;

      foreach $file (@AllFiles) {
	#print stderr "File: $dir/$DialectDir/$spkr/$file\n";
	if (($file =~ m/\.phn$/) || ($file =~ m/\.PHN$/)) {
	  open FP_TIMIT_LAB, "<$dir/$DialectDir/$spkr/$file"
	    or die "ERROR: Unable to open file $file for reading: $!\n";

	  $outHTKlabFile = $file;
	  $outHTKlabFile =~ s/\.phn$/.lab/;
	  $outHTKlabFile =~ s/\.PHN$/.lab/;

	  open FP_HTK_LAB, ">$dir/$DialectDir/$spkr/$outHTKlabFile"
	    or die "ERROR: Unable to open file $outHTKlabFile for writing: $!\n";


	  print FP_MLF "\"$dir/$DialectDir/$spkr/$outHTKlabFile\"\n";

	  print stderr "PROCESSING File $dir/$DialectDir/$spkr/$file\n";
	  undef @ABegin;
	  undef @AEnd;
	  undef @AtimitLab;
	  while (<FP_TIMIT_LAB>) {
	    chomp;
	    ($num1, $num2, $TIMITlab) = split /\s+/;
	    #print(stderr "TIMIT: $_ => NUM1 = $num1, NUM2 = $num2, LAB = $TIMITlab\n");
	    # num1 & num2 are begin and end sample number; sample_rate is 16000
	    # HTK uses 0.1 usec units for labels
	    $begin  = $num1*10000/$sample_rate;
	    $end    = $num2*10000/$sample_rate;
	    push @ABegin, $begin;
	    push @AEnd, $end;
	    push @AtimitLab, $TIMITlab;
	  }
	  close (FP_TIMIT_LAB);

	  $inTIMITwordFile = $file;
	  $inTIMITwordFile =~ s/\.phn$/.WRD/;
	  $inTIMITwordFile =~ s/\.PHN$/.WRD/;
	  $outHTKwordFile  = $inTIMITwordFile;
	  $outHTKwordFile  =~ s/\.WRD$/.wlb/;

	  open FP_TIMIT_WRD, "<$dir/$DialectDir/$spkr/$inTIMITwordFile"
	    or die "ERROR: Unable to open file $inTIMITwordFile for reading: $!\n";

	  open FP_HTK_WRD, ">$dir/$DialectDir/$spkr/$outHTKwordFile"
	    or die "ERROR: Unable to open file $outHTKwordFile for reading: $!\n";

	  print FP_WRD_MLF "\"$dir/$DialectDir/$spkr/$outHTKlabFile\"\n";

	  while (<FP_TIMIT_WRD>) {
	    chomp;
	    ($num1, $num2, $TIMITwrdLab) = split /\s+/;
	    $begin  = $num1*10000/$sample_rate;
	    $end    = $num2*10000/$sample_rate;
	    #push @ABegin, $begin;
	    #push @AEnd, $end;
	    #push @AtimitwrdLab, $TIMITwrdLab;
	    $HTKwrdLab = $TIMITwrdLab;
	    $HTKwrdLab =~ tr/a-z/A-Z/; # Converting to upper case
	    printf(FP_WRD_MLF "%5s\n", $HTKwrdLab);
	    printf(FP_HTK_WRD "%5s\n", $HTKwrdLab);
	  }
	  close (FP_TIMIT_WRD);
	  close (FP_HTK_WRD);

	  $skipFlag = 0;
	  for $indx (0..$#AtimitLab) {
	    # special cases
	    if ($skipFlag == 1) {
	      $skipFlag = 0;
	      next;
	    }
	    $TIMITlab = $AtimitLab[$indx];
	    $begin    = $ABegin[$indx];
	    $end      = $AEnd[$indx];
	
	    # Removing pcl, tcl, kcl, bcl, dcl, & gcl
	    if (($TIMITlab eq "pcl") || ($TIMITlab eq "tcl") || ($TIMITlab eq "kcl") ||
		($TIMITlab eq "bcl") || ($TIMITlab eq "dcl") || ($TIMITlab eq "gcl")) {
	      if (($AtimitLab[$indx+1] eq "p") || ($AtimitLab[$indx+1] eq "t") || ($AtimitLab[$indx+1] eq "k") ||
		  ($AtimitLab[$indx+1] eq "b") || ($AtimitLab[$indx+1] eq "d") || ($AtimitLab[$indx+1] eq "g")) {
		printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx+1], $AtimitLab[$indx+1]);
		printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx+1], $AtimitLab[$indx+1]);
		$skipFlag = 1;
		next;
	      } else {
		$lab = $TIMITlab;
		$lab =~ s/cl$//;
		printf(FP_MLF "%10d %10d %5s\n", $begin, $end, $lab);
		printf(FP_HTK_LAB "%10d %10d %5s\n", $begin, $end, $lab);
		next;
	      }
	    }
	    # Converting "uw" "ax" => "ua"
	    if (($TIMITlab eq "uw") && ($AtimitLab[$indx+1] eq "ax")) {
	      printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx+1], "ua");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx+1], "ua");
	      $skipFlag = 1;
	      next;
	    }
	    # Converting "y" "axr" => "ia" "r"
	    if (($TIMITlab eq "y") && ($AtimitLab[$indx+1] eq "axr")) {
	      printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx], "ia");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx], "ia");
	      printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "r");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "r");
	      $skipFlag = 1;
	      next;
	    }
	    # Converting "eh" "axr" => "ea" "r"
	    if (($TIMITlab eq "eh") && ($AtimitLab[$indx+1] eq "axr")) {
	      printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx], "ea");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx], $AEnd[$indx], "ea");
	      printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "r");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "r");
	      $skipFlag = 1;
	      next;
	    }
	    # Converting "axr" => "ax" & "r"
	    if ($TIMITlab eq "axr") {
	      $mid  = ($begin + $end)/2;
	      printf(FP_MLF "%10d %10d %5s\n", $begin, $mid, "ax");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $begin, $mid, "ax");
	      printf(FP_MLF "%10d %10d %5s\n", $mid, $end, "r");
	      printf(FP_HTK_LAB "%10d %10d %5s\n", $mid, $end, "r");
	      # Converting "axr" "aa" => "ax" "r" "oh"
	      if ($AtimitLab[$indx+1] eq "aa") {
		printf(FP_MLF "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "oh");
		printf(FP_HTK_LAB "%10d %10d %5s\n", $ABegin[$indx+1], $AEnd[$indx+1], "oh");
		$skipFlag = 1;
		next;
	      }
	      next;
	    }

	    $HTKlab = $HTKLabMap{$TIMITlab};
	    printf(FP_MLF "%10d %10d %5s\n", $begin, $end, $HTKlab);
	    printf(FP_HTK_LAB "%10d %10d %5s\n", $begin, $end, $HTKlab);

	    #print(stderr "HTK  : NUM1 = $begin, NUM2 = $end, LAB = $HTKlab\n");
	    #if ($HTKlab =~ m/\w+/) {
	    #  print(stderr "ERROR: $TIMIT_DIR/$DialectDir/$spkr/$file:\n\tLine: $_\n");
	    #  print(stderr "ERROR: Timit Label $TIMITLab not Converted $HTKlab\n");
	    #}
	  }
	  printf(FP_MLF "\.\n");
	  printf(FP_WRD_MLF "\.\n");
	  close (FP_HTK_LAB);
	}
      }
    }
  }
}
