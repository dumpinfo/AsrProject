# -*-Perl-*-

$line        = join " ", @ARGV;

if (@ARGV < 1) {
  print STDERR "Usage: $0 PROMPTS.TXT\n";
  print "[$#ARGV] $line\n";
  exit;
}

$promptsFile = shift;
$listFile    = "prompts.list";
$wlistFile   = "words.list";

open FP, "<$promptsFile"
  or die "ERROR opening file $promptsFile for reading: $!\n";

open FP_PL, ">$listFile"
  or die "ERROR opening file $listFile for reading: $!\n";

open FP_WL, ">$wlistFile"
  or die "ERROR opening file $wlistFile for reading: $!\n";


$count = 0;
$promptCount = 1;

while (<FP>) {
  chomp;
  $count++;

  if (m/^;/) {next;}

  $line = $_;
  $line =~ s/\.//;
  $line =~ s/\s+$//;
  $line =~ s/\.$//;
  $line =~ tr /a-z/A-Z/;
  $line =~ s/\?$//g;
  $line =~ s/\!$//g;
  $line =~ s/\-\-//g;
  $line =~ s/\"//g;
  $line =~ s/\,//g;
  $line =~ s/\;//g;

  if ($line =~ m/^\(\w+\)$/) {
    printf(stderr "ERROR: Processing Prompt file $promtpsFile: line # $count\n");
    exit;
  }

  undef @Sentence;
  @Sentence = split/\s+/, $line;
  $Sfile    = $Sentence[$#Sentence];
  $Sfile    =~ s/\(//;
  $Sfile    =~ s/\)//;

  $sentence = join ' ', @Sentence;
  $sentence =~ s/\(\w+\)$//;
  $sentence =~ s/\?//g;
  $sentence =~ s/\!//g;

  printf(FP_PL "%s\t%s\n", $Sfile, $sentence);

  undef @Sentence;
  @Sentence = split/\s+/, $sentence;

  foreach $word (@Sentence) {
    $word =~ s/\s+//g;
    $word =~ s/\?$//;
    $word =~ s/\!$//;

    #if ($word =~ m/\W+/) {
    #  printf(stderr "Sentence: @Sentence => $word\n");
    #}
    if ($word =~ m/\d+/) {
      printf(stderr "Sentence: @Sentence => $word\n");
    }
    $H_word{$word}++;
  }
}

foreach $word (sort %H_word) {
  #printf(FP_WL "%s\t%d\n", $word, $H_word{$word});
  if ($word =~ m/\D+/) {
    #printf(FP_WL "%s\t%d\n", $word, $H_word{$word});
    printf(FP_WL "%s\n", $word);
  }
  #else {
  #  printf(stderr "ERROR: %s\t%d\n", $word, $H_word{$word});
  #}
}
