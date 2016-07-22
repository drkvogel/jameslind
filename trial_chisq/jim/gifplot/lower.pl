$done = 0;
opendir( curdir, "." ) || die "Unable to open directory";
while ( $name = readdir( curdir ) )
  {
  $lowname = $name;
  $lowname =~ tr/[A-Z]/[a-z]/;
  if ( $name ne $lowname )
    {
    print "\n$name -> $lowname";
    rename( $name, $lowname );
    $done++;
    }
  }
closedir( curdir );
print "\nDONE $done\n";
