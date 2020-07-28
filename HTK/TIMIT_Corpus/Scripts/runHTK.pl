# -*-Perl-*-

use ConfigVar;
use File::Basename;
use HTK_funct;
use TIMIT;

$line        = join " ", @ARGV;

if ((@ARGV < 1) || ($line !~ /config/)) {
  print STDERR "Usage: $0 -config file.cfg\n";
  print "[$#ARGV] $line\n";
  exit;
}
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
#
# Excuting ParseConfig from ConfigVar.pm
#
print stderr "runHTK: Executing ParseConfig ";
ParseConfig (\@CommandLine, \%Default, CORPUS);
print stderr "... Done\n";

%CORPUS        = %ConfigVar::CORPUS;

#foreach $key (sort %CORPUS) {
#  print "Key: $key \t=> Value: $CORPUS{$key}";
#}

$debug         = $CORPUS{debug};
$ver           = $CORPUS{ver};
$corpus        = $CORPUS{CorpusName};
$root_dir      = $CORPUS{RootDir};
$out_dir       = $CORPUS{OutputDir};
$sample_rate   = $CORPUS{SampleRate};
@HTKmonoPhones = @{$CORPUS{HTKmonoPhones}};
%HTKLabMap     = %{$CORPUS{LabMap}};
%HSteps        = %{$CORPUS{HSteps}};

if ($HSteps{Test}) {
  # Creating MLF file and feature files
  if ($HSteps{MLF}) {
    print stderr "Calling: create_MLF\n";
    create_MLF();
  }
}

if ($HSteps{Train}) {
#
# Changinf working dir to out_dir
#
  chdir ${out_dir};

  $MonoPhones    = "lists/${corpus}monoPhonesList";
  $featureFiles  = "train_mfc.scp";
  $MLF           = "${corpus}\.mlf";
  $protoName     = "proto_s1_m1_dc_${corpus}\.pcf";

  if ($HSteps{"HInit"}) {
    $srcDir        = "proto";
    $tgtDir        = "hmms/hmm.0";
    HInit($MLF, $protoName, $MonoPhones, $tgtDir, $srcDir, $featureFiles);
  }

  if ($HSteps{"HRest"}) {
    $srcDir        = "hmms/hmm.0";
    $tgtDir        = "hmms/hmm.1";
    #print(STDERR "Calling HRest Script:\n");
    #print(STDERR "\tMLF     = $MLF\n\tHMM's   = $MonoPhones\n");
    #print(STDERR "\t\TGTDIR = $tgtDir\n\tSCRDIR = $srcDir\n\tDATA   = $featureFiles\n\n");
    HRest($MLF, $MonoPhones, $tgtDir, $srcDir, $featureFiles);
  }

  if ($HSteps{"HERest"}) {
    $srcDir        = "hmms/hmm.1";
    $tgtDir        = "hmms/hmm.2";
    $numIter       = 3;
    #print(STDERR "Calling HRest Script:\n");
    #print(STDERR "\tMLF     = $MLF\n\tHMM's   = $MonoPhones\n");
    #print(STDERR "\t\TGTDIR = $tgtDir\n\tSCRDIR = $srcDir\n\tDATA   = $featureFiles\n\n");
    HERest($numIter, $MLF, $MonoPhones, $tgtDir, $srcDir, $featureFiles);
  }
}
