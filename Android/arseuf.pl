# Copyright 2012, Thierry Sallé
#
# This file is part of ArSeuf.
#  
#  ArSeuf is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  ArSeuf is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with ArSeuf.  If not, see <http://www.gnu.org/licenses/>.



use strict;
use warnings;
use HTTP::Tiny;
use Data::Dumper;
use Android;

my $droid = Android->new();

$droid->webViewShow('file:///sdcard/sl4a/scripts/arseuf.html');
while (1) {
  #my $res = $droid->waitForEvent('action');
  my $res = $droid->eventWait();
 
  print Dumper($res); 
  my $cmd = $res->{result}->{data};
   
#  my $response = HTTP::Tiny->new->get('http://192.168.30.15/'.$cmd);
#  $droid->makeToast("Error") unless $response->{success};
}
