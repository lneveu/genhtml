#!/usr/bin/perl
 
use strict ;
use warnings ;
use Data::Dumper ;
use Carp ;

 


sub read_file($) {
    my $file = shift ;
    open my $IN, "<", $file || return 0 ;

		my $title;
		my $subtitle;
		my $date;
		my $author;
		my $text;
		my $cpt = 1;

    while ( my $line = <$IN> ) {
    	if($cpt == 1){
    		$title = $line;
    	}elsif($cpt == 2){
    		$subtitle = $line;
    	}elsif($cpt == 4){
    		$date = $line;
    	}elsif($cpt == 5){
    		$author = $line;
			}else{
    		$text .= $line;
    	}
    	$cpt++;
    }

		# Conversion date
		my $year;
		my $month;
		my $day;
		$cpt = 1;
		while($date =~ m/(\d+)/g){
			if($cpt == 1){
				$year = $1;
			}elsif($cpt == 2){
				$month = $1;
			}elsif($cpt == 3){
				$day = $1;
			}
			$cpt++;
		}
		$date = dateToString($year,$month,$day);

    #Suppression des div
    $text =~ s/<div.*>//g;
    $text =~ s/<\/div>//g;

    # Suppression des <br>
    $text =~ s/<br\s?\/?>//g;

    # Suppression des <youtube...>
    $text =~ s/<youtube.*>//g;

    #Suppression des <note-image>
    $text =~ s/<note-image\d*>//g;

    #Suppression des <note-galerie>
    $text =~ s/<note-galerie\d*>//g;

    # Suppression des <album...>
    $text =~ s/<album.*>//g;

    # Suppression des notes de bas de page [[..]]
    $text =~ s/\[\[.*\]\]//g;

    # Chapo
    $text =~ s/<chapo>\n?(.*?)<\/chapo>/\n\n<div class=\"chapo"><p>$1<\/p><\/div>\n/g;

    # Quotes
    $text =~ s/<quote>\n?(.*?)<\/quote>/\n\n<q>$1<\/q>\n/g;

    # Poesie
    $text =~ s/<poesie>\n?(.*?)<\/poesie>/\n\n<q>$1<\/q>\n/g;

    # Images
    $text =~ s/<image=(http:\/\/.*?)>/\n\n<div class=\"border\"><img src=\"$1\" alt=\"#\" class=\"miniature\"\/><\/div>\n\n/g;

    # Iframe
    $text =~ s/<iframe.*?src=\"(.*?)\".*?>.*?<\/iframe>/\n\n<div class=\"iframe-wrapper\"><iframe class=\"iframe-content\" src=\"$1\"><\/iframe><div class=\"iframe-blocker\"><\/div><\/div>\n\n/gs;
   
    # <note-texte|texte=...
    $text =~ s/<note-texte\|texte=(.*?)>/<div class=\"marge\"><div class=\"inner\"><p>$1<\/p><\/div><\/div>\n\n/g;

    # H3 : {{{ }}}
    $text =~ s/{{{\n?(.*?\n*.*?)}}}/<h3>$1<\/h3>/g;
    
    # Gras : {{ }}
    $text =~ s/{{\n?(.*?\n*.*?)}}/<b>$1<\/b>/g;

    # Italique : { }
    $text =~ s/{\n?(.*?\n*.*?)}/<i>$1<\/i>/g;

    # <li> : -
    $text =~ s/^- (.*)\n/<li><p>$1<\/p><\/li>\n/mg;

    # Rajout des <ul> autour des <li>
    $text =~ s/(<li>(.*<\/li>\n<li>.*)*<\/li>)/<ul>\n$1\n<\/ul>/g;
    
    # [couleur]
    $text =~ s/\[([a-z\s]+)\]/<span class=\"$1\">/g;
    $text =~ s/\[\/[a-z\s]+\]/<\/span>/g;

    # Link
    $text =~ s/\[(.*?)(->)+(.*?)\]/<a href=\"$3\" class=\"link\">$1<\/a>/g;

    # Paragraphes
    $text =~ s/(^[^<\n]+.+|.+[^\/\n>]+$)/<p>$1<\/p>\n\n/gm;

    # Paragraphes autour des <b> sur une seule ligne
    $text =~ s/(^<b>.*?<\/b>$)/<p>$1<\/p>\n\n/gm;

    # Paragraphes autour des <q> sur une seule ligne
    $text =~ s/(^<q>.*?<\/q>$)/<p>$1<\/p>\n\n/gm;

    #print $text;

   

    


    

    #Supprimer les <p> des <ul>, <img>, <h3> et <div marge>
    $text =~ s/<p>(<\/ul>)<\/p>/$1/g;
    $text =~ s/<p>( ?<img .*?\/>)<\/p>/$1/g;
    $text =~ s/<p>(<h3>.*?<\/h3>)<\/p>/$1/g;
    $text =~ s/<p>(<div class=\"marge\">.*<\/div>)<\/p>/$1/g;

    
    return $title,$subtitle,$date,$author,$text ;
}
 
sub print_file($$$$$$) {
	my $file_out = shift;
	my $title = shift;
	my $subtitle = shift;
	my $date = shift;
	my $author = shift;
	my $text = shift;

	open my $OUT, ">", $file_out || return 0 ;

	my $head = "<html>
					<head>
						<meta charset=\"UTF-8\">
						<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">  

						<title>".$title."</title>
						<link rel=\"stylesheet\" href=\"ressources/style.css\" type=\"text/css\">

						<script src=\"http://code.jquery.com/jquery-1.11.2.min.js\"></script>
						<script type=\"text/javascript\" src=\"ressources/libjs.js\"></script>

					</head>
					<body>
						<div class=\"lines\"></div>
						<div id=\"popup_img\" class=\"popup\"><img id=\"imgpopup\" class=\"imgpopup\" src=\"\"/></div>

						<div class=\"container\">
							<div class=\"top\">
							<div class=\"header\">
								<div class=\"author\">".$author." - ".$date."</div>
								<div class=\"title\">".$title."</div>
								<div class=\"sub-title\">$subtitle</div>
							</div>
							</div>
							<div class=\"content\">";


    

    print $OUT $head;
    print $OUT $text;

    my $foot ="</div>
		          </div>
	                  </body>
                           </html>";
    print $OUT $foot;
    close $OUT;
    return $OUT ;
}

sub dateToString($$$){
	my $year = shift;
	my $month = shift;
	my $day = shift;


	my $date = $day;

    if ($month eq '01') { $date .= " janvier ";
    }elsif($month eq '02') { $date .= " février ";
    }elsif($month eq '03') { $date .= " mars ";
    }elsif($month eq '04') { $date .= " avril ";
    }elsif($month eq '05') { $date .= " mai ";
    }elsif($month eq '06') { $date .= " juin ";
    }elsif($month eq '07') { $date .= " juillet ";
    }elsif($month eq '08') { $date .= " août ";
    }elsif($month eq '09') { $date .= " septembre ";
    }elsif($month eq '10') { $date .= " octobre ";
    }elsif($month eq '11') { $date .= " novembre ";
    }elsif($month eq '12') { $date .= " décembre ";}

	$date .= $year;
	return $date;
}
 
my $file_in  = $ARGV[0] ;
my $file_out = $ARGV[1];
 
my ($title,$subtitle,$date,$author,$text) = read_file($file_in);
if ( not $title ) {
   print "A problem occurs while reading file $file_in\n" ;
   exit 0 ;
}
 
my $res = print_file($file_out, $title,$subtitle,$date,$author,$text);
if ( not $res ) {
    print "A problem occurs while writing file $file_out\n" ;
    exit 0 ;
}
 
exit 1 ;