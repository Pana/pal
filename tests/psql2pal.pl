#!/usr/bin/perl
use strict;

use DBI;
use GD::Image;


sub askValue {
    my $text = shift;
    my $binf = shift;
    my $bsup = shift;
    print "$text\n";
    my $ans;
    do {
        print "enter a value bw $binf and $bsup: ";
        $ans = <>;
        chomp($ans)
    } while ($ans < $binf or $ans > $bsup);
    return $ans;
}



sub askYesNo {
    my @parms = @_;
    my $ans;
    print "$parms[0]";
    do {
        print "(y/n) ";
        $ans = <>;
        chomp($ans)
    } while ($ans ne 'n' and $ans ne 'y');
    return $ans;
}

sub choose {
   my $text = shift;
   print "$text\n";
   my @list = @_;
   my $i = 0;
   foreach (@list){
      $i++;
      print "$i> $list[$i-1]\n"; 
   }

   my $j;
   do {
      print "(1...$i) ";
      chomp($j = <>);
   } while (!($j > 0 and $j <= $i));
   
   return $list[$j -1];
}

my $dpi = 72;


my $dbname = shift;
my $host = shift;
my $user = shift;
my $file = shift;
my $port;


my $pattern = shift;

my $ans;

if (!defined($dbname) or !defined($user) or !defined($host) or !defined($file)){
    print "Usage ./psql2pal.pl database host[:port] user file [SQLtableNamePattern]\n";
    exit;
}

open (PROBLEM, ">$file");

print PROBLEM "%VERSION 1\n";

($host, $port) = split (/:/, $host);

$port = 5432 if !defined($port);

print "dbname: $dbname\n";
print "user: $user\n";
print "host: $host\n";
print "port: $port\n";

print "Enter passwd for user $user :";

my $passwd = <>;

my $dbh = DBI->connect ("dbi:Pg:dbname='$dbname';host='$host';port=5432","$user", "$passwd") or die "Impossible de se connecter !!";

my $mapExtent = askYesNo("Do you want to specify a map extent ?");

my $xmin = 999999999999;
my $ymin = 999999999999;
my $xmax = -999999999999;
my $ymax = -999999999999;

if ($mapExtent eq 'y'){
    print "X min : ";
    chomp($xmin = <>);
    print "Y min : ";
    chomp($ymin = <>);
    print "X max : ";
    chomp($xmax = <>);
    print "Y max : ";
    chomp($ymax = <>);
}

my $table;
my @tables = $dbh->tables( '', 'public', "$pattern", '', {noprefix => 1} );
my $sth;
my @data;

my ($ppt, $pl, $ppoly);

$ppt = 4;
$pl = 12;
$ppoly = 12;

my @units = ('PIXEL', 'METERS');

print PROBLEM "%DPI $dpi\n";
print PROBLEM "%P_POINT $ppt\n";
print PROBLEM "%P_LINE $pl\n";
print PROBLEM "%P_POLYGON $ppoly\n";

foreach $table (@tables){
    $table =~ s/^public\.//;
    if ($table !~ m/geometry_columns/ and $table !~ m/spatial_ref_sys/){
        $ans = askYesNo("Add $table table into pal problem ? ");
        if ($ans eq 'y'){
            print PROBLEM "%LAYER $table\n";
            $sth = $dbh->column_info('', '', $table, ''); 
            

            my @column;
            while (@data = $sth->fetchrow_array()){
                $data[3] =~ s/\"//g;
                push @column, $data[3];
            }
        

            my $label_c;
            
            my $label_x;
            my $label_y;
            my $font;
            my $fontsize;
            my $scaleRef;
            my $obstacle = askYesNo("Treat layer features as obstacle ?");
            my $priority = askValue ("Layer priority (0 is the best, 1 the worst):", 0, 1);

            my $unit = choose ("Units of label: ", @units);
            my $labelAtr = askYesNo("Do you want to link label with an attribute ?");

            if ($labelAtr eq "y"){
                
                $label_c = choose ("Wich attributes contains the label text ?", @column);
                
                my $fontdir = " /usr/share/fonts/corefonts";
                my @fonts = split(/\n/, `find $fontdir -iname *ttf`);
                
                $font = choose ("Please choose a font :", @fonts);
                if ($unit eq 'METERS'){
                    $scaleRef = askValue (" scale references for label size (1:scale) :", 1, 9999999999999999);
                }
                $fontsize = askValue ("Font size : " , 1, 128);
            }
            else{
                $label_c = $column[0];
                $label_x = askValue ("Enter the label length [$unit]: ", 0, 99999999);
                $label_y = askValue ("Enter the label height [$unit]: ", 0, 99999999);
            }


            my $the_geom_c = choose ("Wich attributes contains the geometry ?", @column);

            my $query; 
            if ($mapExtent eq "y"){
                $query = "select \"$label_c\", AsText(geom) , GeometryType(geom) from (select \"$label_c\", geom(dump(Intersection(geom, bbox))) as geom from (select \"$label_c\", geom, GeometryFromText(\'POLYGON(($xmin $ymin, $xmax $ymin, $xmax $ymax, $xmin $ymax, $xmin $ymin))\', srid) as bbox, srid from (select \"$label_c\", geom(dump(\"$the_geom_c\")) as geom, ST_SRID(\"$the_geom_c\") as srid from \"$table\") as bar) as foo  WHERE ST_Intersects (geom, bbox)) as foobar";
            }
            else{
                $query = "select \"$label_c\", AsText(geom) , GeometryType(geom) from (select \"$label_c\", geom(dump(\"$the_geom_c\")) as geom from \"$table\") as foobar";
            }
            print "Query: $query\n";
            $sth = $dbh->prepare($query);
            $sth->execute();

            if (@data = $sth->fetchrow_array()){

                my @arrangement;
                my $arrange;

                if ($data[2] eq 'POINT'){
                   $arrange = 'P_POINT';
                }
                elsif ($data[2] eq 'LINESTRING'){
                   push @arrangement, 'P_LINE';
                   push @arrangement, 'P_LINE_AROUND';
                   $arrange = "none";
                }
                elsif ($data[2] eq 'POLYGON'){
                   push @arrangement, 'P_POINT';
                   push @arrangement, 'P_LINE';
                   push @arrangement, 'P_LINE_AROUND';
                   push @arrangement, 'P_HORIZ';
                   push @arrangement, 'P_FREE';
                   $arrange = "none";
                }
                else{
                   print "$data[2]: error this type is not handle !";
                }
                
                $arrange = choose('Arrangment ?', @arrangement) if $arrange eq 'none';
               
                print PROBLEM "%TYPE $data[2]\n";
                print PROBLEM "%ARRANGE $arrange\n";
                print PROBLEM "%OBSTACLE\n" if ($obstacle eq 'y');
                print PROBLEM "%UNIT $unit\n";
                print PROBLEM "%PRIORITY $priority\n";
                print PROBLEM "%FEATURES\n";
                my $i = 0;
                do {
                    $i ++;
                    #print "type: $data[2]    label: $data[0]    geom: $data[1]  \n";
                    my $geom = $data[1];
                    $geom =~ s/$data[2]//;
                    $geom =~ s/\(//g;
                    $geom =~ s/\)//g;
                    my @points = split (/,/, $geom);
                    my $points = @points;

                    if ($labelAtr eq "y"){
                        my @bounds = GD::Image->stringFT ('black', $font, $fontsize, 0, 0, 0, $data[0]);
                        $label_x = $bounds[2] - $bounds[0];
                        $label_y = $bounds[3] - $bounds[5];
                        if ($unit eq 'METERS'){
                            $label_x = $scaleRef * $label_x * 0.0254 / $dpi;
                            $label_y = $scaleRef * $label_y * 0.0254 / $dpi;
                        }
                    }

                    print PROBLEM "$label_x $label_y\n";
#                    print PROBLEM "$data[2](";
#                    print PROBLEM "(" if $data[2] eq 'POLYGON'; 
                    my $i;
                    for ($i=0;$i<$points;$i++){
                       my ($x, $y) = split (/ /, $points[$i]);
                       if ($mapExtent eq 'n'){
                           $xmin = $x if $x < $xmin;
                           $ymin = $y if $y < $ymin;
                           $xmax = $x if $x > $xmax;
                           $ymax = $y if $y > $ymax;
                       }
#                       print PROBLEM "$x $y";
#                       print PROBLEM "," if $i < $points - 1;
                    }
                    if ($points[0] ne $points[-1]){
                       my ($x, $y) = split (/ /, $points[0]);
#                       print PROBLEM ",$x $y";
                    }
#                    print PROBLEM ")" if $data[2] eq 'POLYGON'; 
#                    print PROBLEM ")\n";
                    print PROBLEM "$data[1]\n";
                    
                } while (@data = $sth->fetchrow_array());
                print "$i features added\n";
            }
        }
    }

}
print PROBLEM "%MAP_EXTENT $xmin $ymin $xmax $ymax\n";

close (PROBLEM);
