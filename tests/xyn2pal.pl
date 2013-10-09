#!/usr/bin/perl
use strict;

my $dpi = 72;

my $xyn_file = shift;
my $pal_file = shift;


if (!defined($xyn_file) or !defined($pal_file)){
    print "Usage ./xyn2pal.pl <xy filename> <pal filename>\n";
    exit;
}
open (XYN, "$xyn_file");

my $xmin = 999999999999;
my $ymin = 999999999999;
my $xmax = -999999999999;
my $ymax = -999999999999;

my $ppt;  # &Lablpositions: n
my $layer;
my ($label_x, $label_y);

$label_x = -1;
$label_y = -1;

my $labelOK = 0;

my $line;

$_ = <XYN>;
chomp();

while (m/^%/){
    s/^%//;
    my ($property, $value) = split (/:/);

    if ($property eq "Filename"){
       $layer = $value;
       $layer =~ s/\..*$//;
       print "The layer is : $layer\n";
    }
    elsif ($property eq "Labelpositions"){
        $ppt = $value;
        print "Point_p : $ppt\n";
    }
    elsif ($property eq "Labelsize"){
        $value =~ s/^\s*//;
        ($label_x, $label_y) = split (/ /, $value);
        print "Label Size :$label_x;$label_y\n";
        $labelOK = 1;
    }
    else{
        print "$property is skipped\n";
    }

    $_ = <XYN>;
    chomp();
}

open (PROBLEM, ">$pal_file");
print PROBLEM "%VERSION 1\n";

print PROBLEM "%DPI $dpi\n";
print PROBLEM "%P_POINT $ppt\n";
print PROBLEM "%LAYER $layer\n";
print PROBLEM "%ARRANGE P_POINT\n";
print PROBLEM "%UNIT METERS\n";
print PROBLEM "%PRIORITY 0.1\n";
print PROBLEM "%FEATURES\n";

while ($_ eq ""){
   $_ = <XYN>;
   chomp();
}

$line = $_;
do{

    my ($x,$y);
    
    if ($labelOK == 1){
       ($x, $y) = split (/ /, $line);
    }
    else{
       ($x, $y, $label_x, $label_y) = split (/ /, $line);
    }
    
    print PROBLEM "$label_x $label_y\n";

    $xmin = $x if $x < $xmin;
    $ymin = $y if $y < $ymin;
    $xmax = $x if $x > $xmax;
    $ymax = $y if $y > $ymax;

    print PROBLEM "POINT($x $y)\n"
} while (chomp($line = <XYN>));

print PROBLEM "%MAP_EXTENT $xmin $ymin $xmax $ymax\n";

close (PROBLEM);
close (XYN);
