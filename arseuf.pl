
use strict;
use warnings;
use HTTP::Tiny;
use Data::Dumper;
use Android;

my $droid = Android->new();

$droid->webViewShow('file:///sdcard/sl4a/scripts/arseuf.html');
while (1) {
  my $res = $droid->waitForEvent('action');
  my $cmd = $res->{result}->{data};
   
  my $response = HTTP::Tiny->new->get('http://192.168.30.15/'.$cmd);
  $droid->makeToast("Error") unless $response->{success};
}