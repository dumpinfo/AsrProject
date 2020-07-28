# -*-Perl-*-

use ConfigVar;
use File::Basename;

$line        = join " ", @ARGV;

if (@ARGV < 1) {
  print STDERR "Usage: $0 dict1 dictSTD\n";
  print "[$#ARGV] $line\n";
  exit;
}
$newDict = shift;
$stdDict = shift;

open FP_NEW, "<$newDict"
  or die "ERROR: Unable to open file $newDict for reading: $!\n";

$countNew = 0;
while (<FP_NEW>) {
  chomp;
  $word = $_;
  $word =~ s/^\s+//;
  $word =~ s/\s+$//;
  $HnewDict{$word} = $word;
  $countNew++;
}
printf(stderr "Loaded %d words form %s dictinonary\n", $countNew, $newDict);

open FP_STD, "<$stdDict"
  or die "ERROR: Unable to open file $stdDict for reading: $!\n";

$countStd = 0;
while (<FP_STD>) {
  chomp;
  $line = $_;
  ($word, @rest) = split /\s+/, $line;
  #printf(stderr "%s => %s\n", $word, @rest);
  $word =~ s/^\s+//;
  $word =~ s/\s+$//;

  $HstdDict{$word} = $word;
  $countStd++;
}
printf(stderr "Loaded %d words form %s dictinonary\n", $countStd, $stdDict);

printf(stderr "Missing Words in STD Dictionary %s:\n", $stdDict);
foreach $newWord (sort keys %HnewDict) {
  if (! defined $HstdDict{$newWord}) {
    printf(stderr "%s\n", $newWord);
  }
}
