#!/usr/bin/perl 

use strict;
use File::Basename;

# batch (directory name, scale, @problems.pal)

sub batch{
    my $directory = shift;
    my $scale = shift;
    my @problems = @_;
    my $stat = basename($directory).".csv";

    open (STAT, ">$stat");

    system ("uname -a");

    print STAT  "name\tscale\t#ft\t#cdts\t#ov\t#cdts_r\t#ov_r\tc(Sfalp)\td(Sfalp)\t%(sFalp)\tsubSum\tmethod\tpop_r\tit\tpart_t\tinit_t\tsearch_t\tfsearch_t\tc(S)\td(S)\t%(s)\tpCreat_t\ttotal_t\n";

    close (STAT);

    my $method;

    foreach $method ("CHAIN", "POP_CHAIN", "POP_TABU", "POP_TABU_CHAIN"){
        foreach (@problems){
            system ("pal_labeller $_ $scale $method 2>> $stat\n");
        }
    }
}


sub usage(){
    print "Usage : pal_batch.pl <problems directory>  [scale]\n";
    exit (-1);
}


my $problems_directory = shift;
my $scale = shift;


if (!defined($problems_directory)){
    usage();
}

if (!defined($scale)){
    $scale = 1000;
}


my @dir_stack;

push @dir_stack, $problems_directory;
my $dir;

while ($dir = shift @dir_stack){
    opendir (DIR, $dir) or die ("error while opening $dir: $!");
    my @entries = readdir DIR;

    my @subdirs = grep -d, map{"$dir/$_"}  grep !/^\./, @entries;
    my @palProb = sort map{"$dir/$_"}  grep /\.pal$/, grep !/^\./, @entries;

    push @dir_stack, @subdirs;

    if (scalar @palProb > 0){
       batch ($dir, $scale, @palProb);
    }

}
