package   HTK_funct;
require   Exporter;

@ISA    = qw (Exporter);
@EXPORT = qw (
	      HHEd
	      HLEd
	      HCopy
	      HList
	      HInit
	      HRest
	      HERest
	      HQuant
	      HSmooth
	      HResults
	      HVite
	      doit
	     );

# Pre-Compiled Set
$HHED      = "c:/HTK/htk/HHEd";
$HLED      = "c:/HTK/htk/HLEd";
$HCOPY     = "c:/HTK/htk/HCopy";
$HLIST     = "c:/HTK/htk/HList";
$HINIT     = "c:/HTK/htk/HInit";
$HREST     = "c:/HTK/htk/HRest";
$HEREST    = "c:/HTK/htk/HERest";
$HQUANT    = "c:/HTK/htk/HQuant";
$HSMOOTH   = "c:/HTK/htk/HSmooth";
$HRESULTS  = "c:/HTK/htk/HResults";
$HVITE     = "c:/HTK/htk/HVite";

# Compiled Set on this machine
$HHED      = "HHEd";
$HLED      = "HLEd";
$HCOPY     = "HCopy";
$HLIST     = "HList";
$HINIT     = "HInit";
$HREST     = "HRest";
$HEREST    = "HERest";
$HQUANT    = "HQuant";
$HSMOOTH   = "HSmooth";
$HRESULTS  = "HResults";
$HVITE     = "HVite";

$MAKEPROTO = "MakeProtoHMMSet.pl";

$hinitConf  = "toolsconfs/hinit.conf";  # HInit conf file
$hrestConf  = "toolsconfs/hrest.conf";  # HRest conf file
$herestConf = "toolsconfs/herest.conf"; # HERest conf file

#----------------------------------------------
# MkStdMonSys: make a standard monophone system
#----------------------------------------------
sub MkStdMonSys {
  local ($labFiles, $srcFmt) = @_;

  #$labFiles="tidata.scr";
  #$srcFmt="-G TIMIT";

  $protoName="proto_s${nS}_m";
  if ($configParams{"hsKind"} =~ /^[dD]/) {
    &AddMixNums();
    $tmp=$protoName."_vq.pcf";
    $protoName = $tmp;
  } else {
    $protoName="proto_s${nS}_m1_dc.pcf";
  }

  (-f "protoconfs\\$protoName")
    || die "Cannot find proto config file protoconfs\\$protoName\n";

  if ($configParams{"HLEd"} =~ /^[yY]/) {
    &HLEd($srcFmt,$labDir,$hmmList,$edFile,$labFiles);
  }

  &CleanUp();

  &HInit($monLabDir,$protoName,$hmmList);
  &HRest($monLabDir,$hmmList);
  &HERest($numIters,$labDir,$hmmList);
}
#-------------------------------------------------------------------
# TestDirEmpty: Tests if directory is empty and prompts user for the
#               deletion of any files found
#-------------------------------------------------------------------
sub TestDirEmpty {
  # Get arguments
  local($dirName, $srcOrTgt, $tool) = @_;
  local(@nFiles,$rtnVal);

  print(STDERR "Testing if Directory is Empty:\n\tDIR = $dirName\n\tsrcORtgt = $srcOrTft\n\ttool = $tool\n");

  $rtnVal=1;
  opendir(DIR,$dirName) || die "Can't open $dirName\n";

  @nFiles = grep(!/^\./, readdir(DIR)); #Forget about . files

  if ($nFiles[0]) {
    if ($srcOrTgt eq "tgt") {
      print "\n$dirName not empty, overwrite using $tool Y/N?:";
      chop($ans = <STDIN>);
      if ($ans =~ /^[yY]/) {
	print "\nRemoving files from $dirName\n";
	system("del /Q $dirName\\*");
	$rtnVal=1;
      } else {
	print "\nDirectory $dirName unaltered, skipping to next test\n";
	$rtnVal=0;
      }
    }
  } else {
    if ($srcOrTgt eq "src") {
      die "Source Directory Empty $dirName\n";
    }
  }
  $rtnVal;
}
#-------------------------------------------------------------------
# SetMacroStr: Generate -H option string dependent on whether source
#              directory contains a macro file
#------------------------------------------------------------------- 
sub SetMacroStr {
  # Get arguments
  local($srcDir) = @_;

  if (-r $srcDir."/newMacros") {
    $macroStr = "-H ".$srcDir."/newMacros";
  } else {
    $macroStr = "";
  }
}
#************************** HTK Tool Functions ********************************

#------------------------------------
# HLEd: Invokes the label editor HLEd
#------------------------------------
sub HLEd {
  # Get arguments
  local($srcFmt,$labDir,$hmmList,$ledFile,$labFiles) = @_;

  if (&TestDirEmpty($labDir,"tgt","HLEd")) {
    doit("$HLED $aOptStr $srcFmt -l $labDir -n $hmmList -D -T 1 -S $labFiles $ledFile");
  }
}
#----------------------------------------------------------------------------
# HList: Invokes HList to display the headers of all parameterised data files
#        created for training together with the 1st 10 lines of the 1st file.
#----------------------------------------------------------------------------
sub HList {
  local($tr1fname)="data\\train\\tr1.mfc";

  print (STDOUT "Display some of the data files\n");
  print (STDOUT "This demonstrates the use of HList\n");

  unless(-r $tr1fname){
    die "Cannot read file $tr1fname\n";
  }
  print (STDOUT"\n\nTraining data file headers\n");
  system("$HLIST $aOptStr -D -h -z -S $trDataFiles");
  print (STDOUT "\n\nFirst 10 frames of tr1.mfc (with deltas appended)\n");
  system("$HLIST -aOptStr -e 10 $tr1fname");
}
#---------------------------------------------------------------
# HInit: Calls initialisation tool HInit for each HMM in hmmList
#---------------------------------------------------------------
sub HInit {
  # Get arguments
  #local($labDir, $protoName, $hmmList, $tgtDir, $srcDir) = @_;
  local($mlfFile, $protoName, $hmmList, $tgtDir, $srcDir, $dataFiles) = @_;
  #local($tgtDir)="hmms/hmm.0";
  #local($srcDir)="mono_proto";

  print(STDERR "Running HInit Script:\n");
  print(STDERR "\tMLF = $mlfFile\n\tPROTO = $protoName\n\tHMM's = $hmmList\n");
  print(STDERR "\t\TGTDIR = $tgtDir\n\tSCRDIR = $scrDir\n\tDATA = $dataFiles\n\n");

  doit("c:/perl/bin/perl $MAKEPROTO protoconfs/$protoName");

  &SetMacroStr($srcDir);
  print(STDERR "Called SetMacroStr Function\n");

  #if (&TestDirEmpty($tgtDir,"tgt","HInit")) {

  print(STDERR "Opening HMMLIST = $hmmList\n");
  open(HMMLIST, $hmmList);

  while (<HMMLIST>) {		# read HMM name into $_
    chop($_);
    print(STDOUT "\n\nCalling HInit for HMM ",$_,"\n");
    #system("($HINIT $aOptStr -i 10 -L $labDir -l $_ -o $_ $macroStr -C$hinitConf -D -M $tgtDir -T 1 -S $dataFiles $srcDir\$_)");
    #doit("$HINIT $aOptStr -i 10 -L $labDir -l $_ -o $_ $macroStr -C $hinitConf -D -M $tgtDir -T 1 -S $dataFiles $srcDir/$_");
	
    doit("$HINIT $aOptStr -i 10 -I $mlfFile -l $_ -o $_ -C $hinitConf -D -M $tgtDir -T 1 -S $dataFiles $srcDir/$_");
	
  }
  close(HMMLIST);
}
#}
#-----------------------------------------------------------------------
# HRest: Calls isolated re-estimation tool HRest for each HMM in hmmList
#-----------------------------------------------------------------------
sub HRest {
  # Get arguments
  #local($labDir,$hmmList,$srcDir,$tgtDir) = @_;
  local($mlfFile, $hmmList, $tgtDir, $srcDir, $dataFiles) = @_;

  #local($srcDir,$tgtDir);
  #$srcDir="hmms\\hmm.0";
  #$tgtDir="hmms\\hmm.1";

  print(STDERR "Running HRest Script:\n");
  print(STDERR "\tMLF     = $mlfFile\n\tHMM's   = $hmmList\n");
  print(STDERR "\t\TGTDIR = $tgtDir\n\tSCRDIR = $srcDir\n\tDATA   = $dataFiles\n\n");

  &SetMacroStr($srcDir);
  print(STDERR "Called SetMacroStr Function\n");

  &TestDirEmpty($srcDir,"src","HRest");

  #if (&TestDirEmpty($tgtDir,"tgt","HRest")) {

  print(STDERR "Opening HMMLIST = $hmmList\n");
  open(HMMLIST, $hmmList);

  while (<HMMLIST>) {		# read HMM name into $_
    chop($_);
    print(STDOUT "\n\nCalling HRest for HMM ",$_,"\n");
    #doit("($HREST $aOptStr -u tmvw -w 3 -v 0.05 -i 30 -L $labDir -l $_ -C $hrestConf $macroStr -D -M $tgtDir -T 1 -S $dataFiles $srcDir\\$_)");
    doit("($HREST $aOptStr -u tmvw -w 3 -v 0.05 -i 30 -I $mlfFile -l $_ -C $hrestConf $macroStr -D -M $tgtDir -T 1 -S $dataFiles $srcDir/$_)");
  }
  close(HMMLIST);
  #}
}
#-------------------------------------------------------------------------
# HERest: Calls embedded re-estimation tool HERest on all HMMs in hmmList
#         for the required number of iterations
#-------------------------------------------------------------------------
sub HERest {
  # Get arguments
  #local($numIters,$labDir,$hmmList,$tgtDir,$srcDir) = @_;
  local($numIters, $mlfFile, $hmmList, $tgtDir, $srcDir, $dataFiles) = @_;
  local($tmpDir,$i);
  #local($srcDir,$tgtDir,$tmpDir,$i);

  #$srcDir="hmms\\hmm.1";
  #$tgtDir="hmms\\hmm.2";
  $tmpDir="hmms/tmp";

  &TestDirEmpty($srcDir,"src","HERest");

  &SetMacroStr($srcDir);

  if (&TestDirEmpty($tgtDir,"tgt","HERest")) {
    $i=1;
    while ($i<=$numIters) {
      print (STDOUT "\n\nIteration ",$i," of Embedded Re-estimation\n");
      #system("$HEREST $aOptStr -w 3 -v 0.05 -C $herestConf -u tmvw $cOptStr -d $srcDir $macroStr -D -M $tgtDir -L $labDir -t 2000.0 -T 1 -S $dataFiles $hmmList");
      doit("$HEREST $aOptStr -w 3 -v 0.05 -C $herestConf -u tmvw $cOptStr -d $srcDir $macroStr -D -M $tgtDir -I $mlfFile -t 2000.0 -T 7 -S $dataFiles $hmmList");
      if ($numIters > 1) {
	#system("copy $tgtDir\\* $tmpDir");
	doit("cp $tgtDir/\* $tmpDir");
	$srcDir=$tmpDir;
	&SetMacroStr($srcDir);
      }
      $i++;
    }
    #system("del /Q $tmpDir\\*");
    doit("rm $tmpDir/\*");
  }
}
#---------------------------------------------------------------
# Calling System Function
#---------------------------------------------------------------
sub doit {
  print STDOUT "DOING:\n",$_[0],"\n";
  my($status) = system $_[0];
  if (!$debug) {
    $status = 0xffff & $status;
    if ($status == 0) {
      #print STDERR "normal exit\n\n";
    } elsif ($status == 0xff00) {
      print STDERR "failed: $!\n";
      exit(1);
    } elsif ($status > 0x80) {
      $status >>= 8;
      print STDERR "EXIT: $status\n";
      exit($status);
    } else {
      if ($status & 0x80) {
        $status &= ~0x80;
        print STDERR "SIGNAL: $status (coredump)\n";
      } else {
        print STDERR "SIGNAL: $status";
      }
      exit($status);
    }
  }
}
