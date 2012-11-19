#!/usr/bin/perl
use LWP::Simple;

$outputFile = $ARGV[0];
$numOfVals = $ARGV[1];
$upperBound = $ARGV[2];
for ($i = 0; $i < $numOfVals; $i++){
  
  
  for ($j = 0; $j < 6; $j++){
    $random_number = rand($upperBound);
    if ($j != 0){
      $insert = $insert . "\t" . int($random_number);
    } else {
      $insert = $insert . int($random_number);
    }
  }
  if ($i != $numOfVals-1){
    $insert = $insert . "\n";
  }
  
}


open ($outfile, '>' . $outputFile);
print $outfile $insert;
close ($outfile); 
