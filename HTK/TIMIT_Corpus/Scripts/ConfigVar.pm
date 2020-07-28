# -*-perl-*-

package ConfigVar;
require Exporter;
@ISA    = qw(Exporter);
@EXPORT = qw(ParseConfig CheckFileExist);

use Getopt::Long;


##########################################################################
# ParseConfig                                                            #
#  Args     - (Reference) - Command line switches                        #
#  Default  - (Reference) - Hash containing default values               #
#  Param    - (Scalar)    - Name of config Hash table                    #
#                                                                        #
#   ParseConfig parses command line arguments taking values from the     #
#   following three sources:                                             #
#     Default     - Input default parameter values                       #
#     ConfigFile  - Optional user config file                            #
#     CommandLine - Command line overrides                               #
#                                                                        #
#   Also load in global configuration file                               #
#                                                                        #
##########################################################################
sub ParseConfig {
  my @Args    = @{ shift (@_) };
  my %Default = %{ shift (@_) };
  my $Param   = shift (@_);
  my %Options, %Vars;
  my $x, $key;

  # create Options for command line parse
  foreach $x (@Args) {
    $Options{$x} = \$Vars{$x};
  }
  # add default command line parameters
  $Options{"config=s"} = \$Vars{config};
  $Options{print}      = \$Vars{print};
  #
  # adding other options with GetOptions
  #
  Getopt::Long::GetOptions(%Options);

  if (defined $Vars{print}) {
    printf "\nCOMMAND LINE: ARGS\n";
    foreach $elem (@Args) {
      printf "%15s\n", $elem;
    }

    printf "\n\nDEFAULT: ARGS\n";
    foreach $key (sort keys %Default) {
      printf "%15s => %s\n", $key, $Default{$key};
    }

    printf "\n\nPARAM: ARGS\n\n";
    printf "%15s\n", $Param;
  }

  # load global config file and evaluate
  if (defined $ENV{GLOBALCONFIG}) {
    open(CONFIG, $ENV{GLOBALCONFIG}) or 
        die "Could not load config file: $ENV{GLOBALCONFIG}\n";;
    $x = join "", <CONFIG>;
    eval($x) or
        die "Error in config file: $@";
  }

  # load config file and evaluate
  if (defined $Vars{config}) {
    open(CONFIG, $Vars{config}) or 
        die "Could not load config file: $Vars{config}\n";;
    $x = join "", <CONFIG>;
    close CONFIG;
    eval($x) or 
        die "Error in config file: $@";
  }

  # copy from command line
  if (defined $Vars{print}) {
    print "\nCOMMAND LINE:\n";
  }

  foreach $x (keys %Vars) {
    if (defined $Vars{print}) {
      printf "key = %s\n", $x;
    }
    if (defined $Vars{$x}) {
      split /=/, $x;
      $$Param{$_[0]} = $Vars{$x};
      if (defined $Vars{print}) {
	printf "\nsplit key = %15s => %s INDX %15s\n", $x, $Vars{$x}, $_[0];
      }
    }
  }

  # copy in defaults if not defined
  foreach $x (keys %Default) {
    if (not defined $$Param{$x}) {
      $$Param{$x} = $Default{$x};
    }
  }

  # print to screen
  if (defined $Vars{print}) {
    printf "\n";
    foreach $x (sort keys %$Param) {
      printf "%-30s => %s\n", "$Param\{$x\}", $$Param{$x};
    }

    print "\n";
    foreach $x (sort keys %GLOBAL) {
      printf "%-20s => %s\n", "GLOBAL\{$x\}", $GLOBAL{$x};
    }
    exit;
  }
}
##########################################################################
# CheckFileExist                                                         #
#   Check that key files exists as defined by keys in Hash table         #
##########################################################################
sub CheckFileExist {
   my %Hash = %{ shift(@_) }; 

   $fl = 0;
   foreach $x (@_) {
       if(! -e $Hash{$x}) {
	   $fl = 1;
	   print "File or Directory ($x=$Hash{$x}) does not exists.\n";
       }
   }
   die if $fl;
}
