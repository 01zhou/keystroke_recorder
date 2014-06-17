$event = $ARGV[0];

if($event =~ /(KeyPress event|KeyRelease event)/)
{
    open(F_LOG, '>>log');
    if($1 eq 'KeyPress event')
    {
        $type = 0;
    }
    else
    {
        $type = 1;
    }

    $time = $1 if($event =~ /time (\d+)/);
    $key = $1 if($event =~ /keysym 0x([0-9abcdef]+)/);

    $dkey = hex($key);

    print F_LOG "$type $time $dkey\n";
    close F_LOG;
    
    if($dkey>=0xff00)
    {
        $dkey -= 0xff00;
    }

    if($dkey<128 && $type == 0)
    {
        print chr($dkey);
    }
}
